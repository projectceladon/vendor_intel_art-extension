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

#ifndef ART_OPT_PASSES_COMMUTATIVE_TREES_FLIPPER_H_
#define ART_OPT_PASSES_COMMUTATIVE_TREES_FLIPPER_H_

#include "optimization_x86.h"

namespace art {

/**
 * @brief Flips trees of commutative operations to help the register allocation.
 * @details From the point of view of simple allocators, expression like
 *          (a + (b + (c + d))) may take more registers than (((a + b) + c) + d).
 *          This pass flips the binary operation to ensure the best estimation of
 *          abstract registers needed for calculation of expressions.
 */
class HCommutativeTreesFlipper : public HOptimization_X86 {
 public:
  explicit HCommutativeTreesFlipper(HGraph* graph,
                                    OptimizingCompilerStats* stats = nullptr)
    : HOptimization_X86(graph, kCommutativeTreesFlipperPassName, stats) {}

  static constexpr const char* kCommutativeTreesFlipperPassName = "commutative_trees_flipper";

  void Run() OVERRIDE;

 private:
  /**
   * @brief Estimates how many abstract regs is required for calculation of instruction.
   * @details In our heuristics, we assume that any non-binary operations takes 1 reg,
   *          any binary operation with args (left, right) takes max(cost(left), 1 + cost(right))
   *          abstract regs, because we need to store the result of left when calculating right.
   *          In case if it is profitable to flip operands of a binary operation to reduce its
   *          cost estimation, this function flips them and remembers the optimal cost.
   * @param insn The instruction that we want to estimate.
   * @return The optimal estimated number of abstract registers needed for calculation.
   */
  uint32_t EstimateCost(HInstruction* insn);

  /**
   * @brief Calculates cost of a binary operation with args (left, right)
   *        using formula max(cost(left), 1 + cost(right)).
   * @param left The 1st argument of the binary operation.
   * @param right The 2nd argument of the binary operation.
   * @return The estimation of number of regs for calculation 'as is'.
   */
  uint32_t CalculationCost(HInstruction* left, HInstruction* right);

  // Memorizes pre-calculated costs of binary operations.
  std::unordered_map<HBinaryOperation*, uint32_t> estimated_cost_;
};

}  // namespace art

#endif  // ART_OPT_PASSES_COMMUTATIVE_TREES_FLIPPER_H_
