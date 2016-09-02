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
 */

#include "ext_utility.h"
#include "loop_iterators.h"
#include "pass_option.h"
#include "peeling.h"

namespace art {

bool HLoopPeeling::ShouldPeel(HLoopInformation_X86* loop) const {
  DCHECK(loop->IsInner());

  static PassOption<int64_t> block_count_threshold(this,
                                                   driver_,
                                                   "BlockCountThreshold",
                                                   kDefaultBlockThreshold);
  if (loop->GetBlocks().NumSetBits() > block_count_threshold.GetValue()) {
    PRINT_PASS_OSTREAM_MESSAGE(this, "Peeling failed because loop block count exceeds "
                               << block_count_threshold.GetValue() << ".");
    return false;
  }

  int64_t num_candidate_instr = 0u;
  int64_t instruction_count = 0u;
  int64_t num_opaque_invokes = 0u;
  static PassOption<int64_t> instruction_count_threshold(this,
                                                         driver_,
                                                         "InstructionCountThreshold",
                                                         kDefaultInstructionThreshold);
  static PassOption<int64_t> opaque_invoke_threshold(this,
                                                     driver_,
                                                     "OpaqueInvokeThreshold",
                                                     kDefaultAllowedOpaqueInvokes);

  // Check to see if peeling may be worth it - these are cases where GVN may do a better job
  // by eliminating throwers/getters inside of a loop.
  for (HBlocksInLoopIterator it_loop(*loop); !it_loop.Done(); it_loop.Advance()) {
    HBasicBlock* block = it_loop.Current();
    for (HInstruction* instruction = block->GetFirstInstruction();
        instruction != nullptr;
        instruction = instruction->GetNext()) {
      instruction_count++;
      if (instruction_count > instruction_count_threshold.GetValue()) {
        PRINT_PASS_OSTREAM_MESSAGE(this, "Peeling failed because loop instruction count exceeds "
                                   << instruction_count_threshold.GetValue() << ".");
        return false;
      }

      // Test whether we are looking at an opaque invoke. We test this by testing
      // whether or not it can be moved - since moving it signifies we know what it does.
      if (instruction->IsInvoke() && !instruction->CanBeMoved()) {
        num_opaque_invokes++;
        if (num_opaque_invokes > opaque_invoke_threshold.GetValue()) {
          PRINT_PASS_OSTREAM_MESSAGE(this, "Peeling failed because the opaque invoke count "
                                     "exceeds " << opaque_invoke_threshold.GetValue() << ".");
          return false;
        }
      }

      if (!instruction->CanBeMoved()) {
        // If instruction cannot be moved, no point in considering this instruction.
        continue;
      }

      // Check if thrower or heap mutator/reader first.
      // We also handle LoadString and LoadClass specially because they may not fall
      // in either category but in reality are useful to hoist since they have no
      // IR inputs and will reload same thing over and over.
      if (instruction->CanThrow() || instruction->GetSideEffects().HasSideEffectsExcludingGC() ||
          instruction->GetSideEffects().DoesAnyRead() ||
          instruction->IsLoadClass() || instruction->IsLoadString()) {
        bool all_inputs_from_outside = true;

        // Now check that all inputs are from outside.
        for (size_t i = 0, e = instruction->InputCount(); i < e; ++i) {
          HInstruction* input = instruction->InputAt(i);
          if (loop->Contains(*input->GetBlock())) {
            all_inputs_from_outside = false;
            break;
          }
        }

        // If all inputs from outside, count that we found a candidate which makes
        // peeling worth it.
        if (all_inputs_from_outside) {
          num_candidate_instr++;
        }
      }
    }
  }

  static PassOption<int64_t> least_candidate_count(this,
                                                   driver_,
                                                   "LeastCandidateCount",
                                                   kDefaultLeastCandidateCount);
  if (num_candidate_instr >= least_candidate_count.GetValue()) {
    return true;
  }

  PRINT_PASS_OSTREAM_MESSAGE(this, "Peeling failed because no viable candidate was found.");
  return false;
}

void HLoopPeeling::Run() {
  HOnlyInnerLoopIterator inner_iter(GetGraphX86()->GetLoopInformation());
  PRINT_PASS_OSTREAM_MESSAGE(this, "Start " << GetMethodName(graph_));
  while (!inner_iter.Done()) {
    HLoopInformation_X86* inner_loop = inner_iter.Current();
    // If the loop should be peeled, make an attempt to do it.
    if (ShouldPeel(inner_loop)) {
      if (inner_loop->IsPeelable(this)) {
        inner_loop->PeelHead(this);
        PRINT_PASS_OSTREAM_MESSAGE(this, "Successfully peeled loop with header block " <<
                                         inner_loop->GetHeader()->GetBlockId() << '.');
        MaybeRecordStat(MethodCompilationStat::kIntelLoopPeeled);
      }
    }
    inner_iter.Advance();
  }
  PRINT_PASS_OSTREAM_MESSAGE(this, "End " << GetMethodName(graph_));
}

}  // namespace art
