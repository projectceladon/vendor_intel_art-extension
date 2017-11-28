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

#ifndef ART_COMPILER_OPTIMIZING_EXTENSIONS_INFRASTRUCTURE_CLONING_H_
#define ART_COMPILER_OPTIMIZING_EXTENSIONS_INFRASTRUCTURE_CLONING_H_

#include "graph_x86.h"
#include "nodes.h"

namespace art {

/**
 * @brief Used to clone instructions.
 * @details Note that this does not copy users from source. Thus, instructions
 * are not valid until proper inputs are added.
 */
class HInstructionCloner : public HGraphVisitor {
 public:
  /**
   * Create an instruction cloner.
   * @param graph Graph containing the instructions to clone.
   * @param enable_cloning 'false' to see if cloning is possible, 'true' to clone.
   * @param use_cloned_inputs 'true' if cloned instructions should use already
   * cloned inputs.
   */
  HInstructionCloner(HGraph_X86* graph,
                     bool enable_cloning = true,
                     bool use_cloned_inputs = true) :
      HGraphVisitor(graph),
      cloning_enabled_(enable_cloning),
      all_cloned_okay_(true),
      use_cloned_inputs_(use_cloned_inputs),
      arena_(graph->GetArena()),
      debug_name_failed_clone_(nullptr) {}

  void VisitInstruction(HInstruction* instruction) OVERRIDE {
    // If this instruction does not have a visitor, assume that it was not cloned correctly.
    all_cloned_okay_ = false;
    debug_name_failed_clone_ = instruction->DebugName();
  }

  void AddCloneManually(HInstruction* original, HInstruction* clone) {
    DCHECK(original != nullptr);
    DCHECK(clone != nullptr);
    orig_to_clone_.Put(original, clone);
  }

  HInstruction* GetClone(HInstruction* source) const {
    if (orig_to_clone_.count(source) != 0) {
      HInstruction* clone = orig_to_clone_.Get(source);
      DCHECK(clone != nullptr);
      return clone;
    } else {
      return nullptr;
    }
  }

  bool AllOkay() const;

  const char* GetDebugNameForFailedClone() const {
    DCHECK(!all_cloned_okay_);
    return debug_name_failed_clone_;
  }

  HEnvironment* CloneEnvironment(HEnvironment* env, HInstruction* clone);

  void VisitAbove(HAbove* instr) OVERRIDE;
  void VisitAboveOrEqual(HAboveOrEqual* instr) OVERRIDE;
  void VisitAdd(HAdd* instr) OVERRIDE;
  void VisitAnd(HAnd* instr) OVERRIDE;
  void VisitArrayGet(HArrayGet* instr) OVERRIDE;
  void VisitArrayLength(HArrayLength* instr) OVERRIDE;
  void VisitArraySet(HArraySet* instr) OVERRIDE;
  void VisitBelow(HBelow* instr) OVERRIDE;
  void VisitBelowOrEqual(HBelowOrEqual* instr) OVERRIDE;
  void VisitBooleanNot(HBooleanNot* instr) OVERRIDE;
  void VisitBoundsCheck(HBoundsCheck* instr) OVERRIDE;
  void VisitBoundType(HBoundType* instr) OVERRIDE;
  void VisitCheckCast(HCheckCast* instr) OVERRIDE;
  void VisitClearException(HClearException* instr) OVERRIDE { VisitInstruction(instr); };
  void VisitClinitCheck(HClinitCheck* instr) OVERRIDE;
  void VisitCompare(HCompare* instr) OVERRIDE;
  void VisitCondition(HCondition* instr) OVERRIDE { VisitInstruction(instr); }
  void VisitCurrentMethod(HCurrentMethod* instr) OVERRIDE { VisitInstruction(instr); }
  void VisitDeoptimize(HDeoptimize* instr) OVERRIDE { VisitInstruction(instr); }
  void VisitDiv(HDiv* instr) OVERRIDE;
  void VisitDivZeroCheck(HDivZeroCheck* instr) OVERRIDE;
  void VisitDoubleConstant(HDoubleConstant* instr) OVERRIDE { VisitInstruction(instr); }
  void VisitEqual(HEqual* instr) OVERRIDE;
  void VisitExit(HExit* instr) OVERRIDE { VisitInstruction(instr); }
  void VisitFloatConstant(HFloatConstant* instr) OVERRIDE { VisitInstruction(instr); }
  void VisitGoto(HGoto* instr) OVERRIDE;
  void VisitGreaterThan(HGreaterThan* instr) OVERRIDE;
  void VisitGreaterThanOrEqual(HGreaterThanOrEqual* instr) OVERRIDE;
  void VisitIf(HIf* instr) OVERRIDE;
  void VisitInstanceFieldGet(HInstanceFieldGet* instr) OVERRIDE;
  void VisitInstanceFieldSet(HInstanceFieldSet* instr) OVERRIDE;
  void VisitInstanceOf(HInstanceOf* instr) OVERRIDE;
  void VisitIntConstant(HIntConstant* instr) OVERRIDE { VisitInstruction(instr); }
  void VisitInvokeInterface(HInvokeInterface* instr) OVERRIDE { VisitInstruction(instr); }
  void VisitInvokeStaticOrDirect(HInvokeStaticOrDirect* instr) OVERRIDE { VisitInstruction(instr); }
  void VisitInvokeVirtual(HInvokeVirtual* instr) OVERRIDE { VisitInstruction(instr); }
  void VisitInvokeUnresolved(HInvokeUnresolved* instr) OVERRIDE { VisitInstruction(instr); }
  void VisitLessThan(HLessThan* instr) OVERRIDE;
  void VisitLessThanOrEqual(HLessThanOrEqual* instr) OVERRIDE;
  void VisitLoadClass(HLoadClass* instr) OVERRIDE;
  void VisitLoadException(HLoadException* instr) OVERRIDE { VisitInstruction(instr); }
  void VisitLoadString(HLoadString* instr) OVERRIDE { VisitInstruction(instr); }
  void VisitLongConstant(HLongConstant* instr) OVERRIDE { VisitInstruction(instr); }
  void VisitMemoryBarrier(HMemoryBarrier* instr) OVERRIDE { VisitInstruction(instr); }
  void VisitMonitorOperation(HMonitorOperation* instr) OVERRIDE { VisitInstruction(instr); }
  void VisitMul(HMul* instr) OVERRIDE;
  void VisitNeg(HNeg* instr) OVERRIDE;
  void VisitNewArray(HNewArray* instr) OVERRIDE { VisitInstruction(instr); }
  void VisitNewInstance(HNewInstance* instr) OVERRIDE { VisitInstruction(instr); }
  void VisitNot(HNot* instr) OVERRIDE;
  void VisitNotEqual(HNotEqual* instr) OVERRIDE;
  void VisitNullConstant(HNullConstant* instr) OVERRIDE { VisitInstruction(instr); }
  void VisitNullCheck(HNullCheck* instr) OVERRIDE;
  void VisitOr(HOr* instr) OVERRIDE;
  void VisitPackedSwitch(HPackedSwitch* instr) OVERRIDE { VisitInstruction(instr); }
  void VisitParallelMove(HParallelMove* instr) OVERRIDE { VisitInstruction(instr); }
  void VisitParameterValue(HParameterValue* instr) OVERRIDE { VisitInstruction(instr); }
  void VisitPhi(HPhi* instr) OVERRIDE;
  void VisitRem(HRem* instr) OVERRIDE;
  void VisitReturn(HReturn* instr) OVERRIDE { VisitInstruction(instr); }
  void VisitReturnVoid(HReturnVoid* instr) OVERRIDE { VisitInstruction(instr); }
  void VisitShl(HShl* instr) OVERRIDE;
  void VisitShr(HShr* instr) OVERRIDE;
  void VisitStaticFieldGet(HStaticFieldGet* instr) OVERRIDE;
  void VisitStaticFieldSet(HStaticFieldSet* instr) OVERRIDE;
  void VisitSub(HSub* instr) OVERRIDE;
  void VisitSuspend(HSuspend* instr) OVERRIDE { VisitInstruction(instr); }
  void VisitSuspendCheck(HSuspendCheck* instr) OVERRIDE;
  void VisitTestSuspend(HTestSuspend* instr) OVERRIDE { VisitInstruction(instr); }
  void VisitThrow(HThrow* instr) OVERRIDE { VisitInstruction(instr); }
  void VisitTryBoundary(HTryBoundary* instr) OVERRIDE { VisitInstruction(instr); }
  void VisitTypeConversion(HTypeConversion* instr) OVERRIDE;
  void VisitUShr(HUShr* instr) OVERRIDE;
  void VisitUnresolvedInstanceFieldGet(HUnresolvedInstanceFieldGet* instr) OVERRIDE { VisitInstruction(instr); };
  void VisitUnresolvedInstanceFieldSet(HUnresolvedInstanceFieldSet* instr) OVERRIDE { VisitInstruction(instr); };
  void VisitUnresolvedStaticFieldGet(HUnresolvedStaticFieldGet* instr) OVERRIDE { VisitInstruction(instr); };
  void VisitUnresolvedStaticFieldSet(HUnresolvedStaticFieldSet* instr) OVERRIDE { VisitInstruction(instr); };
  void VisitXor(HXor* instr) OVERRIDE;
  void VisitConstant(HConstant* instr) OVERRIDE { VisitInstruction(instr); }

  // TODO: fix FakeString.

 private:
  void GetInputsForUnary(HInstruction* instr, HInstruction** input_ptr) const;
  void GetInputsForBinary(HInstruction* instr,
                          HInstruction** lhs_ptr,
                          HInstruction** rhs_ptr) const;
  void GetInputsForTernary(HInstruction* instr,
                           HInstruction** input0_ptr,
                           HInstruction** input1_ptr,
                           HInstruction** input2_ptr) const;
  void CloneEnvironment(HInstruction* instr, HInstruction* clone);

  const bool cloning_enabled_;
  bool all_cloned_okay_;
  const bool use_cloned_inputs_;
  SafeMap<HInstruction*, HInstruction*> orig_to_clone_;
  ArenaAllocator* arena_;
  const char* debug_name_failed_clone_;
};

}  // namespace art

#endif  // ART_COMPILER_OPTIMIZING_EXTENSIONS_INFRASTRUCTURE_CLONING_H_
