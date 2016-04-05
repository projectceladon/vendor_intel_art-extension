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
#include "remove_suspend.h"

#include "ext_utility.h"
#include "graph_x86.h"
#include "loop_iterators.h"
#include "base/stringprintf.h"

namespace art {

void HRemoveLoopSuspendChecks::Run() {
  HGraph_X86* graph = GRAPH_TO_GRAPH_X86(graph_);
  HLoopInformation_X86 *graph_loop_info = graph->GetLoopInformation();
  PRINT_PASS_OSTREAM_MESSAGE(this, "Begin: " << GetMethodName(graph));

  // Don't mess with suspend checks if OSR is enabled.
  if (graph_->IsCompilingOsr()) {
    PRINT_PASS_MESSAGE(this, "Skip this method because it uses On-Stack-Replacement");
  } else {
    // Walk all the inner loops in the graph.
    for (HOnlyInnerLoopIterator it(graph_loop_info); !it.Done(); it.Advance()) {
      HLoopInformation_X86* loop_info = it.Current();
      HBasicBlock* pre_header = loop_info->GetPreHeader();

      PRINT_PASS_OSTREAM_MESSAGE(this, "Visit " << loop_info->GetHeader()->GetBlockId()
                                       << ", preheader = " << pre_header->GetBlockId());

      // There must be a suspend check to remove.
      HSuspendCheck* suspend_check = loop_info->GetSuspendCheck();
      if (suspend_check == nullptr) {
        PRINT_PASS_MESSAGE(this, "No SuspendCheck in the loop");
        continue;
      }

      // This must be a countable loop.
      if (!loop_info->HasKnownNumIterations()) {
        PRINT_PASS_MESSAGE(this, "Loop is not countable");
        continue;
      }

      DCHECK(loop_info->IsInner());
      // This must be a simple loop.
      bool is_simple = true;
      for (HBlocksInLoopIterator bb_it(*loop_info); is_simple && !bb_it.Done(); bb_it.Advance()) {
        HBasicBlock* bb = bb_it.Current();
        for (HInstructionIterator insn_it(bb->GetInstructions()); !insn_it.Done(); insn_it.Advance()) {
          HInstruction* insn = insn_it.Current();

          if (insn->IsReturn() ||
              insn->IsNewInstance() ||
              insn->IsNewArray() ||
              insn->IsArrayLength() ||
              insn->IsThrow() ||
              insn->IsInvoke() ||
              insn->IsLoadException()) {
            is_simple = false;
            break;
          }
        }
      }
      if (!is_simple) {
        PRINT_PASS_MESSAGE(this, "Loop is not very simple");
        continue;
      }

      // There must not be another way to exit the loop.
      if (loop_info->CanSideExit()) {
        PRINT_PASS_MESSAGE(this, "Loop can side exit");
        continue;
      }

      uint64_t cost = 0;
      if (!loop_info->GetLoopCost(&cost) || cost == 0) {
        // We ran into an issue while counting instructions in the loop.
        PRINT_PASS_MESSAGE(this, "Unable to compute loop cost");
        continue;
      }

      // Compute the total cost of the loop.
      uint64_t num_iterations = loop_info->GetNumIterations(loop_info->GetHeader());
      if (num_iterations >= (std::numeric_limits<uint64_t>::max() / cost)) {
        // We would overflow computing the total cost.
        PRINT_PASS_MESSAGE(this, "cost * num_iterations is too large");
        continue;
      }
      cost *= num_iterations;

      if (cost > kMaxSuspendFreeLoopCost) {
        PRINT_PASS_OSTREAM_MESSAGE(this, "The cost of the loop (" << cost
                                         << ") exceeds " << kMaxSuspendFreeLoopCost);
        continue;
      }

      // Remove the suspend check.
      PRINT_PASS_OSTREAM_MESSAGE(this, "Remove the suspend check from loop "
                               << loop_info->GetHeader()->GetBlockId()
                               << ", preheader = " << pre_header->GetBlockId());
      suspend_check->GetBlock()->RemoveInstruction(suspend_check);
      loop_info->SetSuppressSuspendCheck(true);
      loop_info->SetSuspendCheck(nullptr);
      MaybeRecordStat(MethodCompilationStat::kIntelRemoveSuspendCheck);
    }
  }
  PRINT_PASS_OSTREAM_MESSAGE(this, "End: " << GetMethodName(graph));
}

}  // namespace art
