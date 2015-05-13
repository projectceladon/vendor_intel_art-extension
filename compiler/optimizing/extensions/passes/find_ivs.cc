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

#include "find_ivs.h"
#include "graph_x86.h"
#include "induction_variable.h"
#include "loop_iterators.h"

namespace art {

void HFindInductionVariables::FindInductionVariablesHelper(HLoopInformation_X86* info) {
  // Get the loop entry BB.
  HBasicBlock* entry = info->GetHeader();

  // Paranoid.
  DCHECK(entry != nullptr);

  // Clear the induction variable information and inter-iteration variables.
  ArenaVector<HInductionVariable*>& iv_list = info->GetInductionVariables();
  iv_list.clear();

  // Clear inter iteration variables.
  info->ClearInterIterationVariables();

  // We could do a visitor here but we'd use up compile time for the non phi instructions.
  // Let us just visit it with a non C++ iterator.
  for (HInstructionIterator it(entry->GetPhis()); !it.Done(); it.Advance()) {
    HPhi* phi = static_cast<HPhi*> (it.Current());
    DetectAndInitializeBasicIV(info, phi);

    // Also add it as a inter-iteration variable.
    info->AddInterIterationVariable(phi->GetRegNumber());
  }

  // Set is it a count up loop.
  if (iv_list.size() == 1) {
    HInductionVariable* iv = iv_list[0];

    DCHECK(iv != nullptr);

    bool is_count_up = (iv->IsBasic() && iv->IsIncrementPositive());
    info->SetCountUpLoop(is_count_up);
  }
}

void HFindInductionVariables::DetectAndInitializeBasicIV(HLoopInformation_X86* info, HInstruction* phi) {
  size_t input_count = phi->InputCount();

  // For now accept only PHI nodes that have two uses and one define.
  if (input_count != 2) {
    return;
  }

  // Keep track of HInstruction that is candidate for identifying simple IV.
  HInstruction* candidate = nullptr;

  // Go through each of the PHI's uses.
  for (size_t use = 0; use < input_count; use++) {
    // Get HInstruction that defines the use.
    candidate = phi->InputAt(use);

    // If we cannot find the definition, then just continue.
    if (candidate == nullptr) {
      continue;
    }

    // If candidate is executed per iteration of current loop, then we
    // can keep it and run it through the IV detection criteria.
    if (info->ExecutedPerIteration(candidate) == true) {
      break;
    }

    // If we get here it means that the candidate doesn't have to be
    // executed per iteration and thus it cannot be an IV. Therefore,
    // we reset the candidate now.
    candidate = nullptr;
  }

  // If we found a candidate, check that it matches criteria for basic IV.
  if (candidate != nullptr) {
    // We want the right side of the instruction.
    // Currently we only look at additions, future work consists of handling negative increments.
    if (candidate->IsAdd()) {
      HBinaryOperation* binary = candidate->AsBinaryOperation();

      if (binary != nullptr) {
        // If neither side goes back to the phi node, we are not a basic IV.
        if (binary->GetRight() != phi && binary->GetLeft() != phi) {
          return;
        }

        // Get constant right is cool because it will look to the right but,
        //  if right is not a constant, will look to the left for a constant.
        HInstruction* right = binary->GetConstantRight();

        if (right != nullptr) {
          bool is_positive_or_zero = false;
          bool is_wide = false;
          bool is_fp = false;
          if (right->IsFloatConstant()) {
            is_positive_or_zero = right->AsFloatConstant()->GetValue() >= 0.0;
            is_fp = true;
          } else if (right->IsDoubleConstant()) {
            is_wide = true;
            is_fp = true;
            is_positive_or_zero = right->AsDoubleConstant()->GetValue() >= 0;
          } else if (right->IsIntConstant()) {
            is_positive_or_zero = right->AsIntConstant()->GetValue() >= 0;
          } else if (right->IsLongConstant()) {
            is_wide = true;
            is_positive_or_zero = right->AsLongConstant()->GetValue() >= 0;
          } else {
            return;
          }

          // Only accept this IV if the delta_value is positive for now.
          if (is_positive_or_zero == true) {
            ArenaAllocator* arena = graph_->GetArena();
            ArenaVector<HInductionVariable*>& iv_list = info->GetInductionVariables();

            int ssa_reg = phi->GetId();

            HInductionVariable* iv_info = new (arena) HInductionVariable(ssa_reg, right->AsConstant(), is_wide, is_fp, candidate, phi);
            DCHECK(iv_info != nullptr);

            iv_list.push_back(iv_info);
            MaybeRecordStat(MethodCompilationStat::kIntelBIVFound);
            if (IsVerbose()) {
              iv_info->Dump();
            }
          }
        }
      }
    }
  }
}

void HFindInductionVariables::Run() {
  HGraph_X86* graph = GRAPH_TO_GRAPH_X86(graph_);
  HLoopInformation_X86* loop_info = graph->GetLoopInformation();
  for (HOutToInLoopIterator loop_iter(loop_info); loop_iter.Done() == false; loop_iter.Advance()) {
    FindInductionVariablesHelper(loop_iter.Current());
  }
}
}  // namespace art

