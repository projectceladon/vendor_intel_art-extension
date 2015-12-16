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
#include "graph_x86.h"
#include "loop_iterators.h"
#include "peeling.h"

namespace art {

static constexpr size_t kLeastRequiredCandidateCount = 1u;

bool HLoopPeeling::ShouldPeel(HLoopInformation_X86* loop) {
  DCHECK(loop->IsInner());

  size_t num_candidate_instr = 0u;
  if (loop->GetBlocks().NumSetBits() <= 2u) {
    // Check to see if peeling may be worth it - these are cases where GVN may do a better job
    // by eliminating throwers/getters inside of a loop.
    for (HBlocksInLoopIterator it_loop(*loop); !it_loop.Done(); it_loop.Advance()) {
      HBasicBlock* block = it_loop.Current();
      for (HInstruction* instruction = block->GetFirstInstruction();
          instruction != nullptr;
          instruction = instruction->GetNext()) {
        // Check if thrower or heap mutator/reader first.
        // We also handle LoadString and LoadClass specially because they may not fall
        // in either category but in reality are useful to hoist since they have no
        // IR inputs and will reload same thing over and over.
        if (instruction->CanThrow() || instruction->HasSideEffects() ||
            instruction->GetSideEffects().HasDependencies() ||
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
            if (num_candidate_instr >= kLeastRequiredCandidateCount) {
              return true;
            }
          }
        }
      }
    }
  }

  return false;
}

void HLoopPeeling::Run() {
  HOnlyInnerLoopIterator inner_iter(GRAPH_TO_GRAPH_X86(graph_)->GetLoopInformation());
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
