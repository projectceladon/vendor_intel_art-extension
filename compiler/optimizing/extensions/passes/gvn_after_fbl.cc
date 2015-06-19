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
#include "gvn_after_fbl.h"
#include "gvn.h"
#include "loop_iterators.h"
#include "side_effects_analysis.h"

namespace art {

void GVNAfterFormBottomLoops::Run() {
  HGraph_X86* graph = GRAPH_TO_GRAPH_X86(graph_);
  HLoopInformation_X86* loop = graph->GetLoopInformation();

  // Go through each loop and check whether there is at least one bottom tested
  // loop. If so, launch the GVN pass.
  bool do_gvn = false;
  for (HOutToInLoopIterator iter(loop); !iter.Done(); iter.Advance()) {
    HLoopInformation_X86* l = iter.Current();

    DCHECK(l != nullptr);  // Paranoid.
    if (l->IsBottomTested()) {
      do_gvn = true;
      break;
    }
  }

  // If there are no bottom tested loops, give up now.
  if (!do_gvn) {
    return;
  }

  // We have seen a bottom tested loop.  Rerun GVN.
  SideEffectsAnalysis side_effects(graph_);
  side_effects.Run();
  GVNOptimization gvn(graph_, side_effects);
  gvn.Run();
}

}  // namespace art
