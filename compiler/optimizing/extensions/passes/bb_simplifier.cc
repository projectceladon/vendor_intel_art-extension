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

#include "bb_simplifier.h"
#include "ext_utility.h"
#include "graph_x86.h"

namespace art {

bool HBBSimplifier::TrySimplifyIf(HBasicBlock* block, HIf* if_insn) {
  HGraph_X86* graph = GRAPH_TO_GRAPH_X86(graph_);

  // Paranoia.
  DCHECK(if_insn != nullptr);
  DCHECK_EQ(block->GetSuccessors().size(), 2u);

  // Simplify pattern:
  //    A
  //   / \
  //  B   C
  //   \ /
  //    D
  // Where B and C are single GOTO blocks, and D has no Phis.
  HBasicBlock* succ_1 = block->GetSuccessors()[0];
  HBasicBlock* succ_2 = block->GetSuccessors()[1];

  DCHECK(succ_1 != nullptr);
  DCHECK(succ_2 != nullptr);

  if (succ_1->IsSingleGoto()
      && succ_2->IsSingleGoto()
      && succ_1->GetPredecessors().size() == 1u
      && succ_2->GetPredecessors().size() == 1u) {
    HBasicBlock* lower_block = succ_1->GetSingleSuccessor();
    if (lower_block != block
        && lower_block == succ_2->GetSingleSuccessor()
        && lower_block->GetPhis().IsEmpty()) {

      PRINT_PASS_OSTREAM_MESSAGE(this, "Simplifying branching in block #"
                                       << block->GetBlockId());

      // First, remove block C.
      block->RemoveSuccessor(succ_2);
      graph->DeleteBlock(succ_2, /* remove_from_loops */ true);

      // Second, remove block B, taking care about back edges.
      bool succ_1_is_a_back_edge = false;
      for (auto loop = LOOPINFO_TO_LOOPINFO_X86(succ_1->GetLoopInformation());
           loop != nullptr && !succ_1_is_a_back_edge;
           loop = loop->GetParent()) {
        succ_1_is_a_back_edge |= loop->IsBackEdge(*succ_1);
      }

      if (!succ_1_is_a_back_edge) {
        block->ReplaceSuccessor(succ_1, lower_block);
        graph->DeleteBlock(succ_1, /* remove_from_loops */ true);
      }

      // Third, replace IF with GOTO.
      HInstruction* condition = if_insn->InputAt(0);
      block->ReplaceAndRemoveInstructionWith(if_insn, new (graph_->GetArena()) HGoto());

      // Finally, try to remove the condition.
      std::unordered_set<HInstruction*> all_removed;
      TryKillUseTree(this, condition, &all_removed);
      if (!all_removed.empty()) {
        MaybeRecordStat(MethodCompilationStat::kIntelBranchConditionDeleted);
        PRINT_PASS_OSTREAM_MESSAGE(this, "Removed condition and "
                                         << (all_removed.size() - 1)
                                         << " other instructions");
      }

      MaybeRecordStat(MethodCompilationStat::kIntelBranchSimplified);
      return true;
    }
  }

  return false;
}

bool HBBSimplifier::TrySimplify(HBasicBlock* block) {
  // Paranoia.
  DCHECK(block != nullptr);

  // Skip entry, exit blocks and catch blocks.
  if (block->IsEntryBlock()
      || block->IsExitBlock()
      || block->IsCatchBlock()) {
    return false;
  }

  // Simplification of blocks with two or more successors.
  HInstruction* last = block->GetLastInstruction();
  DCHECK(last != nullptr);
  DCHECK(last->IsControlFlow());
  if (last->IsIf()) {
    return TrySimplifyIf(block, last->AsIf());
  }

  return false;
}

void HBBSimplifier::Run() {
  HGraph_X86* graph = GRAPH_TO_GRAPH_X86(graph_);
  PRINT_PASS_OSTREAM_MESSAGE(this, "Start " << GetMethodName(graph));

  bool changed = false;

  for (HPostOrderIterator it(*graph_); !it.Done(); it.Advance()) {
    changed |= TrySimplify(it.Current());
  }

  if (changed) {
    PRINT_PASS_OSTREAM_MESSAGE(this, "Rebuilding domination in " << GetMethodName(graph));
    graph->RebuildDomination();
  }

  PRINT_PASS_OSTREAM_MESSAGE(this, "End " << GetMethodName(graph));
}

}  // namespace art
