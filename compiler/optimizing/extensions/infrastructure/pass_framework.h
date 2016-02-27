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

#ifndef ART_OPT_INFRASTRUCTURE_PASS_FRAMEWORK_H_
#define ART_OPT_INFRASTRUCTURE_PASS_FRAMEWORK_H_

#include "nodes.h"

namespace art {

// Forward declarations.
class HOptimization;
class PassObserver;

/**
 * @brief Print the passes for debugging information.
 * @param opts the optimization vector.
 * @param post_opts the post-optimization vectors.
 */
void PrintPasses(ArenaVector<HOptimization*>& opts,
                 ArenaVector<HOptimization*>& post_opts);

/**
 * @brief Print the passes for debugging information, if their
 *        printing is enabled by --print-pass-names option and
 *        they have not been printed by this method before.
 * @param opts the optimization vector.
 * @param post_opts the post-optimization vectors.
 * @param driver the compilation driver.
 * @return true, if the passes have been printed by this method call.
 *         false, if the pass printing is not enabled by the option
 *                or they have already been printed before.
 */
bool PrintPassesOnlyOnce(ArenaVector<HOptimization*>& opts,
                         ArenaVector<HOptimization*>& post_opts,
                         CompilerDriver* driver);

void RunOptimizationsX86(HGraph* graph,
                         CompilerDriver* driver,
                         OptimizingCompilerStats* stats,
                         ArenaVector<HOptimization*>& opt_list,
                         const DexCompilationUnit& dex_compilation_unit,
                         PassObserver* pass_observer,
                         StackHandleScopeCollection* handles);

class RunOptWithPassScope {
 public:
  RunOptWithPassScope(HOptimization* opt, PassObserver* pass_observer)
      : opt_(opt),
        pass_observer_(pass_observer) { }

  void Run();

 private:
  HOptimization* opt_;
  PassObserver* const pass_observer_;
};

}  // namespace art

#endif  // ART_OPT_INFRASTRUCTURE_GRAPH_X86_H_
