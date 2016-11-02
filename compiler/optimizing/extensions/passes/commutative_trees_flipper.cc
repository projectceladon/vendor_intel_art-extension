/*
 * INTEL CONFIDENTIAL
 * Copyright (c) 2016, Intel Corporation All Rights Reserved.
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
 */


#include "commutative_trees_flipper.h"
#include "ext_utility.h"

namespace art {

uint32_t HCommutativeTreesFlipper::EstimateCost(HInstruction* insn) {
  DCHECK(insn != nullptr);
  auto op = insn->AsBinaryOperation();

  if (op == nullptr) {
    // Roughly assume that non-binary operations require 1 VR for operation.
    return 1u;
  }

  auto known = estimated_cost_.find(op);
  if (known != estimated_cost_.end()) {
    // We already know the cost of this operation.
    return known->second;
  }

  auto left = op->InputAt(0);
  auto right = op->InputAt(1);

  if (!op->IsCommutative()) {
    // Non-commutative operations cannot be flipped.
    auto cost = CalculationCost(left, right);
    estimated_cost_[op] = cost;
    return cost;
  } else {
    // There is a special case when we know that placing
    // Phi on left position is benefitable. It is:
    //   phi = Phi(a, b);
    //     ...
    //   b = x + phi
    // Here we know that phi is (most likely) in a register,
    // so making b like
    //   b = phi + x
    // may use this advantage.
    if (!left->IsPhi() && right->IsPhi()) {
      HPhi* phi = right->AsPhi();
      DCHECK(phi != nullptr);
      if (phi->GetBlock()->IsLoopHeader()
          && phi->InputAt(1) == op) {
        PRINT_PASS_OSTREAM_MESSAGE(this, "Flip " << op << " because of loop Phi pattern");
        auto cost = EstimateCost(left);
        op->ReplaceInput(right, 0);
        op->ReplaceInput(left, 1);
        MaybeRecordStat(kIntelCommutativeOperationFlipped);
        estimated_cost_[op] = cost;
        return cost;
      }
    }

    // We can try to flip the args.
    auto no_flip_cost = CalculationCost(left, right);
    auto flip_cost = CalculationCost(right, left);
    // Flip unless either of args is a Phi; be conservative for Phis.
    if (flip_cost < no_flip_cost
        && !left->IsPhi()
        && !right->IsPhi()) {
      PRINT_PASS_OSTREAM_MESSAGE(this, "Flip " << op << " (no flip cost = " << no_flip_cost
                                               << ", flip cost = " << flip_cost << ")");
      // Should never place a constant into the left position.
      DCHECK(!right->IsConstant());
      op->ReplaceInput(right, 0);
      op->ReplaceInput(left, 1);
      MaybeRecordStat(kIntelCommutativeOperationFlipped);
      estimated_cost_[op] = flip_cost;
      return flip_cost;
    } else {
      estimated_cost_[op] = no_flip_cost;
      return no_flip_cost;
    }
  }
}

uint32_t HCommutativeTreesFlipper::CalculationCost(HInstruction* left, HInstruction* right) {
  return std::max(EstimateCost(left), 1 + EstimateCost(right));
}

void HCommutativeTreesFlipper::Run() {
  auto graph = GetGraphX86();

  PRINT_PASS_OSTREAM_MESSAGE(this, "Start " << GetMethodName(graph));

  for (HReversePostOrderIterator b(*graph); !b.Done(); b.Advance()) {
    HBasicBlock* block = b.Current();
    for (HInstructionIterator i(block->GetInstructions()); !i.Done(); i.Advance()) {
      EstimateCost(i.Current());
    }
  }

  PRINT_PASS_OSTREAM_MESSAGE(this, "End " << GetMethodName(graph));
  estimated_cost_.clear();
}

}  // namespace art
