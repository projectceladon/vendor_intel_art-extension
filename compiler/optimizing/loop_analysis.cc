/*
 * Copyright (C) 2018 The Android Open Source Project
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

#include "loop_analysis.h"

#include "base/bit_vector-inl.h"
#include "induction_var_range.h"
#include <iostream>

namespace art {

void LoopAnalysis::CalculateLoopBasicProperties(HLoopInformation* loop_info,
                                                LoopAnalysisInfo* analysis_results,
                                                int64_t trip_count) {
  
  analysis_results->trip_count_ = trip_count;

  for (HBlocksInLoopIterator block_it(*loop_info);
       !block_it.Done();
       block_it.Advance()) {
    HBasicBlock* block = block_it.Current();
    for (HBasicBlock* successor : block->GetSuccessors()) {
      if (!loop_info->Contains(*successor)) {
        analysis_results->exits_num_++;

        // We track number of invariant loop exits which correspond to HIf instruction and
        // can be eliminated by loop peeling; other control flow instruction are ignored and will
        // not cause loop peeling to happen as they either cannot be inside a loop, or by
        // definition cannot be loop exits (unconditional instructions), or are not beneficial for
        // the optimization.
        HIf* hif = block->GetLastInstruction()->AsIf();
        if (hif != nullptr && !loop_info->Contains(*hif->InputAt(0)->GetBlock())) {
          analysis_results->invariant_exits_num_++;
        }
      }
    }

    for (HInstructionIterator it(block->GetInstructions()); !it.Done(); it.Advance()) {
      HInstruction* instruction = it.Current();
      if (it.Current()->GetType() == DataType::Type::kInt64) {
        analysis_results->has_long_type_instructions_ = true;
      }
      if (MakesScalarPeelingUnrollingNonBeneficial(instruction)) {
        analysis_results->has_instructions_preventing_scalar_peeling_ = true;
        analysis_results->has_instructions_preventing_scalar_unrolling_ = true;
      }
      analysis_results->instr_num_++;
    }
    analysis_results->bb_num_++;
  }
}

int64_t LoopAnalysis::GetLoopTripCount(HLoopInformation* loop_info,
                                       const InductionVarRange* induction_range) {
  int64_t trip_count;
  if (!induction_range->HasKnownTripCount(loop_info, &trip_count)) {
    trip_count = LoopAnalysisInfo::kUnknownTripCount;
  }
  return trip_count;
}
}  // namespace art
