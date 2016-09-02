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
#ifndef ART_COMPILER_OPTIMIZING_NON_TEMPORAL_MOVE_H_
#define ART_COMPILER_OPTIMIZING_NON_TEMPORAL_MOVE_H_

#include "driver/compiler_driver.h"
#include "optimization_x86.h"

namespace art {

class DexCompilationUnit;

class HNonTemporalMove : public HOptimization_X86 {
 public:
  explicit HNonTemporalMove(HGraph* graph, CompilerDriver* driver,
                            OptimizingCompilerStats* stats = nullptr)
      : HOptimization_X86(graph, kNonTemporalMovePassName, stats), driver_(driver) {}

  void Run() OVERRIDE;

 private:
  static constexpr const char* kNonTemporalMovePassName = "non_temporal_move";

  static constexpr int kDefaultMinNonTemporalIterations = 131072;

  typedef std::set<HArraySet*> ArraySets;

  /*
   * @brief Is this loop a candidate for non-temporal-move replacement?
   * @param loop_info Candidate loop information.
   * @param array_sets ArraySet instructions found in the loop are added to array_sets.
   * @returns 'true' if the loop should use non-temporal moves.
   */
  bool Gate(HLoopInformation_X86* loop_info, ArraySets& array_sets) const;

  const CompilerDriver* const driver_;

  DISALLOW_COPY_AND_ASSIGN(HNonTemporalMove);
};

}  // namespace art

#endif  // ART_COMPILER_OPTIMIZING_NON_TEMPORAL_MOVE_H_
