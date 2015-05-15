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
#include "pretty_printer.h"

namespace art {

void HGraph_X86::Dump() {
  for (HBasicBlock* block : blocks_) {
    LOG(INFO) << "Block " << block->GetBlockId() << " has LoopInformation " << block->GetLoopInformation();
  }

  StringPrettyPrinter printer(this);
  printer.VisitInsertionOrder();
  const std::string& print_string = printer.str();

  LOG(INFO) << print_string;
}

void HGraph_X86::DeleteBlock(HBasicBlock* block) {
  // Remove all Phis.
  for (HInstructionIterator it2(block->GetPhis()); !it2.Done(); it2.Advance()) {
    HInstruction* insn = it2.Current();
    RemoveAsUser(insn);
    RemoveFromEnvironmentUsers(insn);
    block->RemovePhi(insn->AsPhi(), false);
  }

  // Remove the rest of the instructions.
  for (HInstructionIterator it2(block->GetInstructions()); !it2.Done(); it2.Advance()) {
    HInstruction* insn = it2.Current();
    RemoveAsUser(insn);
    RemoveFromEnvironmentUsers(insn);
    block->RemoveInstruction(insn, false);
  }

  // Remove all successors from the block.
  const ArenaVector<HBasicBlock*>& successors = block->GetSuccessors();
  for (size_t j = successors.size(); j > 0; j--) {
    HBasicBlock* successor = successors[j - 1];
    if (std::find(successor->GetPredecessors().begin(),
                  successor->GetPredecessors().end(),
                  block) != successor->GetPredecessors().end()) {
      successor->RemovePredecessor(block);
    }
    block->RemoveSuccessor(successor);
  }

  // Remove all predecessors.
  block->ClearAllPredecessors();

  // Remove all data structures pointing to the block.
  blocks_[block->GetBlockId()] = nullptr;
  RemoveElement(reverse_post_order_, block);
  if (linear_order_.size() > 0) {
    RemoveElement(linear_order_, block);
  }
}

}  // namespace art
