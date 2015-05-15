/*
 * INTEL CONFIDENTIAL
 * Copyright (c) 2015, Intel Corporation All Rights Reserved.
 *
 * The source code contained or described herein and all documents related to the
 * source code ("Material") are owned by Intel Corporation or its suppliers or
 * licensors. Title to the Material remains with Intel Corporation or its suppliers
 * and licensors. The Material contains trade secrets and proprietary and
 * confidential information of Intel or its suppliers and licensors. The Material
 * is protected by worldwide copyright and trade secret laws and treaty provisions.
 * No part of the Material may be used, copied, reproduced, modified, published,
 * uploaded, posted, transmitted, distributed, or disclosed in any way without
 * Intel's prior express written permission.
 *
 * No license under any patent, copyright, trade secret or other intellectual
 * property right is granted to or conferred upon you by disclosure or delivery of
 * the Materials, either expressly, by implication, inducement, estoppel or
 * otherwise. Any license under such intellectual property rights must be express
 * and approved by Intel in writing.
 *
 */

#include "cloning.h"

namespace art {

bool HInstructionCloner::AllOkay() const {
  if (kIsDebugBuild && all_cloned_okay_) {
    for (auto it : orig_to_clone_) {
      DCHECK(it.first != nullptr);
      DCHECK(it.second != nullptr);
    }
  }

  return all_cloned_okay_;
}

void HInstructionCloner::GetInputsForUnary(HInstruction* instr, HInstruction** input_ptr) const {
  DCHECK(input_ptr != nullptr);
  DCHECK_EQ(instr->InputCount(), 1u);

  *input_ptr = instr->InputAt(0);

  if (use_cloned_inputs_) {
    // Look to see if this input has a clone.
    auto input_it = orig_to_clone_.find(*input_ptr);

    // If there is a clone, make it so the new inputs are from clone.
    if (input_it != orig_to_clone_.end()) {
      *input_ptr = input_it->second;
    }
  }
}

void HInstructionCloner::GetInputsForBinary(HInstruction* instr, HInstruction** lhs_ptr,
                                            HInstruction** rhs_ptr) const {
  DCHECK(lhs_ptr != nullptr);
  DCHECK(rhs_ptr != nullptr);
  DCHECK_EQ(instr->InputCount(), 2u);

  *lhs_ptr = instr->InputAt(0);
  *rhs_ptr = instr->InputAt(1);

  if (use_cloned_inputs_) {
    // Look to see if this input has a clone.
    auto lhs_it = orig_to_clone_.find(*lhs_ptr);
    auto rhs_it = orig_to_clone_.find(*rhs_ptr);

    // If there is a clone, make it so the new inputs are from clone.
    if (lhs_it != orig_to_clone_.end()) {
      *lhs_ptr = lhs_it->second;
    }
    if (rhs_it != orig_to_clone_.end()) {
      *rhs_ptr = rhs_it->second;
    }
  }
}

void HInstructionCloner::GetInputsForTernary(HInstruction* instr,
                                             HInstruction** input0_ptr,
                                             HInstruction** input1_ptr,
                                             HInstruction** input2_ptr) const {
  DCHECK(input0_ptr != nullptr);
  DCHECK(input1_ptr != nullptr);
  DCHECK(input2_ptr != nullptr);
  DCHECK_EQ(instr->InputCount(), 3u);

  *input0_ptr = instr->InputAt(0);
  *input1_ptr = instr->InputAt(1);
  *input2_ptr = instr->InputAt(2);

  if (use_cloned_inputs_) {
    // Look to see if this input has a clone.
    auto i0_it = orig_to_clone_.find(*input0_ptr);
    auto i1_it = orig_to_clone_.find(*input1_ptr);
    auto i2_it = orig_to_clone_.find(*input2_ptr);

    // If there is a clone, make it so the new inputs are from clone.
    if (i0_it != orig_to_clone_.end()) {
      *input0_ptr = i0_it->second;
    }
    if (i1_it != orig_to_clone_.end()) {
      *input1_ptr = i1_it->second;
    }
    if (i2_it != orig_to_clone_.end()) {
      *input2_ptr = i2_it->second;
    }
  }
}

HEnvironment* HInstructionCloner::CloneEnvironment(HEnvironment* env, HInstruction* clone) {
  DCHECK_EQ(cloning_enabled_, true);

  HEnvironment* env_clone_base = nullptr;
  HEnvironment* env_clone_parent = nullptr;

  while (env != nullptr) {
    HEnvironment* env_clone = new (arena_) HEnvironment(arena_, env->Size(),
        env->GetDexFile(), env->GetMethodIdx(), env->GetDexPc(),
        env->GetInvokeType(), clone);

    if (env_clone_parent != nullptr) {
      env_clone->SetParent(env_clone_parent);
    } else {
      // This case is hit only if this is the first environment being cloned.
      // This means that the newly created environment is the base.
      env_clone_base = env_clone;
    }
    // Now set the current clone as parent to use in next iteration.
    env_clone_parent = env_clone;

    for (size_t i = 0; i < env->Size(); i++) {
      HInstruction* instruction = env->GetInstructionAt(i);

      if (use_cloned_inputs_) {
        // Now check to see if this input was cloned.
        auto clone_it = orig_to_clone_.find(instruction);
        if (clone_it != orig_to_clone_.end()) {
          instruction = clone_it->second;
        }
      }

      // Now link the inputs to the environment.
      env_clone->SetRawEnvAt(i, instruction);
      if (instruction != nullptr) {
        instruction->AddEnvUseAt(env_clone, i, arena_);
      }
    }

    env = env->GetParent();
  }

  DCHECK(env_clone_base != nullptr);
  return env_clone_base;
}

void HInstructionCloner::CloneEnvironment(HInstruction* instr, HInstruction* clone) {
  if (instr->HasEnvironment()) {
    HEnvironment* env_clone = CloneEnvironment(instr->GetEnvironment(), clone);
    clone->SetRawEnvironment(env_clone);
  }
}

void HInstructionCloner::VisitAbove(HAbove* instr) {
  if (cloning_enabled_) {
    DCHECK(!instr->HasEnvironment());
    HInstruction* lhs, *rhs;
    GetInputsForBinary(instr, &lhs, &rhs);
    HAbove* clone = new (arena_) HAbove(lhs, rhs);
    orig_to_clone_.Put(instr, clone);
  }
}

void HInstructionCloner::VisitAboveOrEqual(HAboveOrEqual* instr) {
  if (cloning_enabled_) {
    DCHECK(!instr->HasEnvironment());
    HInstruction* lhs, *rhs;
    GetInputsForBinary(instr, &lhs, &rhs);
    HAboveOrEqual* clone = new (arena_) HAboveOrEqual(lhs, rhs);
    orig_to_clone_.Put(instr, clone);
  }
}

void HInstructionCloner::VisitAdd(HAdd* instr) {
  if (cloning_enabled_) {
    DCHECK(!instr->HasEnvironment());
    HInstruction* lhs, *rhs;
    GetInputsForBinary(instr, &lhs, &rhs);
    HAdd* clone = new (arena_) HAdd(instr->GetResultType(), lhs, rhs);
    orig_to_clone_.Put(instr, clone);
  }
}

void HInstructionCloner::VisitAnd(HAnd* instr) {
  if (cloning_enabled_) {
    DCHECK(!instr->HasEnvironment());
    HInstruction* lhs, *rhs;
    GetInputsForBinary(instr, &lhs, &rhs);
    HAnd* clone = new (arena_) HAnd(instr->GetResultType(), lhs, rhs);
    orig_to_clone_.Put(instr, clone);
  }
}

void HInstructionCloner::VisitArrayGet(HArrayGet* instr) {
  if (cloning_enabled_) {
    DCHECK(!instr->HasEnvironment());
    HInstruction* array, *index;
    GetInputsForBinary(instr, &array, &index);
    HArrayGet* clone = new (arena_) HArrayGet(array, index, instr->GetType(),
                                              instr->GetDexPc());
    orig_to_clone_.Put(instr, clone);
  }
}

void HInstructionCloner::VisitArrayLength(HArrayLength* instr) {
  if (cloning_enabled_) {
    DCHECK(!instr->HasEnvironment());
    HInstruction* input;
    GetInputsForUnary(instr, &input);
    HArrayLength* clone = new (arena_) HArrayLength(input, instr->GetDexPc());
    orig_to_clone_.Put(instr, clone);
  }
}

void HInstructionCloner::VisitArraySet(HArraySet* instr) {
  if (cloning_enabled_) {
    HInstruction* array, *index, *value;
    GetInputsForTernary(instr, &array, &index, &value);
    HArraySet* clone = new (arena_) HArraySet(array, index, value,
        instr->GetComponentType(), instr->GetDexPc());
    orig_to_clone_.Put(instr, clone);

    // Also clone the environment appropriately.
    CloneEnvironment(instr, clone);
  }
}

void HInstructionCloner::VisitBelow(HBelow* instr) {
  if (cloning_enabled_) {
    DCHECK(!instr->HasEnvironment());
    HInstruction* lhs, *rhs;
    GetInputsForBinary(instr, &lhs, &rhs);
    HBelow* clone = new (arena_) HBelow(lhs, rhs);
    orig_to_clone_.Put(instr, clone);
  }
}

void HInstructionCloner::VisitBelowOrEqual(HBelowOrEqual* instr) {
  if (cloning_enabled_) {
    DCHECK(!instr->HasEnvironment());
    HInstruction* lhs, *rhs;
    GetInputsForBinary(instr, &lhs, &rhs);
    HBelowOrEqual* clone = new (arena_) HBelowOrEqual(lhs, rhs);
    orig_to_clone_.Put(instr, clone);
  }
}

void HInstructionCloner::VisitBooleanNot(HBooleanNot* instr) {
  if (cloning_enabled_) {
    DCHECK(!instr->HasEnvironment());
    HInstruction* input;
    GetInputsForUnary(instr, &input);
    HBooleanNot* clone = new (arena_) HBooleanNot(input);
    orig_to_clone_.Put(instr, clone);
  }
}

void HInstructionCloner::VisitBoundsCheck(HBoundsCheck* instr) {
  if (cloning_enabled_) {
    HInstruction* index, *length;
    GetInputsForBinary(instr, &index, &length);
    HBoundsCheck* clone = new (arena_) HBoundsCheck(index, length, instr->GetDexPc());
    orig_to_clone_.Put(instr, clone);

    // Also clone the environment appropriately.
    CloneEnvironment(instr, clone);
  }
}

void HInstructionCloner::VisitBoundType(HBoundType* instr) {
  if (cloning_enabled_) {
    DCHECK(!instr->HasEnvironment());
    HInstruction* input;
    GetInputsForUnary(instr, &input);
    HBoundType* clone = new (arena_) HBoundType(input);
    clone->SetUpperBound(instr->GetUpperBound(), instr->GetUpperCanBeNull());
    orig_to_clone_.Put(instr, clone);
  }
}

void HInstructionCloner::VisitCheckCast(HCheckCast* instr) {
  if (cloning_enabled_) {
    HInstruction* object, *constant;
    GetInputsForBinary(instr, &object, &constant);
    DCHECK(constant->IsLoadClass());
    HCheckCast* clone = new (arena_) HCheckCast(object, constant->AsLoadClass(),
        instr->GetTypeCheckKind(), instr->GetDexPc());
    orig_to_clone_.Put(instr, clone);

    // Also clone the environment appropriately.
    CloneEnvironment(instr, clone);
  }
}

void HInstructionCloner::VisitClinitCheck(HClinitCheck* instr) {
  if (cloning_enabled_) {
    HInstruction* input;
    GetInputsForUnary(instr, &input);
    DCHECK_EQ(input->IsLoadClass(), true);
    HClinitCheck* clone = new (arena_) HClinitCheck(input->AsLoadClass(), instr->GetDexPc());
    orig_to_clone_.Put(instr, clone);

    // Also clone the environment appropriately.
    CloneEnvironment(instr, clone);
  }
}

void HInstructionCloner::VisitCompare(HCompare* instr) {
  if (cloning_enabled_) {
    DCHECK(!instr->HasEnvironment());
    HInstruction* lhs, *rhs;
    GetInputsForBinary(instr, &lhs, &rhs);
    DCHECK_EQ(lhs->GetType(), rhs->GetType());
    HCompare* clone = new (arena_) HCompare(lhs->GetType(), lhs, rhs, instr->GetBias(), instr->GetDexPc());
    orig_to_clone_.Put(instr, clone);
  }
}

void HInstructionCloner::VisitDiv(HDiv* instr) {
  if (cloning_enabled_) {
    DCHECK(!instr->HasEnvironment());
    HInstruction* lhs, *rhs;
    GetInputsForBinary(instr, &lhs, &rhs);
    HDiv* clone = new (arena_) HDiv(instr->GetResultType(), lhs, rhs, instr->GetDexPc());
    orig_to_clone_.Put(instr, clone);
  }
}

void HInstructionCloner::VisitDivZeroCheck(HDivZeroCheck* instr) {
  if (cloning_enabled_) {
    HInstruction* input;
    GetInputsForUnary(instr, &input);
    HDivZeroCheck* clone = new (arena_) HDivZeroCheck(input, instr->GetDexPc());
    orig_to_clone_.Put(instr, clone);

    // Also clone the environment appropriately.
    CloneEnvironment(instr, clone);
  }
}

void HInstructionCloner::VisitEqual(HEqual* instr) {
  if (cloning_enabled_) {
    DCHECK(!instr->HasEnvironment());
    HInstruction* lhs, *rhs;
    GetInputsForBinary(instr, &lhs, &rhs);
    HEqual* clone = new (arena_) HEqual(lhs, rhs);
    orig_to_clone_.Put(instr, clone);
  }
}

void HInstructionCloner::VisitGoto(HGoto* instr) {
  if (cloning_enabled_) {
    DCHECK(!instr->HasEnvironment());
    HGoto* clone = new (arena_) HGoto();
    orig_to_clone_.Put(instr, clone);
  }
}

void HInstructionCloner::VisitGreaterThan(HGreaterThan* instr) {
  if (cloning_enabled_) {
    DCHECK(!instr->HasEnvironment());
    HInstruction* lhs, *rhs;
    GetInputsForBinary(instr, &lhs, &rhs);
    HGreaterThan* clone = new (arena_) HGreaterThan(lhs, rhs);
    orig_to_clone_.Put(instr, clone);
  }
}

void HInstructionCloner::VisitGreaterThanOrEqual(HGreaterThanOrEqual* instr) {
  if (cloning_enabled_) {
    DCHECK(!instr->HasEnvironment());
    HInstruction* lhs, *rhs;
    GetInputsForBinary(instr, &lhs, &rhs);
    HGreaterThanOrEqual* clone = new (arena_) HGreaterThanOrEqual(lhs, rhs);
    orig_to_clone_.Put(instr, clone);
  }
}

void HInstructionCloner::VisitIf(HIf* instr) {
  if (cloning_enabled_) {
    DCHECK(!instr->HasEnvironment());
    HInstruction* input;
    GetInputsForUnary(instr, &input);
    HIf* clone = new (arena_) HIf(input);
    orig_to_clone_.Put(instr, clone);
  }
}

void HInstructionCloner::VisitInstanceFieldGet(HInstanceFieldGet* instr) {
  if (cloning_enabled_) {
    DCHECK(!instr->HasEnvironment());
    HInstruction* input;
    GetInputsForUnary(instr, &input);
    const FieldInfo& fi = instr->GetFieldInfo();
    HInstanceFieldGet* clone =
        new (arena_) HInstanceFieldGet(input,
                                       instr->GetFieldType(),
                                       instr->GetFieldOffset(),
                                       instr->IsVolatile(),
                                       fi.GetFieldIndex(),
                                       fi.GetDeclaringClassDefIndex(),
                                       fi.GetDexFile(),
                                       fi.GetDexCache(),
                                       instr->GetDexPc());
    orig_to_clone_.Put(instr, clone);
  }
}

void HInstructionCloner::VisitInstanceFieldSet(HInstanceFieldSet* instr) {
  if (cloning_enabled_) {
    DCHECK(!instr->HasEnvironment());
    HInstruction* object, *value;
    GetInputsForBinary(instr, &object, &value);
    const FieldInfo& fi = instr->GetFieldInfo();
    HInstanceFieldSet* clone =
        new (arena_) HInstanceFieldSet(object,
                                       value,
                                       instr->GetFieldType(),
                                       instr->GetFieldOffset(),
                                       instr->IsVolatile(),
                                       fi.GetFieldIndex(),
                                       fi.GetDeclaringClassDefIndex(),
                                       fi.GetDexFile(),
                                       fi.GetDexCache(),
                                       instr->GetDexPc());
    orig_to_clone_.Put(instr, clone);
  }
}

void HInstructionCloner::VisitInstanceOf(HInstanceOf* instr) {
  if (cloning_enabled_) {
    HInstruction* object, *constant;
    GetInputsForBinary(instr, &object, &constant);
    DCHECK(constant->IsLoadClass());
    HInstanceOf* clone = new (arena_) HInstanceOf(object, constant->AsLoadClass(),
        instr->GetTypeCheckKind(), instr->GetDexPc());
    orig_to_clone_.Put(instr, clone);

    // Also clone the environment appropriately.
    CloneEnvironment(instr, clone);
  }
}

void HInstructionCloner::VisitLessThan(HLessThan* instr) {
  if (cloning_enabled_) {
    DCHECK(!instr->HasEnvironment());
    HInstruction* lhs, *rhs;
    GetInputsForBinary(instr, &lhs, &rhs);
    HLessThan* clone = new (arena_) HLessThan(lhs, rhs);
    orig_to_clone_.Put(instr, clone);
  }
}

void HInstructionCloner::VisitLessThanOrEqual(HLessThanOrEqual* instr) {
  if (cloning_enabled_) {
    DCHECK(!instr->HasEnvironment());
    HInstruction* lhs, *rhs;
    GetInputsForBinary(instr, &lhs, &rhs);
    HLessThanOrEqual* clone = new (arena_) HLessThanOrEqual(lhs, rhs);
    orig_to_clone_.Put(instr, clone);
  }
}

void HInstructionCloner::VisitLoadClass(HLoadClass* instr) {
  if (cloning_enabled_) {
    HInstruction* input;
    GetInputsForUnary(instr, &input);
    HLoadClass* clone =
        new (arena_) HLoadClass(static_cast<HCurrentMethod*>(input),
                                instr->GetTypeIndex(),
                                instr->GetDexFile(),
                                instr->IsReferrersClass(),
                                instr->GetDexPc(),
                                instr->NeedsAccessCheck(),
                                instr->IsInDexCache());
    if (instr->GetLoadedClassRTI().IsExact()) {
      clone->SetLoadedClassRTI(instr->GetLoadedClassRTI());
    }
    orig_to_clone_.Put(instr, clone);

    // Also clone the environment appropriately.
    CloneEnvironment(instr, clone);
  }
}

void HInstructionCloner::VisitMul(HMul* instr) {
  if (cloning_enabled_) {
    DCHECK(!instr->HasEnvironment());
    HInstruction* lhs, *rhs;
    GetInputsForBinary(instr, &lhs, &rhs);
    HMul* clone = new (arena_) HMul(instr->GetResultType(), lhs, rhs);
    orig_to_clone_.Put(instr, clone);
  }
}

void HInstructionCloner::VisitNeg(HNeg* instr) {
  if (cloning_enabled_) {
    DCHECK(!instr->HasEnvironment());
    HInstruction* input;
    GetInputsForUnary(instr, &input);
    HNeg* clone = new (arena_) HNeg(instr->GetResultType(), input);
    orig_to_clone_.Put(instr, clone);
  }
}

void HInstructionCloner::VisitNot(HNot* instr) {
  if (cloning_enabled_) {
    DCHECK(!instr->HasEnvironment());
    HInstruction* input;
    GetInputsForUnary(instr, &input);
    HNot* clone = new (arena_) HNot(instr->GetResultType(), input);
    orig_to_clone_.Put(instr, clone);
  }
}

void HInstructionCloner::VisitNotEqual(HNotEqual* instr) {
  if (cloning_enabled_) {
    DCHECK(!instr->HasEnvironment());
    HInstruction* lhs, *rhs;
    GetInputsForBinary(instr, &lhs, &rhs);
    HNotEqual* clone = new (arena_) HNotEqual(lhs, rhs);
    orig_to_clone_.Put(instr, clone);
  }
}

void HInstructionCloner::VisitOr(HOr* instr) {
  if (cloning_enabled_) {
    DCHECK(!instr->HasEnvironment());
    HInstruction* lhs, *rhs;
    GetInputsForBinary(instr, &lhs, &rhs);
    HOr* clone = new (arena_) HOr(instr->GetResultType(), lhs, rhs);
    orig_to_clone_.Put(instr, clone);
  }
}

void HInstructionCloner::VisitNullCheck(HNullCheck* instr) {
  if (cloning_enabled_) {
    HInstruction* input;
    GetInputsForUnary(instr, &input);
    HNullCheck* clone = new (arena_) HNullCheck(input, instr->GetDexPc());
    orig_to_clone_.Put(instr, clone);

    // Also clone the environment appropriately.
    CloneEnvironment(instr, clone);
  }
}

void HInstructionCloner::VisitPhi(HPhi* phi) {
  if (cloning_enabled_) {
    DCHECK(!phi->HasEnvironment());
    HPhi* clone = new (arena_) HPhi(arena_, phi->GetRegNumber(), 0, phi->GetType());
    orig_to_clone_.Put(phi, clone);

    // Now copy the inputs while making sure to use input from clone (if one exists).
    for (size_t input_idx = 0u; input_idx != phi->InputCount(); input_idx++) {
      HInstruction* input = phi->InputAt(input_idx);
      auto it = orig_to_clone_.find(input);
      if (use_cloned_inputs_ && it != orig_to_clone_.end()) {
        clone->AddInputNoUseUpdate(it->second);
      } else {
        clone->AddInputNoUseUpdate(input);
      }
    }
  }
}

void HInstructionCloner::VisitRem(HRem* instr) {
  if (cloning_enabled_) {
    DCHECK(!instr->HasEnvironment());
    HInstruction* lhs, *rhs;
    GetInputsForBinary(instr, &lhs, &rhs);
    HRem* clone = new (arena_) HRem(instr->GetResultType(), lhs, rhs, instr->GetDexPc());
    orig_to_clone_.Put(instr, clone);
  }
}

void HInstructionCloner::VisitShl(HShl* instr) {
  if (cloning_enabled_) {
    DCHECK(!instr->HasEnvironment());
    HInstruction* lhs, *rhs;
    GetInputsForBinary(instr, &lhs, &rhs);
    HShl* clone = new (arena_) HShl(instr->GetResultType(), lhs, rhs);
    orig_to_clone_.Put(instr, clone);
  }
}

void HInstructionCloner::VisitShr(HShr* instr) {
  if (cloning_enabled_) {
    DCHECK(!instr->HasEnvironment());
    HInstruction* lhs, *rhs;
    GetInputsForBinary(instr, &lhs, &rhs);
    HShr* clone = new (arena_) HShr(instr->GetResultType(), lhs, rhs);
    orig_to_clone_.Put(instr, clone);
  }
}

void HInstructionCloner::VisitStaticFieldGet(HStaticFieldGet* instr) {
  if (cloning_enabled_) {
    DCHECK(!instr->HasEnvironment());
    HInstruction* input;
    GetInputsForUnary(instr, &input);
    const FieldInfo& fi = instr->GetFieldInfo();
    HStaticFieldGet* clone =
        new (arena_) HStaticFieldGet(input,
                                     instr->GetFieldType(),
                                     instr->GetFieldOffset(),
                                     instr->IsVolatile(),
                                     fi.GetFieldIndex(),
                                     fi.GetDeclaringClassDefIndex(),
                                     fi.GetDexFile(),
                                     fi.GetDexCache(),
                                     instr->GetDexPc());
    orig_to_clone_.Put(instr, clone);
  }
}

void HInstructionCloner::VisitStaticFieldSet(HStaticFieldSet* instr) {
  if (cloning_enabled_) {
    DCHECK(!instr->HasEnvironment());
    HInstruction* cls, *value;
    GetInputsForBinary(instr, &cls, &value);
    const FieldInfo& fi = instr->GetFieldInfo();
    HStaticFieldSet* clone =
        new (arena_) HStaticFieldSet(cls,
                                     value,
                                     instr->GetFieldType(),
                                     instr->GetFieldOffset(),
                                     instr->IsVolatile(),
                                     fi.GetFieldIndex(),
                                     fi.GetDeclaringClassDefIndex(),
                                     fi.GetDexFile(),
                                     fi.GetDexCache(),
                                     instr->GetDexPc());
    orig_to_clone_.Put(instr, clone);
  }
}

void HInstructionCloner::VisitSub(HSub* instr) {
  if (cloning_enabled_) {
    DCHECK(!instr->HasEnvironment());
    HInstruction* lhs, *rhs;
    GetInputsForBinary(instr, &lhs, &rhs);
    HSub* clone = new (arena_) HSub(instr->GetResultType(), lhs, rhs);
    orig_to_clone_.Put(instr, clone);
  }
}

void HInstructionCloner::VisitSuspendCheck(HSuspendCheck* instr) {
  if (cloning_enabled_) {
    HSuspendCheck* clone = new (arena_) HSuspendCheck(instr->GetDexPc());
    orig_to_clone_.Put(instr, clone);

    // Also clone the environment appropriately.
    CloneEnvironment(instr, clone);
  }
}

void HInstructionCloner::VisitTypeConversion(HTypeConversion* instr) {
  if (cloning_enabled_) {
    DCHECK(!instr->HasEnvironment());
    HInstruction* input;
    GetInputsForUnary(instr, &input);
    if (instr->GetResultType() != input->GetType()) {
      HTypeConversion* clone = new (arena_) HTypeConversion(instr->GetResultType(), input,
          instr->GetDexPc());
      orig_to_clone_.Put(instr, clone);
    } else {
      orig_to_clone_.Put(instr, input);
    }
  }
}

void HInstructionCloner::VisitUShr(HUShr* instr) {
  if (cloning_enabled_) {
    HInstruction* lhs, *rhs;
    GetInputsForBinary(instr, &lhs, &rhs);
    HUShr* clone = new (arena_) HUShr(instr->GetResultType(), lhs, rhs);
    orig_to_clone_.Put(instr, clone);
  }
}

void HInstructionCloner::VisitXor(HXor* instr) {
  if (cloning_enabled_) {
    HInstruction* lhs, *rhs;
    GetInputsForBinary(instr, &lhs, &rhs);
    HXor* clone = new (arena_) HXor(instr->GetResultType(), lhs, rhs);
    orig_to_clone_.Put(instr, clone);
  }
}

}  // namespace art
