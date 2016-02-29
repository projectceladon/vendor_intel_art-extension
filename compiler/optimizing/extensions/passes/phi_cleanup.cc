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
  // Replace Phi(x, x, ... , x) with x.
  for (HPostOrderIterator it(*graph_); !it.Done(); it.Advance()) {
    HBasicBlock* block = it.Current();
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

}  // namespace art
