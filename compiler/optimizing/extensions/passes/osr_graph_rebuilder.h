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

#ifndef ART_OPT_PASSES_OSR_GRAPH_REBUILDER_H_
#define ART_OPT_PASSES_OSR_GRAPH_REBUILDER_H_

#include <vector>

#include "nodes.h"
#include "optimization_x86.h"

namespace art {

/**
 * @brief Transforms loops in graph to allow more optimizations in OSR mode.
 */
class HOsrGraphRebuilder : public HOptimization_X86 {
 public:
  explicit HOsrGraphRebuilder(HGraph* graph, OptimizingCompilerStats* stats = nullptr)
    : HOptimization_X86(graph, "osr_graph_rebuilder", stats),
      fixable_loops_(graph->GetArena()->Adapter()) {}

  void Run() OVERRIDE;

 private:
  /**
   * @brief Collects the loops that can be prepared for OSR.
   * @return True, if at least one valid loop is found.
   */
  bool Gate();

  /**
   * @brief Prepares graph to allow more optimizations.
   */
  void DoOsrPreparation();

  /**
   * @brief Transforms CFG, creating blocks for OSR and normal path.
   *        Returns newly-created blocks in params.
   * @param osr_fork contains one fictive branching between OSR and normal paths.
   * @param normal_path is taken in normal execution, contains parameters.
   * @param osr_path is fictively taken in case of OSR.
   */
  void TransformCfg(HBasicBlock*& osr_fork,
                    HBasicBlock*& normal_path,
                    HBasicBlock*& osr_path);

  /**
    * @brief Moves params and method entry suspend check from entry block
    *        to normal path block.
    * @param entry_block The graph entry block.
    * @param normal_path The newly-created normal path block.
    */
  void MoveParams(HBasicBlock* entry_block,
                  HBasicBlock* normal_path);

  /**
    * @brief Injects edges from osr_path to all found fixable loops.
    * @param osr_path The OSR path block.
    */
  void InjectOsrIntoLoops(HBasicBlock* osr_path);

  /**
    * @brief Performs Phi node fixup after OSR path injection.
    * @param loop The loop to be fixed.
    * @param osr_path The OSR path block.
    * @param new_pre_header The loop pre-header.
    */
  void InjectionPhiFixup(HLoopInformation_X86* loop,
                         HBasicBlock* osr_path,
                         HBasicBlock* new_pre_header);

  // Used to collect the loops that can be prepared for OSR.
  ArenaVector<HLoopInformation_X86*> fixable_loops_;
};

}  // namespace art

#endif  // ART_OPT_PASSES_OSR_GRAPH_REBUILDER_H_
