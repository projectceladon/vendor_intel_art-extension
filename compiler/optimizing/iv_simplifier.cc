/*
Copyright (c) 2018 Corporation
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
    * this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright notice,
    * this list of conditions and the following disclaimer in the documentation
    * and/or other materials provided with the distribution.

    * Neither the name of Intel Corporation nor the names of its contributors
    * may be used to endorse or promote products derived from this software
    * without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "iv_simplifier.h"
#include<tuple>

namespace art{


typedef std::tuple<HInstruction*,HConstant*, HConstant* > triple;
//
// public methods
//
InductionVarSimplification::InductionVarSimplification(HInductionVarAnalysis* induction_analysis)
    : induction_analysis_(induction_analysis),
      biv_increment_(nullptr) {
  DCHECK(induction_analysis != nullptr);
}

/*Helper Functions*/
static HConstant* GetNewConstant(HInstruction::InstructionKind kind, HConstant* cst1, HConstant* cst2) {
   DataType::Type type = cst1->GetType();
   HConstant* new_cst = nullptr;
   HGraph* graph = cst1->GetBlock()->GetGraph();
   if (DataType::IsIntegralType(type)) {
     int64_t new_val = 0;
     switch (kind) {
      case HInstruction::kAdd :
      new_val = Int64FromConstant(cst1) + Int64FromConstant(cst2);
      break;
      case HInstruction::kSub: 
      new_val = Int64FromConstant(cst1) - Int64FromConstant(cst2);
      break;
      case HInstruction::kMul:
       new_val = Int64FromConstant(cst1) * Int64FromConstant(cst2);
       break;
      default :
       UNREACHABLE();
     }
      new_cst = graph->GetConstant(type, new_val);
      return new_cst;
    } else if (cst1->IsDoubleConstant()) {
      double new_val =0.0;
      switch (kind){
        case HInstruction::kAdd:
        new_val = cst1->AsDoubleConstant()->GetValue() + cst2->AsDoubleConstant()->GetValue();
        break;
        case HInstruction::kSub:
        new_val = cst1->AsDoubleConstant()->GetValue() - (cst2->AsDoubleConstant()->GetValue());
        break;
        case HInstruction::kMul:
        new_val = cst1->AsDoubleConstant()->GetValue() * cst2->AsDoubleConstant()->GetValue();
        break;
        default :
          UNREACHABLE();
      }
      new_cst = graph->GetDoubleConstant(new_val);
      return new_cst;
    } else if (cst1->IsFloatConstant()) {
      float new_val = 0.0;
      switch (kind) {
       case HInstruction::kAdd:
        new_val = cst1->AsFloatConstant()->GetValue() + cst2->AsFloatConstant()->GetValue();
        break;
       case HInstruction::kSub:
        new_val = cst1->AsFloatConstant()->GetValue() - cst2->AsFloatConstant()->GetValue();
        break;
       case HInstruction::kMul:
        new_val = cst1->AsFloatConstant()->GetValue() * cst2->AsFloatConstant()->GetValue();
        break;
        default:
          UNREACHABLE();
      }
      new_cst = graph->GetFloatConstant( new_val);
      return new_cst;
    }
    return nullptr;
}
static HConstant* GetConstant(HGraph* graph, DataType::Type type, int64_t value){
   HConstant * cst = nullptr;
   if (DataType::IsIntegralType(type)) {
      cst = graph->GetConstant(type, value);
   } else if (type == DataType::Type::kFloat32) {
     float val = static_cast<float>(value);
     cst = graph->GetFloatConstant(val);
   } else if (type == DataType::Type::kFloat64) {
     double val = static_cast<double>(value);
     cst = graph->GetDoubleConstant(val);
   }
   return cst;
}

static HConstant* GetNegativeConstant(HGraph* graph, DataType::Type type, HConstant* input) {
  HConstant* cst = nullptr;
  if (DataType::IsIntegralType(type)) {
        int64_t val = Int64FromConstant(input);
        cst = GetConstant(graph, type, -1*val);
  } else if (type ==DataType::Type::kFloat32){
    float val = input->AsFloatConstant()->GetValue();
    cst = GetConstant(graph, type, -1.0f*val);
  } else if (type == DataType::Type::kFloat64){
     double val = input->AsDoubleConstant()->GetValue();
     cst = GetConstant(graph, type, -1.0f*val);
  }
  return cst;
}

HInstruction* GetOutOfLoopInput(HLoopInformation* loop, HPhi* phi) {
 HInputsRef inputs = phi->GetInputs();
 for (size_t i = 0; i < inputs.size(); ++i) {
   HInstruction* input = inputs[i];
   if (input->GetBlock()->GetLoopInformation() != loop) {
    return input;
   }
 }
 return nullptr;
}

bool InductionVarSimplification::IsCandidatePhi(HLoopInformation* loop, HPhi* phi) {
  HInputsRef inputs = phi->GetInputs();
  if (inputs.size() != 2 )
    return false;
  HInstruction* control = loop->GetHeader()->GetLastInstruction();
  HInductionVarAnalysis::InductionInfo* phi_info = induction_analysis_->LookupInfo(loop, phi);
  if (control->IsIf()) {
    HIf* ifs = control->AsIf();
    HInstruction* if_expr = ifs->InputAt(0);
    // Determine if loop has following structure in header.
    // loop-header: ....
    //              if (condition) goto X
    if (if_expr->IsCondition()) {
      HCondition* condition = if_expr->AsCondition();
      HInductionVarAnalysis::InductionInfo* a = induction_analysis_->LookupInfo(loop, condition->InputAt(0));
      HInductionVarAnalysis::InductionInfo* b = induction_analysis_->LookupInfo(loop, condition->InputAt(1));
      if (HInductionVarAnalysis::InductionEqual(a,phi_info) || HInductionVarAnalysis::InductionEqual(b, phi_info)) {
       // check if it is incremented by a constant amount 
       ArenaSet<HInstruction*>* set = induction_analysis_->LookupCycle(phi);
       for (HInstruction* ins : *set) {
          if (ins->IsAdd()) { 
            HBinaryOperation* op = ins->AsAdd();
            HConstant* cst = op->GetConstantRight();
            if (cst != nullptr) {
               biv_increment_=op;
              return true;
            }
        }
      }
      }
    }
  }
  return false;
}

void InductionVarSimplification::PerformReduction(HLoopInformation* loop, HInstruction* derived_var,
                  std::tuple<HInstruction*, HConstant*, HConstant*> val) {
    DCHECK(biv_increment_ != nullptr);
    HInstruction* var = std::get<0>(val);
    HPhi* phi = var->AsPhi();
    DCHECK(phi != nullptr);
    HInstruction* input = GetOutOfLoopInput(loop, phi);
    // Create two new instructions mul and add
    ArenaAllocator* allocator = derived_var->GetBlock()->GetGraph()->GetAllocator();
    HInstruction* new_mul =  new (allocator) HMul(derived_var->GetType(), input, std::get<1>(val), derived_var->GetDexPc());
    loop->GetPreHeader()->InsertInstructionBefore(new_mul, loop->GetPreHeader()->GetLastInstruction());
    HInstruction* new_add = new (allocator) HAdd(new_mul->GetType(), new_mul,std::get<2>(val), derived_var->GetDexPc());
    loop->GetPreHeader()->InsertInstructionBefore(new_add, loop->GetPreHeader()->GetLastInstruction());
    HPhi* new_phi = new (allocator) HPhi(allocator, kNoRegNumber, 0, phi->GetType());
    phi->GetBlock()->InsertPhiAfter(new_phi, phi);
    new_phi->AddInput(new_add);
    HConstant* biv_inc_cst = biv_increment_->GetConstantRight();
    // Ensured in IsCandidatePhi()
    CHECK(biv_inc_cst != nullptr);
    HConstant* biv_cst = GetNewConstant(HInstruction::InstructionKind::kMul, biv_inc_cst, std::get<1>(val));
    CHECK(biv_cst != nullptr);
    HAdd* biv_add = new (allocator) HAdd(biv_inc_cst->GetType(), new_phi, biv_cst);
    biv_increment_->GetBlock()->InsertInstructionAfter(biv_add, biv_increment_);
    new_phi->AddInput(biv_add);
    // Replace uses of new add within the loop with new_phi
    const HUseList<HInstruction*>& uses = derived_var->GetUses();
    for (auto it = uses.begin(), end = uses.end(); it != end;) {
      HInstruction* user = it->GetUser();
      size_t input_index = it->GetIndex();
      ++it;
      user->ReplaceInput(new_phi, input_index);
    }
    const HUseList<HEnvironment*>& env_uses = derived_var->GetEnvUses();
    for (auto it = env_uses.begin(), end = env_uses.end(); it != end;) {
      HEnvironment* user = it->GetUser();
      size_t index = it->GetIndex();
      ++it;  // increment prior to potential removal
      user->RemoveAsUserOfInput(index);
      user->SetRawEnvAt(index, new_phi);
      new_phi->AddEnvUseAt(user, index);
    } 
    DCHECK(!derived_var->HasUses());
}

bool InductionVarSimplification::IsCandidateForReduction (
       HLoopInformation* loop, HPhi* biv, HBinaryOperation* to_check,
       std::map<HBinaryOperation*, triple>* candidate) {

  HInductionVarAnalysis::InductionInfo* ind_info = induction_analysis_->LookupInfo(loop, to_check);
  HInductionVarAnalysis::InductionInfo* biv_ind_info = induction_analysis_->LookupInfo(loop, biv);
  if (ind_info == nullptr)
    return false;
  // TODO: Handle Add and Shl is pending
  DataType::Type type = to_check->GetType();
  HGraph* graph = to_check->GetBlock()->GetGraph();
  if (to_check->IsMul()) {
    HConstant* cst = to_check->GetConstantRight();
    if (cst == nullptr)
      return false;
    HInstruction* left_ins = to_check->InputAt(0);
    HInstruction* right_ins = to_check->InputAt(1);
    HInductionVarAnalysis::InductionInfo* left_ins_info = induction_analysis_->LookupInfo(loop, left_ins);
    HInductionVarAnalysis::InductionInfo* right_ins_info = induction_analysis_->LookupInfo(loop, right_ins);
    if (HInductionVarAnalysis::InductionEqual(biv_ind_info, left_ins_info)){
      if ( right_ins_info != nullptr && right_ins_info->induction_class == HInductionVarAnalysis::kInvariant) {
        HConstant * zero = GetConstant(graph, type, 0);
        candidate->insert(std::pair<HBinaryOperation*, triple>(to_check, std::make_tuple(biv, cst, zero)));
        return true;
      }
    }
    if (HInductionVarAnalysis::InductionEqual(biv_ind_info, right_ins_info) &&
         left_ins_info != nullptr && left_ins_info->induction_class == HInductionVarAnalysis::kInvariant) {
      HConstant* zero = GetConstant(graph, type, 0);
      candidate->insert(std::pair<HBinaryOperation*, triple>(to_check, std::make_tuple(biv, cst, zero)));
      return true;
    }
  } else if (to_check ->IsAdd() || to_check->IsSub()) {
    HInstruction* left = to_check->InputAt(0);
    HInstruction* right = to_check->InputAt(1);
    HConstant* cst = to_check->GetConstantRight();
    if (cst == nullptr)
     return false;
    if (to_check->IsSub()) {
     cst = GetNegativeConstant(graph, type, cst);
    }
    HInductionVarAnalysis::InductionInfo* left_ins_info = induction_analysis_->LookupInfo(loop,left);
    HInductionVarAnalysis::InductionInfo* right_ins_info = induction_analysis_->LookupInfo(loop, right);
    // check for a derived  induction variable
    if (!HInductionVarAnalysis::InductionEqual(left_ins_info, biv_ind_info) &&
        !HInductionVarAnalysis::InductionEqual(right_ins_info, biv_ind_info)) {
      bool found = false;
      for (auto it : *candidate) {
        HInductionVarAnalysis::InductionInfo* info = induction_analysis_->LookupInfo(loop, it.first);
        if ( right_ins_info != nullptr && HInductionVarAnalysis::InductionEqual(left_ins_info, info) &&
            right_ins_info->induction_class == HInductionVarAnalysis::kInvariant) {
          HConstant * one = GetConstant(graph, type, 1);
          candidate->insert(std::pair<HBinaryOperation*, triple>(to_check, std::make_tuple( it.first,one, cst)));
          found = true;
          break;
        } else if ( left_ins_info != nullptr && HInductionVarAnalysis::InductionEqual(right_ins_info, info) && 
                   left_ins_info->induction_class == HInductionVarAnalysis::kInvariant) {
          HConstant* one = GetConstant(graph, type, 1);
          candidate->insert(std::pair<HBinaryOperation*, triple>(to_check, std::make_tuple(it.first, one, cst)));
          found = true;
          break;
        }
      }
      if (found == true)
        return true;
    }
  }
  return false;
}

bool InductionVarSimplification::SimplifyLoop(HLoopInformation* loop) {
  bool did_reduction = false;
  HBasicBlock* header = loop->GetHeader();
  /*HGraph* gr = header->GetGraph();
  std::string cur_name = gr->GetDexFile().PrettyMethod(gr->GetMethodIdx());
  std::cout << "method="<< cur_name <<std::endl;*/
  std::map<HBinaryOperation*, std::tuple< HInstruction*,HConstant*, HConstant*>> candidate;
  for (HInstructionIterator it(header->GetPhis()); !it.Done(); it.Advance()) {
    HInstruction* cur = it.Current();
    HPhi* phi = cur->AsPhi();
    if (IsCandidatePhi(loop, phi)) {
      for (HBlocksInLoopIterator it1(*loop); !it1.Done(); it1.Advance()) {
        HBasicBlock* block = it1.Current();
        for (HInstructionIterator it2(block->GetInstructions()); !it2.Done(); it2.Advance()) {
          HInstruction* to_check = it2.Current();
          if (to_check->IsBinaryOperation())
           IsCandidateForReduction(loop, phi, to_check->AsBinaryOperation(), &candidate);
        }
      }
    for (auto itr = candidate.cbegin(); itr != candidate.cend(); itr++) {
      HBinaryOperation* derived_var = itr->first;
      HInstruction::InstructionKind kind = derived_var->GetKind();
      triple val = itr->second;
      HInstruction* var = std::get<0>(val);
      if (var->GetId() != phi->GetId()) {
        HBinaryOperation* var_bin = var->AsBinaryOperation();
       if (derived_var->IsAdd() || derived_var->IsSub()) {
         DCHECK(var->IsBinaryOperation());
         HConstant* new_cst = GetNewConstant(kind, std::get<2>(val), std::get<2>(candidate[var_bin]));
         DCHECK(new_cst != nullptr);
         std::get<0>(val) = std::get<0>(candidate[var_bin]);
         std::get<1>(val) = std::get<1>(candidate[var_bin]);
         std::get<2>(val) = new_cst;
         candidate[derived_var] = val;
       } else if(derived_var->IsMul()) {
         HConstant* new_cst1 = GetNewConstant(kind, std::get<2>(val), std::get<1>(candidate[var_bin]));
         HConstant* new_cst2 = GetNewConstant(kind, std::get<2>(val), std::get<2>(candidate[var_bin]));
         DCHECK(new_cst1 != nullptr);
         DCHECK(new_cst2 != nullptr);
         std::get<0>(val) = std::get<0>(candidate[var_bin]);
         std::get<1>(val) = new_cst1;
         std::get<2>(val) = new_cst2;
         candidate[derived_var] = val;
        }
    }
  }
  // Perform strength Reduction
  for (auto cur1 = candidate.begin(); cur1 != candidate.end(); cur1++ ) {
    PerformReduction(loop, cur1->first, cur1->second);
  }
  
 }
 // clear the candidate map now
 if (!did_reduction && !candidate.empty()) {
 did_reduction = true;
 candidate.clear();
 }
}
  return did_reduction;
}

} // namespace art
