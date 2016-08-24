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

#ifndef ART_OPT_PASSES_BB_SIMPLIFIER_H_
#define ART_OPT_PASSES_BB_SIMPLIFIER_H_

#include "nodes.h"
#include "optimization_x86.h"

namespace art {

/**
 * @brief Simplifies CFG, removing unneeded branching.
 */
class HBBSimplifier : public HOptimization_X86 {
 public:
  explicit HBBSimplifier(HGraph* graph, OptimizingCompilerStats* stats = nullptr)
    : HOptimization_X86(graph, "bb_simplifier", stats) {}

  void Run() OVERRIDE;

 private:
  /**
    * @brief Try to eliminate meaningless diamond starting on the block.
    * @param block The block to simplify.
    * @param if_insn The IF instruction of the diamond.
    * @return True, if the simplification succeeded.
    */
  bool TrySimplifyIf(HBasicBlock* block, HIf* if_insn);

  /**
    * @brief Try to simplify a construction starting in current bb.
    * @param block The block to simplify.
    * @return True, if the simplification succeeded.
    */
  bool TrySimplify(HBasicBlock* block);
};

}  // namespace art

#endif  // ART_OPT_PASSES_BB_SIMPLIFIER_H_
