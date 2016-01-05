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

#ifndef ART_COMPILER_OPTIMIZING_EXTENSIONS_PASSES_PEELING_H_
#define ART_COMPILER_OPTIMIZING_EXTENSIONS_PASSES_PEELING_H_

#include "driver/compiler_driver.h"
#include "nodes.h"
#include "optimization_x86.h"

namespace art {

/**
 * @brief This is an optimization pass that peels some loops.
 * @details The peeling applies always to the loops on which the heuristics pass.
 */
class HLoopPeeling : public HOptimization_X86 {
 public:
  HLoopPeeling(HGraph* graph, CompilerDriver* driver,
               OptimizingCompilerStats* stats = nullptr)
    : HOptimization_X86(graph, "loop_peeling", stats), driver_(driver) {}

  void Run() OVERRIDE;

  bool ShouldPeel(HLoopInformation_X86* loop);

 private:
  // How many maximum instructions a loop can have to be considered for peeling.
  static constexpr int kDefaultInstructionThreshold = 100;
  // How many maximum blocks a loop can have to be considered for peeling.
  static constexpr int kDefaultBlockThreshold = 10;
  // How many viable candidates make peeling viable.
  static constexpr int kDefaultLeastCandidateCount = 1;
  // How many opaque (no analysis available) invokes should terminate peeling consideration.
  static constexpr bool kDefaultAllowedOpaqueInvokes = 2;
  const CompilerDriver* driver_;
};

}  // namespace art

#endif  // ART_COMPILER_OPTIMIZING_EXTENSIONS_PASSES_PEELING_H_
