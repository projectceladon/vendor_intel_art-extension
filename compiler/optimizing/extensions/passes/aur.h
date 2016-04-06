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

#ifndef ART_COMPILER_OPTIMIZING_EXTENSIONS_PASSES_AUR_H_
#define ART_COMPILER_OPTIMIZING_EXTENSIONS_PASSES_AUR_H_

#include "nodes.h"
#include "optimization_x86.h"

namespace art {

// Forward declarations.
class CompilerDriver;

/**
 * @brief This is an optimization pass that cleans up environments in non-debuggable mode.
 * @details Environments currently keep a list of live dex virtual registers which are
 * potentially live into runtime. This makes it possible to transition to interpreter
 * at any point there is interaction with runtime. However, for non-debuggable applications,
 * that is not needed unless it is an explicit deoptimize.
 */
class HAggressiveUseRemoverPass : public HOptimization_X86 {
 public:
  HAggressiveUseRemoverPass(HGraph* graph,
                            OptimizingCompilerStats* stats = nullptr)
    : HOptimization_X86(graph, "aur", stats) {}

  void Run() OVERRIDE;
};

}  // namespace art

#endif  // ART_COMPILER_OPTIMIZING_EXTENSIONS_PASSES_AUR_H_
