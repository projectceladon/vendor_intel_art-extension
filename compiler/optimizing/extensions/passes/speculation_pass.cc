/*
 * Copyright (C) 2016 Intel Corporation
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

#include "common_dominator.h"
#include "driver/compiler_driver-inl.h"
#include "ext_utility.h"
#include "graph_x86.h"
#include "pass_option.h"
#include "speculation_pass.h"
#include <set>

namespace art {

bool HSpeculationPass::Gate() {
  if (compiler_driver_->GetInstructionSet() != kX86 &&
      compiler_driver_->GetInstructionSet() != kX86_64) {
    PRINT_PASS_MESSAGE(this, "Skipping pass because ISA is not supported.");
    return false;
  }

  // Speculation passes insert instructions which may not have an implementation in other ISAs.
  return true;
}

void HSpeculationPass::GroupCandidates(std::multimap<HInstruction*, HInstruction*>& candidates) {
  // Now walk through the candidates to consolidate the ones that can use same prediction.
  for (auto it = candidates.begin(); it != candidates.end(); it++) {
    // We want to iterate through all remaining candidates to see if any match, so
    // get the next element that follows.
    auto it2 = it;
    it2++;
    // Iterate through all elements that follow.
    for (; it2 != candidates.end(); it2++) {
      HInstruction* key = it->first;
      HInstruction* same_candidate = it2->second;

      // Skip elements with same key (having same key means that it has
      // already been proven they are the same).
      if (key == it2->first) {
        continue;
      }

      // Now check if these two candidates have same prediction.
      // The comparison is done between key and non-key intentionally.
      if (IsPredictionSame(key, same_candidate)) {
        // The candidates are the same - but can they use same prediction?
        // Well that depends if one dominates the other.
        if (key->GetBlock()->Dominates(same_candidate->GetBlock())) {
          // Erase the candidate we found.
          candidates.erase(it2);
          candidates.insert(std::make_pair(key, same_candidate));
          // For safety, reset the iterators so we can restart from beginning.
          it = candidates.begin();
          it2 = it;

          PRINT_PASS_OSTREAM_MESSAGE(this, "Found same prediction for " <<
                                     key << " and " << same_candidate);
        } else if (same_candidate->GetBlock()->Dominates(key->GetBlock())) {
          // Seems that the candidate actually dominates. Erase the "key" instead.
          candidates.erase(it);
          candidates.insert(std::make_pair(same_candidate, key));

          // Fix all the cases which used the old key since they will need to use the
          // new key. What this means is that anything that this dominated,
          // our new entry will also dominate.
          for (auto it3 = candidates.find(key);
               it3 != candidates.end();
               it3 = candidates.find(key)) {
            HInstruction* value = it3->second;
            candidates.erase(it3);
            candidates.insert(std::make_pair(same_candidate, value));
          }

          // For safety, reset the iterators so we can restart from beginning.
          it = candidates.begin();
          it2 = it;

          PRINT_PASS_OSTREAM_MESSAGE(this, "Found same prediction for " <<
                                     same_candidate << " and " << key);
        }
      }
    }
  }
}

bool HSpeculationPass::HandleDeopt(HInstruction* candidate) {
  HSpeculationGuard* guard = InsertSpeculationGuard(candidate, candidate);

  // If guard insertion fails return false to mark we failed deopt generation.
  if (guard == nullptr) {
    return false;
  }

  // Create a deoptimization node that depends on this.
  HDeoptimize* deopt = new (graph_->GetArena()) HDeoptimize(guard, candidate->GetDexPc());
  guard->GetBlock()->InsertInstructionAfter(deopt, guard);
  deopt->CopyEnvironmentFrom(candidate->GetEnvironment());

  return true;
}

bool HSpeculationPass::HandleCodeVersioning(HInstruction* candidate,
                                            HInstructionCloner& cloner,
                                            bool with_counting ATTRIBUTE_UNUSED,
                                            bool needs_trap,
                                            VersioningApproach versioning) {
  if (versioning == kVersioningLoop || versioning == kVersioningRange) {
    // TODO Add support for these versioning schemes when interesting.
    PRINT_PASS_OSTREAM_MESSAGE(this, "Failed to handle " << candidate << " because versioning " <<
                               versioning << " is not supported.");
    return false;
  } else if (versioning == kVersioningAny || versioning == kVersioningLocal) {
    // We treat the any versioning as local versioning so we do not bloat code.

    // First try to insert the guard. If we fail, we must reject this case.
    HSpeculationGuard* guard = InsertSpeculationGuard(candidate, candidate);
    if (guard == nullptr) {
      PRINT_PASS_OSTREAM_MESSAGE(this, "Failed to handle " << candidate << " because guard "
                                 "insertion failed.");
      return false;
    }

    HGraph_X86* graph = GRAPH_TO_GRAPH_X86(graph_);
    HBasicBlock* pre_block = guard->GetBlock();
    HBasicBlock* post_block = pre_block->SplitBefore(candidate);
    graph_->AddBlock(post_block);
    HBasicBlock* fail_block = graph->CreateNewBasicBlock(candidate->GetDexPc());
    HBasicBlock* success_block = needs_trap ? post_block :
        graph->CreateNewBasicBlock(candidate->GetDexPc());

    // Add control flow instruction which uses the result of guard.
    HIf* hif = new (graph->GetArena()) HIf(guard, candidate->GetDexPc());
    pre_block->AddInstruction(hif);

    graph->CreateLinkBetweenBlocks(pre_block, fail_block, true, true);
    graph->CreateLinkBetweenBlocks(pre_block, success_block, true, true);

    if (needs_trap) {
      HTrap* trap = new (graph->GetArena()) HTrap(candidate->GetDexPc());
      fail_block->AddInstruction(trap);
      HBasicBlock* exit_block = graph->GetExitBlock();
      // Compute dominator for exit block since it is no longer valid.
      HBasicBlock* common_dominator = CommonDominator::ForPair(fail_block, exit_block);
      exit_block->GetDominator()->RemoveDominatedBlock(exit_block);
      common_dominator->AddDominatedBlock(exit_block);
      exit_block->SetDominator(common_dominator);
      // Create link between the two blocks.
      fail_block->AddSuccessor(exit_block);
    } else {
      // Add a goto to the end of the success block.
      HGoto* got = new (graph->GetArena()) HGoto(candidate->GetDexPc());
      success_block->AddInstruction(got);
      // Move the candidate into our new block.
      graph->MoveInstructionBefore(candidate, got);

      // Duplicate the success block now.
      for (HInstructionIterator inst_it(success_block->GetInstructions());
           !inst_it.Done();
           inst_it.Advance()) {
        HInstruction* insn = inst_it.Current();
        insn->Accept(&cloner);
        DCHECK(cloner.AllOkay());
        HInstruction* clone = cloner.GetClone(insn);
        fail_block->AddInstruction(clone);
      }

      // Link the two blocks to the rest of graph.
      DCHECK_NE(success_block, post_block);
      DCHECK_NE(fail_block, post_block);
      graph->CreateLinkBetweenBlocks(fail_block, post_block, false, true);
      success_block->AddSuccessor(post_block);
      post_block->SetDominator(pre_block);
      pre_block->AddDominatedBlock(post_block);

      // Create a phi node to represent the final result.
      if (candidate->GetType() != Primitive::kPrimVoid) {
        HPhi* phi = new (graph->GetArena()) HPhi(graph->GetArena(), kNoRegNumber, 0,
                                                 HPhi::ToPhiType(candidate->GetType()),
                                                 candidate->GetDexPc());
        post_block->AddPhi(phi);
        candidate->ReplaceWith(phi);
        phi->AddInput(cloner.GetClone(candidate));
        phi->AddInput(candidate);
      }
    }

    // Fix the loop information.
    HLoopInformation_X86* loop_info =
        LOOPINFO_TO_LOOPINFO_X86(pre_block->GetLoopInformation());
    if (loop_info != nullptr) {
      if (loop_info->IsBackEdge(*pre_block)) {
        loop_info->ReplaceBackEdge(pre_block, post_block);
      }
      loop_info->AddToAll(success_block);
      if (success_block != post_block) {
        loop_info->AddToAll(post_block);
      }
      if (!needs_trap) {
        loop_info->AddToAll(fail_block);
      }
    }
  }

  return true;
}

static bool NeedsCounting(SpeculationRecoveryApproach recovery) {
  return recovery == kRecoveryCodeVersioningWithCounting;
}

static bool NeedsDeopt(SpeculationRecoveryApproach recovery) {
  return recovery == kRecoveryDeopt;
}

static bool NeedsCodeVersioning(SpeculationRecoveryApproach recovery) {
  return recovery == kRecoveryCodeVersioning || recovery == kRecoveryCodeVersioningWithCounting ||
      recovery == kRecoveryFault;
}

static bool NeedsTrap(SpeculationRecoveryApproach recovery) {
  return recovery == kRecoveryFault;
}

static bool NeedsNoRecovery(SpeculationRecoveryApproach recovery) {
  return recovery == kRecoveryNotNeeded;
}

class ScopedMessage {
 public:
  explicit ScopedMessage(HSpeculationPass* pass) :
      pass_instance_(pass) {
    if (pass_instance_->IsVerbose()) {
      const DexCompilationUnit& compilation_unit = pass_instance_->GetDexCompilationUnit();
      std::string method_name = PrettyMethod(
          compilation_unit.GetDexMethodIndex(),
          *compilation_unit.GetDexFile(), true);
      PRINT_PASS_OSTREAM_MESSAGE(pass_instance_,
                                 "Started devirtualization attempt for " << method_name);
    }
  }
  ~ScopedMessage() {
    if (pass_instance_->IsVerbose()) {
      const DexCompilationUnit& compilation_unit = pass_instance_->GetDexCompilationUnit();
      std::string method_name = PrettyMethod(
          compilation_unit.GetDexMethodIndex(),
          *compilation_unit.GetDexFile(), true);
      PRINT_PASS_OSTREAM_MESSAGE(pass_instance_,
                                 "Finished devirtualization attempt for " << method_name);
    }
  }

 private:
  HSpeculationPass* pass_instance_;
};

void HSpeculationPass::Run() {
  ScopedMessage entry_exit_marker(this);
  if (!Gate()) {
    return;
  }

  std::multimap<HInstruction*, HInstruction*> candidates;

  // Walk the graph to find candidates.
  for (HReversePostOrderIterator it(*graph_); !it.Done(); it.Advance()) {
    HBasicBlock* block = it.Current();
    for (HInstructionIterator inst_it(block->GetInstructions());
         !inst_it.Done(); inst_it.Advance()) {
      HInstruction* instr = inst_it.Current();
      if (IsCandidate(instr)) {
        RecordFoundCandidate();
        if (HasPrediction(instr, true)) {
          // We map each candidate as a pair to itself (since we know it can use same prediction).
          candidates.insert(std::make_pair(instr, instr));
        }
      }
    }
  }

  // If there are no candidates, no reason to continue with this pass.
  if (candidates.empty()) {
    return;
  }

  // If there is more than one candidate, consider if it can use same prediction.
  static PassOption<bool> grouping_enabled(this, compiler_driver_, "EnableGrouping", false);
  if (grouping_enabled.GetValue() && candidates.size() > 1) {
    GroupCandidates(candidates);
  }

  HInstructionCloner cloner(GRAPH_TO_GRAPH_X86(graph_));
  // TODO Add capability to use grouped candidates with the same prediction (which includes
  // counting the appropriate stat for elimination).
  for (auto it : candidates) {
    // The keys are guaranteed to also appear as the second object. So only look at the
    // second part of the pair.
    HInstruction* candidate = it.second;
    DCHECK(candidate != nullptr);

    SpeculationRecoveryApproach recovery = GetRecoveryMethod(candidate);

    if (recovery == kRecoveryAny) {
      // We pick code versioning because this is the least costly version
      // in case of failed speculation.
      recovery = kRecoveryCodeVersioning;
    } else if (recovery == kRecoveryCodeVersioningWithCounting) {
      // TODO Add support for counting - only useful when there is a framework that makes
      // use of it for purpose of reoptimization.
      recovery = kRecoveryCodeVersioning;
    }

    PRINT_PASS_OSTREAM_MESSAGE(this, "Trying to speculate for " << candidate <<
                               " using recovery mode " << recovery);

    if (IsPredictionWorthIt(candidate, recovery)) {
      if (!SupportsRecoveryMethod(recovery, candidate)) {
        // Code versioning is always supported.
        recovery = kRecoveryCodeVersioning;
        DCHECK(SupportsRecoveryMethod(recovery, candidate));
      }

      bool success = false;
      if (NeedsNoRecovery(recovery)) {
        // When recovery is not needed - we have nothing to do.
        success = true;
      } else if (NeedsDeopt(recovery)) {
        success = HandleDeopt(candidate);
      } else if (NeedsCodeVersioning(recovery)) {
        bool needs_trap = NeedsTrap(recovery);
        bool needs_counting = NeedsCounting(recovery);
        VersioningApproach versioning = GetVersioningApproach(candidate);
        success = HandleCodeVersioning(candidate, cloner, needs_counting, needs_trap, versioning);
      } else {
        LOG(FATAL) << "Unhandled recovery mode for speculation: " << recovery;
      }

      if (success) {
        bool guard_inserted = !NeedsNoRecovery(recovery);
        if (HandleSpeculation(candidate, guard_inserted)) {
          if (guard_inserted) {
            PRINT_PASS_OSTREAM_MESSAGE(this, "Successfully speculated for " << candidate <<
                                       " via " << recovery);
          } else {
            PRINT_PASS_OSTREAM_MESSAGE(this, "Successfully speculated for " << candidate <<
                                       " with no guard.");
          }
          RecordSpeculation();
        }
      }
    } else {
      PRINT_PASS_OSTREAM_MESSAGE(this, "Speculating is not worth it for " << candidate);
    }
  }
}

bool HSpeculationPass::IsPredictionWorthIt(HInstruction* instr,
                                           SpeculationRecoveryApproach recovery,
                                           std::list<HInstruction*>* similar_candidates) {
  // Prediction is always positive if recovery is not needed.
  if (recovery == kRecoveryNotNeeded) {
    // It MUST be the case that there is no chance of mispredict if recovery is not needed.
    std::pair<uint64_t, uint64_t> t = GetMispredictRate(instr);
    DCHECK_EQ(t.first, 0u);
    return true;
  }

  double cost_of_recovery = GetCostOfRecoveryMethod(recovery);
  double cost = static_cast<double>(GetCost(instr));
  double profit = static_cast<double>(GetProfit(instr));
  if (similar_candidates != nullptr) {
    // The profit accumulates for each application.
    for (auto it : *similar_candidates) {
      profit += static_cast<double>(GetProfit(it));
    }
  }
  std::pair<uint64_t, uint64_t> t = GetMispredictRate(instr);
  double mispredict_rate = static_cast<double>(t.first) / static_cast<double>(t.second);
  double net_benefit = profit * (1.0 - mispredict_rate) - cost -
      static_cast<double>(cost_of_recovery) * mispredict_rate;
  return net_benefit > 0.0;
}

int32_t HSpeculationPass::GetCostOfRecoveryMethod(SpeculationRecoveryApproach recovery) {
  switch (recovery) {
    case kRecoveryNotNeeded:
    return 0;
    case kRecoveryAny:
    case kRecoveryFault:
    case kRecoveryCodeVersioning:
      return kCostCodeVersioning;
    case kRecoveryDeopt:
      return kCostOfDeopt;
    case kRecoveryCodeVersioningWithCounting:
      return kCostCodeVersioning + kCostToCount;
    default:
      UNIMPLEMENTED(FATAL)<< "Unknown cost of recovery method";
  }

  return 0;
}

}  // namespace art
