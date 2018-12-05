/* Copyright (C) 2018 The Android Open Source Project
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

#include "instruction_simplifier_x86_shared.h"
#include "nodes_x86.h"

namespace art {

bool TryCombineAndNot(HNot* instruction) {
  DataType::Type type = instruction->GetType();
  if ( !DataType::IsIntOrLongType(type)) {
    return false;
  }
  // Replace code looking like
  // Not tmp, x 
  // And dst, x, tmp with
  // AndNot dst, x
   ArenaAllocator* arena = instruction->GetBlock()->GetGraph()->GetAllocator(); 
   // Only do the simplification if instruction has only one use 
   // and thus can be safely removed.
   if (instruction->HasOnlyOneNonEnvironmentUse()) {
     HInstruction* use = instruction->GetUses().front().GetUser();
     HAnd* and_ins = use->AsAnd();
     HInstruction* left = and_ins->GetLeft();
     HInstruction* right = and_ins->GetRight();
     // Perform simplication only when either left or right
     // is Not. When both are Not, instruction should be simplified with 
     // DeMorgan's Laws.
     if (left->IsNot() ^ right->IsNot()) {
       HInstruction* other_ins = (left->IsNot() ? right : left);
       HAndNot* and_not = new (arena) HAndNot(type, instruction->GetInput(), other_ins, instruction->GetDexPc());
       and_ins->GetBlock()->ReplaceAndRemoveInstructionWith(and_ins, and_not);
       DCHECK(!instruction->HasUses());
       instruction->GetBlock()->RemoveInstruction(instruction);
       return true;
     }
   }
   return false;
}

bool TryCombineAndNeg(HNeg* instruction) {
  DataType::Type type = instruction->GetType();
  if ( !DataType::IsIntOrLongType(type)) {
    return false;
  }
  // Replace code looking like
  // Neg tmp, x 
  // And dst, x, tmp with
  // AndNeg dst, x
  ArenaAllocator* arena = instruction->GetBlock()->GetGraph()->GetAllocator();
  HInstruction* input = instruction->GetInput();
  // Only do the simplification if instruction has only one use
  // and thus can be safely removed.
  if (instruction->HasOnlyOneNonEnvironmentUse()) {
    HInstruction* use = instruction->GetUses().front().GetUser();
    if (use->IsAnd()) {
      HAnd* and_ins = use->AsAnd();
      HInstruction*  left = and_ins->GetLeft();
      HInstruction* right = and_ins->GetRight();
      // Perform simplication only when either left or right
      // is Neg. When both are Neg, instruction should be simplified with
      // DeMorgan's Laws.
      if (left->IsNeg() ^ right->IsNeg()) {
        HInstruction* other_ins = (left->IsNeg() ? right : left);
        if (input == other_ins) {
          HAndNeg* and_neg = new (arena) HAndNeg(type, instruction->GetInput(), instruction->GetDexPc());
          and_ins->GetBlock()->ReplaceAndRemoveInstructionWith(and_ins, and_neg);
          DCHECK(!instruction->HasUses());
          instruction->GetBlock()->RemoveInstruction(instruction);
          return true;
        }
      }
    }
  }
  return false;
}

bool TryGenerateBitwiseAddRight(HAdd* instruction) {
  DataType::Type type = instruction->GetType();
  if ( !DataType::IsIntOrLongType(type)) {
    return false;
  }
  
  // Replace code looking like 
  // Add              tmp, src, -1
  // Xor/And          dst, x , tmp with 
  // BitwiseAddRight  dst, x, src
  ArenaAllocator* arena = instruction->GetBlock()->GetGraph()->GetAllocator();
  HConstant* cst_input = instruction->GetConstantRight();
  HInstruction* other_input = instruction->GetLeastConstantLeft();
  if (cst_input == nullptr ) {
     return false;
  }
  if (cst_input->IsMinusOne() && instruction->HasOnlyOneNonEnvironmentUse()) {
    HInstruction* use = instruction->GetUses().front().GetUser();
    if (use->IsAnd()|| use->IsXor()) {
      HInstruction* left = use->AsBinaryOperation()->GetLeft();
      HInstruction* right = use->AsBinaryOperation()->GetRight();
      if (left == other_input || right == other_input) {
        HInstruction::InstructionKind kind = use->IsAnd() ? HInstruction::kAnd : HInstruction::kXor;
        HBitwiseAddRight* add_right = new (arena) HBitwiseAddRight(type, kind, other_input, instruction->GetDexPc());
        use->GetBlock()->ReplaceAndRemoveInstructionWith(use, add_right);
        DCHECK(!instruction->HasUses());
        instruction->GetBlock()->RemoveInstruction(instruction);
        return true;
      }
    }
  }
  return false;
}

}  // namespace art
