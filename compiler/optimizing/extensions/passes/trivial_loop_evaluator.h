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

#ifndef ART_COMPILER_OPTIMIZING_TRIVIAL_LOOP_EVALUATOR_H_
#define ART_COMPILER_OPTIMIZING_TRIVIAL_LOOP_EVALUATOR_H_

#include "code_generator.h"
#include "driver/compiler_driver.h"
#include "ext_utility.h"
#include "nodes.h"
#include "optimization_x86.h"

#include <iostream>

namespace art {
// Forward declaration.
class TLEVisitor;

/**
 * @brief Trivial Loop Evaluator is an optimization pass which tries to evaluate the loops of
 * a HGraph when possible. If so, it removes the loop of the graph, writes back the evaluated
 * results and updates their users.
 */
class TrivialLoopEvaluator : public HOptimization_X86 {
  public:
    explicit TrivialLoopEvaluator(HGraph* graph, CompilerDriver* driver,
                                  OptimizingCompilerStats* stats = nullptr)
      : HOptimization_X86(graph, "trivial_loop_evaluator", stats), driver_(driver) {}

    virtual ~TrivialLoopEvaluator() {}

    void Run() OVERRIDE;

  private:
    /**
     * @brief Narrows down the scope of application of TLE to the loops having
     * specific properties.
     * @param loop The HLoopInformation_X86 loop this method will check.
     * @return True if the loop is a valid candidate for TLE, or false otherwise.
     */
    bool LoopGate(HLoopInformation_X86* loop);

    /**
     * @brief Tries to statically evaluate the given loop.
     * @param loop The HLoopInformation_X86 loop this method will try to evaluate
     * statically.
     * @param visitor The structure which will hold all the writes resulting of the
     * evaluation of the loop. It should be empty when calling this method.
     * @return True if TLE successfully evaluated the loop, or false otherwise.
     */
    bool EvaluateLoop(HLoopInformation_X86* loop, TLEVisitor& visitor);

    /**
     * @brief This method deletes all the instructions in the loop's only BB,
     * it replaces them by the constant values previously evaluated by TLE, and deletes
     * the backedge of the given loop.
     * @param loop The HLoopInformation_X86 loop previously evaluated successfully by TLE.
     * @param visitor The result structure holding values we need to write back.
     */
    void UpdateRegisters(HLoopInformation_X86* loop, TLEVisitor& visitor);

    /** Maximum number of iterations in an evaluable loop. Beyond this limits, loops are considered
     * too costly to be statically evaluated. */
    static constexpr int64_t kDefaultLoopEvalMaxIter = 1000;

    const CompilerDriver* driver_;

    /** Copy and assignment are not allowed. */
    DISALLOW_COPY_AND_ASSIGN(TrivialLoopEvaluator);
};

}  // namespace art.

#endif  // ART_COMPILER_OPTIMIZING_TRIVIAL_LOOP_EVALUATOR_H_

