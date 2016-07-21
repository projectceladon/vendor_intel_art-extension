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

#include <unordered_set>

#include "ext_utility.h"
#include "graph_x86.h"
#include "loop_iterators.h"
#include "phi_cleanup.h"

namespace art {

void HPhiCleanup::Run() {
  if (Gate()) {
    PRINT_PASS_OSTREAM_MESSAGE(this, "Begin: " << GetMethodName(graph_));
    CleanUpPhis();
    PRINT_PASS_OSTREAM_MESSAGE(this, "End " << GetMethodName(graph_));
  } else {
    PRINT_PASS_OSTREAM_MESSAGE(this, "Skip the method " << GetMethodName(graph_)
                                     << " because it has no loops");
  }
}

bool HPhiCleanup::Gate() {
  // Apply only for methods with loops.
  return GRAPH_TO_GRAPH_X86(graph_)->GetLoopInformation() != nullptr;
}

void HPhiCleanup::CleanUpPhis() {
  // This is to avoid looking into the same Phis multiple times.
  std::unordered_set<HInstruction*> seen_insns;

  // Replace Phi(x, x, ... , x) with x.
  for (HPostOrderIterator it(*graph_); !it.Done(); it.Advance()) {
    HBasicBlock* block = it.Current();
    // Inputs of a CatchPhi don't dominate the catch block and DCHECK below fails.
    if (block->IsCatchBlock()) {
      continue;
    }
    for (HInstructionIterator phi_it(block->GetPhis());
                              !phi_it.Done(); phi_it.Advance()) {
      HPhi* phi = phi_it.Current()->AsPhi();
      if (AllInputsSame(phi)) {
        PRINT_PASS_OSTREAM_MESSAGE(this, "Removing " << phi << " because all"
                                         << " its inputs are the same.");
        DCHECK(phi->InputAt(0)->GetBlock()->Dominates(block));
        phi->ReplaceWith(phi->InputAt(0));
        block->RemovePhi(phi);
        MaybeRecordStat(MethodCompilationStat::kIntelPhiNodeEliminated);
      } else if (RemoveClique(phi, seen_insns)) {
        PRINT_PASS_OSTREAM_MESSAGE(this, "Clique removed successfully");
      }
    }
  }
}


bool HPhiCleanup::AllInputsSame(HPhi* phi) {
  HInstruction* in_0 = phi->InputAt(0);
  for (size_t i = 1, e = phi->InputCount(); i < e; i++) {
    if (phi->InputAt(i) != in_0) {
      return false;
    }
  }
  return true;
}

bool HPhiCleanup::RemoveCliqueHelper(HInstruction* to_check,
                                     std::unordered_set<HInstruction*>& seen_insns,
                                     std::unordered_set<HInstruction*>& candidates) {
  if (candidates.find(to_check) != candidates.end()) {
    // We are already considering this instruction as a candidate for removal.
    return true;
  } else if (seen_insns.find(to_check) != seen_insns.end()) {
    // We have already rejected this instruction.
    return false;
  } else if (to_check->HasEnvironmentUses()) {
    // The Phi should not have env uses.
    return false;
  }

  // Consider this instruction as a candidate for removal.
  candidates.insert(to_check);
  seen_insns.insert(to_check);

  // We reject the instruction if its use is either a control flow or
  // it has side effects. In other cases we run the recursive check for it.
  const HUseList<HInstruction*>& uses = to_check->GetUses();
  for (auto it = uses.begin(), end = uses.end(); it != end; ++it) {
    HInstruction* use = it->GetUser();
    if (use->IsControlFlow() ||
        use->GetSideEffects().HasSideEffectsExcludingGC() ||
        use->CanThrow()) {
      // Instruction with side effects or control flow is not something that we can just remove.
      return false;
    } else if (!RemoveCliqueHelper(use, seen_insns, candidates)) {
      // Recursive check failed.
      return false;
    }
  }

  // This instruction seems a fine candidate for removal.
  return true;
}

bool HPhiCleanup::RemoveClique(HPhi* phi,
                               std::unordered_set<HInstruction*>& seen_insns) {
  // For quick checks we keep the clique candidates in set.
  std::unordered_set<HInstruction*> candidates;

  if (RemoveCliqueHelper(phi, seen_insns, candidates)) {
    PRINT_PASS_OSTREAM_MESSAGE(this, "Found a clique of: " << candidates.size() << " elements");

    // Delete unsafely.
    for (auto to_remove : candidates) {
      if (to_remove->IsPhi()) {
        to_remove->GetBlock()->RemovePhi(to_remove->AsPhi(), false);
      } else {
        to_remove->GetBlock()->RemoveInstruction(to_remove, false);
      }
      RemoveAsUser(to_remove);
    }

    return true;
  }

  return false;
}

}  // namespace art
