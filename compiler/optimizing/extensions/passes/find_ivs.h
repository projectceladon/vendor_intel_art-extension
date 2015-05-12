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

#ifndef VENDOR_INTEL_ART_EXTENSION_PASSES_FIND_IVS_H_
#define VENDOR_INTEL_ART_EXTENSION_PASSES_FIND_IVS_H_

#include "induction_variable.h"
#include "optimization_x86.h"

namespace art {

// Forward declarations.
class HLoopInformation_X86;

/**
 * Find the Induction Variables of the loop.
 */
class HFindInductionVariables : public HOptimization_X86 {
 protected:
  void DetectAndInitializeBasicIV(HLoopInformation_X86* info, HPhi* phi);
  void FindInductionVariablesHelper(HLoopInformation_X86* info);
  HIf* FindLoopIf(HLoopInformation_X86* loop);
  bool FindLoopUpperBound(HLoopInformation_X86* info, int64_t& upper_bound);
  bool IsValidCastForIV(HInstruction* candidate, HLoopInformation_X86* loop);

 public:
  explicit HFindInductionVariables(HGraph* graph, OptimizingCompilerStats* stats = nullptr)
    : HOptimization_X86(graph, "find_ivs", stats) {}

  void Run() OVERRIDE;
};
}  // namespace art

#endif  // VENDOR_INTEL_ART_EXTENSION_PASSES_FIND_IVS_H_
