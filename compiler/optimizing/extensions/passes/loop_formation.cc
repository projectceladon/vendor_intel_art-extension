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

#include <queue>

#include "graph_x86.h"
#include "loop_iterators.h"
#include "loop_formation.h"

namespace art {


/**
 * @class CompareLoops
 * @brief A comparison between loops.
 */
class HCompareLoops {
 public:
  /**
   * @brief Compare both loops.
   * @param l1 the first loop.
   * @param l2 the second loop.
   * @return does l1 contain less number of BasicBlocks than l2?
   */
  bool operator()(HLoopInformation_X86* l1, HLoopInformation_X86* l2) {
    return l1->NumberOfBlocks() < l2->NumberOfBlocks();
  }
};

void HLoopFormation::Run() {
  /**
   * The algorithm of this method is to actually create all the LoopInformation for the loops of the method.
   *  Put the loops in a priority queue that will have everything sorted in an inverse way:
   *    ie. the loops with the most BBs are first.
   *  This means that when adding the loops:
   *    They are either sibling or nested, they cannot contain a previously added loop.
   */
  std::priority_queue<HLoopInformation_X86*, std::vector<HLoopInformation_X86*>, HCompareLoops> queue;
  std::set<HLoopInformation_X86*> info_set;

  // Post order visit to visit inner loops before outer loops.
  GRAPH_TO_GRAPH_X86(graph_)->ClearLoopInformation();
  for (HPostOrderIterator it(*graph_); !it.Done(); it.Advance()) {
    HBasicBlock* block = it.Current();
    if (block->IsLoopHeader()) {
      HLoopInformation_X86* info = LOOPINFO_TO_LOOPINFO_X86(block->GetLoopInformation());

      // If we do not have it, add it.
      if (info_set.find(info) == info_set.end()) {
        queue.push(info);
        info_set.insert(info);
      }
    }
  }

  // Now clear data structures to make way for initialization.
  HGraph_X86* graph_x86 = GRAPH_TO_GRAPH_X86(graph_);
  graph_x86->ClearLoopInformation();
  for (auto loop : info_set) {
    loop->ResetRelationships();
  }

  // Now add them together.
  while (queue.empty() == false) {
    HLoopInformation_X86* info = queue.top();
    graph_x86->AddLoopInformation(info);
    queue.pop();
  }

  // Finally, now that we are done, set the depths right.
  HLoopInformation_X86* outer = graph_x86->GetLoopInformation();
  if (outer != nullptr) {
    outer->SetDepth(0);
  }
}

}  // namespace art

