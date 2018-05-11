/*
 * Copyright (C) 2018 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "cloning.h"
#include "ext_utility.h"
#include "graph_x86.h"
#include "loop_iterators.h"
#include "loop_partial_unrolling.h"
#include "loop_unroll_by_factor.h"

namespace art {

void HLoopUnrollByFactor::Run() {
  HGraph_X86* graph = GRAPH_TO_GRAPH_X86(graph_);
  HLoopInformation_X86* loop_start = graph->GetLoopInformation();
  bool graph_updated = false;
  PRINT_PASS_OSTREAM_MESSAGE(this, "Begin " << GetMethodName(graph));

  // For each inner loop in the graph, we will try to apply Loop Unrolling.
  for (HOnlyInnerLoopIterator it(loop_start); !it.Done(); it.Advance()) {
    HLoopInformation_X86* loop = it.Current();

    HLoopPartialUnrolling loop_partial_unrolling(loop, this);

    if (!Gate(&loop_partial_unrolling)) {
      continue;
    }

    if (!loop_partial_unrolling.PartialUnroll()) {
      continue;
    }

    HBasicBlock* loop_header = loop->GetHeader();
    MaybeRecordStat(MethodCompilationStat::kIntelLoopFullyUnrolled);
    PRINT_PASS_OSTREAM_MESSAGE(this, "Loop #" << loop_header->GetBlockId()
      << " of method " << GetMethodName(graph)
      << " has been successfully partially unrolled by factor "
      << kDefaultUnrollFactor);
  }

  if (graph_updated) {
    graph->RebuildDomination();
  }

  PRINT_PASS_OSTREAM_MESSAGE(this, "End " << GetMethodName(graph));
}

bool HLoopUnrollByFactor::Gate(HLoopPartialUnrolling* loop_partial_unrolling) const {
  DCHECK(loop_partial_unrolling != nullptr);

  if (!loop_partial_unrolling->Gate(kDefaultMaxInstructionsUnrolled, kDefaultUnrollFactor)) {
    return false;
  }

  return true;
}

}  // namespace art
