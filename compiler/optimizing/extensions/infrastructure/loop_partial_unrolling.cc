/*
 * Copyright (C) 2018 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "cloning.h"
#include "ext_utility.h"
#include "graph_x86.h"
#include "loop_information.h"
#include "induction_variable.h"
#include "loop_partial_unrolling.h"
#include "optimization_x86.h"
#include <iostream>

namespace art {

HLoopPartialUnrolling::HLoopPartialUnrolling(HLoopInformation_X86* loop, HOptimization_X86* optim)
    : loop_(loop),
      graph_(GRAPH_TO_GRAPH_X86(loop_->GetGraph())),
      loop_instructions_(GRAPH_TO_GRAPH_X86(loop_->GetGraph())->GetArena()->Adapter(kArenaAllocMisc)),
      cloner_(graph_, true, true, true),
      optim_(optim) {}

static bool IsUselessInstruction(HInstruction* insn) {

  //Omit loop suspend checks as they need not be cloned
  switch (insn->GetKind()) {
    case HInstruction::kTestSuspend:
    case HInstruction::kSuspendCheck:
    case HInstruction::kSuspend:
      return true;
    default:
      break;
  }

  return false;
}

void HLoopPartialUnrolling::DumpBB(HBasicBlock* block){

  std::cout << "block id" << block->GetBlockId() << std::endl;
  std::cout << "Predecessors=" << std::endl;
  for (HBasicBlock* pred : block->GetPredecessors()){
    std::cout << pred->GetBlockId() << std::endl;
  }
  std::cout << "successors=" << std::endl;
  for (HBasicBlock* succ : block->GetSuccessors()){
    std::cout << succ->GetBlockId() << std::endl;
  }
}

void HLoopPartialUnrolling::DumpLoop(HLoopInformation_X86* loop){

  std::cout << "Loops in block" << std::endl;
  for (HBlocksInLoopIterator it_loop(*loop); !it_loop.Done(); it_loop.Advance()) {
    HBasicBlock* block = it_loop.Current();
    DumpBB(block);
    DumpInstructions(block);
  }
  std::cout << "Back Edges" << std::endl;
  for (HBasicBlock* back_edge : loop->GetBackEdges())
    std::cout << "back_edge="<< back_edge->GetBlockId() << std::endl;
}

void  HLoopPartialUnrolling::DumpInstructions(HBasicBlock* block){

  for (HInstruction* instruction = block->GetFirstInstruction();
    instruction != nullptr;
    instruction = instruction->GetNext()) {
    std::cout << "ins=" << instruction->DebugName() <<"::"<< instruction->GetId() << std::endl;
  }
}

void HLoopPartialUnrolling::CloneInstructionsForUnroll(HInstructionCloner& cloner){

  for (HInstruction* instruction : loop_instructions_) {
    if (IsUselessInstruction(instruction)) {
        // No need to copy useless instructions
       continue;
    } else {
        instruction->Accept(&cloner);
    }
  }
}

void HLoopPartialUnrolling::MapLoopPhiInputs(){

   HBasicBlock* loop_header = loop_->GetHeader();
   for (HInstructionIterator it(loop_header->GetPhis()); !it.Done(); it.Advance()) {
     HPhi* phi = it.Current()->AsPhi();
     DCHECK(phi->IsLoopHeaderPhi());
     DCHECK_EQ(phi->InputCount(),2u);
     HInstruction* in_loop_input = loop_->PhiInput(phi, /*in_loop_input*/true);
     phi_to_loop_inputs_.Put(phi,in_loop_input);
   }
}

void HLoopPartialUnrolling::UpdateLoopPhiInputsMap(HInstructionCloner& cloner){

  HBasicBlock* loop_header = loop_->GetHeader();
  //Update users of phis with their in_loop inputs
  for (HInstructionIterator it(loop_header->GetPhis());!it.Done();it.Advance()) {
    HInstruction* instruction = it.Current();
    if (phi_to_loop_inputs_.find(instruction)!=phi_to_loop_inputs_.end()) {
      if (ins_to_use_.find(instruction)!=ins_to_use_.end()){
        ins_to_use_.Overwrite(instruction,phi_to_loop_inputs_.Get(instruction));
      } else {
        ins_to_use_.Put(instruction,phi_to_loop_inputs_.Get(instruction));
      }
    }
  }
  for (HInstruction* instruction : loop_instructions_) {
    HInstruction* replacement = cloner.GetClone(instruction);
    if (ins_to_use_.find(instruction)!= ins_to_use_.end()){
      ins_to_use_.Overwrite(instruction,replacement);
    } else {
      ins_to_use_.Put(instruction,replacement);
   }
  }

  // The phi nodes are updated virtually "in parallel". That means that if one loop header phi
  // depends on another, we should not introduce an ordering dependency between them, but take
  // their old values instead. This temp mapping takes care of that.
  SafeMap<HInstruction*, HInstruction*> temp_mapping;

  for (HInstructionIterator it(loop_header->GetPhis());!it.Done();it.Advance()) {
    HInstruction* instruction = it.Current();
    HPhi* phi = instruction->AsPhi();
    DCHECK(phi->IsLoopHeaderPhi());
    DCHECK_EQ(phi->InputCount(), 2u);
    HInstruction* in_loop_input =  loop_->PhiInput(phi, true);
    if (loop_->Contains(*(in_loop_input->GetBlock()))) {
      // We map it to the latest in loop input instruction.
      if (phi_to_loop_inputs_.find(in_loop_input)!= phi_to_loop_inputs_.end()) {
        HInstruction* to_update = phi_to_loop_inputs_.Get(in_loop_input);

        //We have already updated loop phis at least once by now
        DCHECK(to_update!=nullptr);
        temp_mapping.Put(phi, to_update);
      } else {
        HInstruction* clone = cloner.GetClone(in_loop_input);
        DCHECK(clone!=nullptr);
        temp_mapping.Put(phi, clone);
      }
    } else {
      temp_mapping.Put(phi, in_loop_input);
    }
  }
  // Now that temporary mapping is done, we go through the list again to update the final
  // mapping that will be used for the loop instructions.
  for (HInstructionIterator it(loop_header->GetPhis()); !it.Done(); it.Advance()) {
    HInstruction* phi = it.Current();
    if (phi_to_loop_inputs_.find(phi)!=phi_to_loop_inputs_.end()) {

      // At this point, we should have at least one mapping per loop header phi.
      DCHECK(temp_mapping.find(phi) != temp_mapping.end()) << phi;
      HInstruction* map_insn = temp_mapping.Get(phi);

      // We map the original phi node to the latest clone.
      phi_to_loop_inputs_.Overwrite(phi, map_insn);
    }
  }
}

void HLoopPartialUnrolling::IncrementConstantForUnrolling(HInstruction* toAdd, uint64_t iteration){
 
  for (size_t i=0,e=toAdd->InputCount(); i<e ; ++i) {
    HInstruction* input = toAdd->InputAt(i);
    if (input->IsConstant()) {
      Primitive::Type input_type = input->GetType();
      HInstruction * new_iv_increment = nullptr;
      switch (input_type) {
        case Primitive::Type::kPrimBoolean:
        case Primitive::Type::kPrimChar:
        case Primitive::Type::kPrimShort:
        case Primitive::Type::kPrimByte:
        case Primitive::Type::kPrimInt:
        {
          auto value = input->AsIntConstant()->GetValue();
          new_iv_increment = loop_->GetGraph()->GetConstant(input->GetType(), static_cast<int32_t>(value*iteration+value));
          break;
        }
        case Primitive::Type::kPrimLong:
        {
          auto value = input->AsLongConstant()->GetValue();
          new_iv_increment = loop_->GetGraph()->GetConstant(input->GetType(),
                                                static_cast<int64_t>(value*iteration+value));
          break;
        }
        case Primitive::Type::kPrimFloat:
        {
          auto value = input->AsFloatConstant()->GetValue();
          new_iv_increment = loop_->GetGraph()->GetFloatConstant(static_cast<float>(static_cast<float>(iteration))*value+value);
          break;
        }
        case Primitive::Type::kPrimDouble:
        {
          auto value = input->AsDoubleConstant()->GetValue();
          new_iv_increment = loop_->GetGraph()->GetDoubleConstant(static_cast<double>(static_cast<double>(iteration)*value + value));
          break;
        }
        default:
          LOG(FATAL) << "Unsupported constant type";
          UNREACHABLE();
        }
      DCHECK(new_iv_increment!=nullptr);
      toAdd->ReplaceInput(new_iv_increment,i);
    }
  }

}

void HLoopPartialUnrolling::AddClonedInstructions(HInstructionCloner& cloner, HBasicBlock* copy_bb, uint64_t iteration) {

  bool is_header= false;
  HInductionVariable* iv = loop_->GetBasicIV();
  HInstruction* linear_op = iv->GetLinearInsn();
 
  if (copy_bb == loop_->GetHeader())
    is_header = true;
 
  HInstruction* last_ins = nullptr;
  for (HInstruction* instruction : loop_instructions_) {

  if (IsUselessInstruction(instruction) || instruction->IsGoto()) {
    continue;
  } else {
    HInstruction* toAdd = cloner.GetClone(instruction);
    if (toAdd != nullptr && toAdd->GetBlock() == nullptr) {
       
      if (is_header) {
        last_ins = copy_bb->GetLastInstruction()->GetPrevious();
      } else {
        last_ins = copy_bb->GetLastInstruction();
      }
      DCHECK(last_ins != nullptr);
      copy_bb->InsertInstructionBefore(toAdd, last_ins);

      //Increment constant of linear operation 
      if (instruction->Equals(linear_op)) {
        IncrementConstantForUnrolling(toAdd,iteration);
       } else {
        //Correct in loop inputs for newly added clones
        //This needs to be done because unrolled body should not recieve
        //out of loop inputs
        for (size_t i = 0, e = instruction->InputCount(); i < e; ++i) {
          HInstruction* input = instruction->InputAt(i);
        
          if (input->IsPhi() && input->GetBlock()== loop_->GetHeader()) {
            if (phi_to_loop_inputs_.find(input)!=phi_to_loop_inputs_.end()){
              HInstruction* replacement = phi_to_loop_inputs_.Get(input);
              toAdd->ReplaceInput(replacement,i);
            } 
          }
        }
      }
      //Update cloner map to have the new input now 
      cloner.AddOrUpdateCloneManually(instruction,toAdd);
   }
  }
 }
}

void HLoopPartialUnrolling::GetInLoopInstructions() {

  HBasicBlock* loop_body = loop_->GetBackEdges()[0];
  if (loop_body->IsSingleGoto()) {
    HBasicBlock* header = loop_->GetHeader();
    for (HInstructionIterator it(header->GetInstructions());!it.Done();it.Advance()) {
      HInstruction* insn = it.Current();
      if (insn->IsPhi() || IsUselessInstruction(insn) 
          ||insn->IsGoto() || IsLoopConditionInstruction(insn)) {
        continue;
      } else {
       loop_instructions_.push_back(insn);
      }  
    }
  } else {
    for (HInstructionIterator it(loop_body->GetInstructions());!it.Done();it.Advance()) {
      HInstruction* insn = it.Current();
      if (IsUselessInstruction(insn)|| insn->IsGoto()) {
        continue;
      } else {
        loop_instructions_.push_back(insn);
     }
   }
  }
}

void HLoopPartialUnrolling::DumpInstruction(HInstruction* ins) {

  std::cout << ins->GetId() <<"::"<<ins->DebugName() << "block->"<< ins->GetBlock()->GetBlockId() << std::endl;
}

void HLoopPartialUnrolling::FixLoopHeaderPhis() {

   HBasicBlock* loop_header = loop_->GetHeader();
   for (HInstructionIterator it(loop_header->GetPhis());!it.Done();it.Advance()) {
     HInstruction* instruction = it.Current();
     HPhi* phi = instruction->AsPhi();
     DCHECK(phi!=nullptr);
     DCHECK(phi->IsLoopHeaderPhi());
     DCHECK_EQ(phi->InputCount(), 2u);
      
     //Update header phis with appropriate in loop inputs 
     if (phi_to_loop_inputs_.find(phi)!=phi_to_loop_inputs_.end()) {
       HInstruction* last_replacement = phi_to_loop_inputs_.Get(phi);
       DCHECK(last_replacement!=nullptr);
       phi->ReplaceInput(last_replacement,1);
     }
   } 
}
  
void HLoopPartialUnrolling::FixLoopUsers(HInstructionCloner& cloner) {

  HBasicBlock* loop_body = loop_->GetBackEdges()[0];
  //Modify input of comparision instruction
  HInstruction* contorl_flow = loop_->GetExitBlock()->GetPredecessors()[0]->GetLastInstruction();
  HInstruction* cmp = contorl_flow->GetPrevious();
  for (size_t i=0, e = cmp->InputCount(); i<e; ++i) {
    HInstruction* input = cmp->InputAt(i);
    HInstruction* new_input = cloner.GetClone(input);

    //clone can be null when the loop is top tested.
    if (new_input!=nullptr) {
      cmp->ReplaceInput(new_input,i);
    }
    if (loop_body->IsSingleGoto()) {
      if (phi_to_loop_inputs_.find(input) != phi_to_loop_inputs_.end())
        cmp->ReplaceInput(phi_to_loop_inputs_.Get(input),i);
      }
    }

  for (HInstruction* instruction : loop_instructions_) {
     for (HAllUseIterator use_it(instruction); !use_it.Done(); use_it.Advance()) {
       HInstruction* user = use_it.Current();

       // We do not want to replace uses inside the loop.
       if (!loop_->Contains(*(user->GetBlock()))) {
         HInstruction* replacement = ins_to_use_.Get(instruction);
         DCHECK(replacement != nullptr);
         use_it.ReplaceInput(replacement);
       }
     }
  }

  //Top Tested Loops recieve corerct in_loop_inputs. So we update
  //phi users outside loop only for a loop whose body is empty
  if (loop_body->IsSingleGoto()) {

  //Mapping to store users and their indexes updated.We should
  //update users only once and not recursively. This temp mapping 
  //takes care of that.
   SafeMap<int64_t,std::unordered_set<size_t>>temp_mapping;

   for (HInstructionIterator it(loop_->GetHeader()->GetPhis());!it.Done();it.Advance()) {
     HInstruction* instruction = it.Current();
     const HUseList<HInstruction*>& uses = instruction->GetUses();
     for (auto use_it = uses.begin(), end = uses.end(); use_it != end; /* ++it below */) {
       HInstruction* user = use_it->GetUser();
       size_t index = use_it->GetIndex();
       // Increment `it` now because `*it` may disappear thanks to user->ReplaceInput().
       ++use_it;
       if (!loop_->Contains(*(user->GetBlock()))) {
         if (temp_mapping.find(user->GetId())!=temp_mapping.end()) {
           std::unordered_set<size_t>index_set = temp_mapping.Get(user->GetId());
           if (index_set.find(index)==index_set.end()) {
             temp_mapping.Get(user->GetId()).insert(index);      
             HInstruction* replacement = ins_to_use_.Get(instruction);
             DCHECK(replacement != nullptr);
             user->ReplaceInput(replacement, index);
           }
         } else {
           //insert pair into temp_mapping 
           std::unordered_set<size_t>index_set;
           index_set.insert(index);
           temp_mapping.Put(user->GetId(),index_set);
           HInstruction* replacement = ins_to_use_.Get(instruction);
           DCHECK(replacement != nullptr);
           user->ReplaceInput(replacement,index);
         } 
       }
     }
   }
  }
} 
  
void HLoopPartialUnrolling::ListLoopConditionInstructions() {
  // Last instruction should be the loop control flow, and we do not need it.
  HInstruction* control_flow = loop_->GetExitBlock()->GetPredecessors()[0]->GetLastInstruction();
  loop_condition_instructions_.insert(control_flow);
  for (HInputIterator inputs(control_flow); !inputs.Done(); inputs.Advance()) {
    HInstruction* input = inputs.Current();
    // If control_flow is the only use of its input, then we consider it useless.
    if (input->HasOnlyOneNonEnvironmentUse()) {
      loop_condition_instructions_.insert(input);
    }
  }
}

bool HLoopPartialUnrolling::IsLoopConditionInstruction(HInstruction* insn)  {
  return loop_condition_instructions_.find(insn) != loop_condition_instructions_.end();
}

bool HLoopPartialUnrolling::IsSimpleLoop(){

  HBasicBlock* body = loop_->GetBackEdges()[0];

  // Current implementation does not handle below case

  /*while(true){
  if (condition)
   ---
   ---
   break;
   ---
   ---
  } */

  HBasicBlock* loop_header = loop_->GetHeader();
  HInstruction* first_ins = loop_header->GetFirstInstruction();
  if (!body->IsSingleGoto()) {
    if (first_ins->IsSuspendCheck()) {
      HInstruction* next = first_ins->GetNext();
      if (!IsLoopConditionInstruction(next)) {
        return false;
      }
    } else {
     if (!IsLoopConditionInstruction(first_ins))
       return false;
    }
  }
  return true; 
}

bool HLoopPartialUnrolling::UnrollBody(uint64_t num_iterations, uint64_t unrolling_factor) {
  
  if(!IsSimpleLoop()){
   return false;
  }  
 
  if (!loop_->IsBottomTested()) {
    DCHECK_EQ((num_iterations-1)%unrolling_factor,0u);
  } else {
    DCHECK_EQ(num_iterations%unrolling_factor,0u);
  }

  //Maps in_loop_inputs for loop body instructions
  MapLoopPhiInputs();

  for (uint64_t iteration=0; iteration<unrolling_factor-1; iteration++) {
    CloneInstructionsForUnroll(cloner_);

    DCHECK(cloner_.AllOkay()) << "Cloner failed to copy loop body.";

    //Add the instructions in inplace, i,e in loop body only
    HBasicBlock* loop_body = loop_->GetBackEdges()[0];
  
    if (loop_body->IsSingleGoto()) {
     //we already have i+1 in the loop body
     AddClonedInstructions(cloner_,loop_->GetHeader(),iteration+1);
    } else {
      AddClonedInstructions(cloner_, loop_body,iteration+1);
    }

    //Update in_loop_inputs for loop header phis with newly added instructions
    UpdateLoopPhiInputsMap(cloner_);
  }
 
  //Fix of loop header phis to use instructions of the last iteration.
  FixLoopHeaderPhis();
 
  //Fix users of loop instructions
  FixLoopUsers(cloner_);

  return true;
}

bool HLoopPartialUnrolling::PartialUnroll() {

  uint64_t num_iterations = loop_->GetNumIterations(loop_->GetHeader());

  // Unroll the loop body.
  if (!UnrollBody(num_iterations,2)) {
    return false;
  }

  return true;
}

bool HLoopPartialUnrolling::IsLoopEmpty() {

  if (loop_instructions_.size() <= 1u) {
    HBasicBlock* header = loop_->GetHeader();
    int phi_count = 0;
    for (HInstructionIterator it(header->GetPhis());!it.Done(); it.Advance()) {
      phi_count++;
    }
    if (phi_count == 1) {
      return true;
    }
  }
  return false;
}

bool HLoopPartialUnrolling::Gate(uint64_t max_unrolled_instructions, uint64_t unroll_factor) {

  if (loop_ == nullptr) {
    PRINT_PASS_OSTREAM_MESSAGE(optim_, "Loop is nullptr");
    return false;
  }

  if (loop_->IsOrHasIrreducibleLoop()) {
    PRINT_PASS_OSTREAM_MESSAGE(optim_, "Unrolling failed because the loop is irreducible.");
    return false;
  }
  if (loop_->HasTryCatchHandler()) {
    PRINT_PASS_OSTREAM_MESSAGE(optim_, "Found a try or catch handler inside the loop.");
    return false;
  }
  if (!loop_->HasKnownNumIterations()) {
    PRINT_PASS_OSTREAM_MESSAGE(optim_, "Loop must have a known number of iterations.");
    return false;
  }

  uint64_t num_iterations = loop_->GetNumIterations(loop_->GetHeader());

  if ((loop_->IsBottomTested() && num_iterations < 2u) ||
      (!loop_->IsBottomTested() && num_iterations < 3u)) {
    PRINT_PASS_OSTREAM_MESSAGE(optim_, "Loop has one or less iterations: "
      "loop unrolling is useless.");
    return false;
  }
  if (!loop_->IsBottomTested() && (num_iterations-1)%unroll_factor != 0) {
   //TODO: Remove this limitation later
   PRINT_PASS_OSTREAM_MESSAGE(optim_,"Loop iteration count is not a factor of unroll factor");
   return false;
  }

  if (loop_->IsBottomTested() && num_iterations%unroll_factor != 0 ) {
   PRINT_PASS_OSTREAM_MESSAGE(optim_, "Loop iteration count is not a factor of unroll factor");
   return false;
  }

  if (!loop_->HasOneExitEdge()) {
    PRINT_PASS_OSTREAM_MESSAGE(optim_, "Loop must have one exit edge.");
    return false;
  }

  if (loop_->GetBackEdges().size() > 1u) {
    PRINT_PASS_OSTREAM_MESSAGE(optim_, "Loop must have one back edge.");
    return false;
  }

  // TODO: As future work, we can remove this limitation.
  if (loop_->NumberOfBlocks() > 2) {
    PRINT_PASS_OSTREAM_MESSAGE(optim_, "Loop must have less than three basic blocks.");
    return false;
  }
  
  uint64_t nb_instructions = loop_->CountInstructionsInBody(true);
  uint64_t nb_unrolled_instructions = unroll_factor * nb_instructions;
  if (nb_unrolled_instructions > max_unrolled_instructions) {
    PRINT_PASS_OSTREAM_MESSAGE(optim_, "Number of unrolled instructions ("
      << nb_unrolled_instructions << ") is too large (max: " << max_unrolled_instructions << ")");
    return false;
  }

   ListLoopConditionInstructions();
   GetInLoopInstructions();
   if (IsLoopEmpty()) {
     PRINT_PASS_OSTREAM_MESSAGE(optim_,"Loop Body has only one linear operation instruction."
       "Unrolling is useless" );
   }

  // Verify that the instructions can be all cloned by the instruction cloner.
  // Return false if it was not successful.
  {
    HInstructionCloner instruction_verifier(graph_, false);
       CloneInstructionsForUnroll(instruction_verifier);
    if (!instruction_verifier.AllOkay()) {
      PRINT_PASS_OSTREAM_MESSAGE(optim_, "The loop body cannot be copied entirely."
        " because of instruction: " << instruction_verifier.GetDebugNameForFailedClone());
      return false;
    }
  }

  return true;
}

}  // namespace art
