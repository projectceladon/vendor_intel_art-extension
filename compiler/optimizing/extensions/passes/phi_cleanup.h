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

#ifndef ART_COMPILER_OPTIMIZING_EXTENSIONS_PASSES_PHI_CLEANUP_H_
#define ART_COMPILER_OPTIMIZING_EXTENSIONS_PASSES_PHI_CLEANUP_H_

#include "nodes.h"
#include "optimization_x86.h"

namespace art {

/**
 * @brief This is an optimization pass that removes some unneeded phis.
 */
class HPhiCleanup : public HOptimization_X86 {
 public:
  explicit HPhiCleanup(HGraph* graph, OptimizingCompilerStats* stats = nullptr)
    : HOptimization_X86(graph, kPhiCleanupPassName, stats) {}

  void Run() OVERRIDE;

 private:
  static constexpr const char* kPhiCleanupPassName = "phi_cleanup";

  /**
    * @brief Checks whether the optimization should be applied.
    * @details In theory, we can apply it to any method. But to save
    *          the compile time, we limit the scope of Phi Cleanup to
    *          methods with loops only.
    * @return true, if Phi Cleanup should be applied.
    */
  bool Gate();

  /**
    * @brief Performs phi cleanup.
    */
  void CleanUpPhis();

  /**
   * @brief Checks whether a Phi has the same instruction on all its inputs.
   * @param phi The Phi to check.
   * @return true, if the Phi has the same instruction on all inputs.
   */
  bool AllInputsSame(HPhi* phi);
};

}  // namespace art

#endif  // ART_COMPILER_OPTIMIZING_EXTENSIONS_PASSES_PHI_CLEANUP_H_
