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

#ifndef ART_OPT_INFRASTRUCTURE_OPTIMIZATION_X86_H_
#define ART_OPT_INFRASTRUCTURE_OPTIMIZATION_X86_H_

#include "graph_x86.h"
#include "optimization.h"

namespace art {

/**
 * @class HOptimization_X86
 * @brief Abstraction to implement an Intel-specific optimization pass.
 */
class HOptimization_X86 : public HOptimization {
 public:
  HOptimization_X86(HGraph* graph,
                    const char* pass_name,
                    OptimizingCompilerStats* stats = nullptr,
                    bool verbose = false) :
    HOptimization(graph, pass_name, stats), verbose_(verbose) {}

  /**
   * @brief Is this pass verbose?
   * @return true, if this optimization pass is verbose.
   */
  bool IsVerbose() const {
    return verbose_;
  }

  /**
   * @brief Sets verbosity of the pass.
   * @param verbose the new verbosity.
   */
  void SetVerbose(bool verbose) {
    verbose_ = verbose;
  }

  ALWAYS_INLINE HGraph_X86* GetGraphX86() const {
    return GRAPH_TO_GRAPH_X86(graph_);
  }

 private:
  bool verbose_;
};

}  // namespace art

#endif  // ART_OPT_INFRASTRUCTURE_OPTIMIZATION_X86_H_
