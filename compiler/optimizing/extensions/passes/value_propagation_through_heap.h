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
#ifndef ART_COMPILER_OPTIMIZING_VALUE_PROPAGATION_THROUGHHEAP_H_
#define ART_COMPILER_OPTIMIZING_VALUE_PROPAGATION_THROUGHHEAP_H_

#include "ext_alias.h"
#include "nodes.h"
#include "optimization_x86.h"

#include <set>

namespace art {

class HValuePropagationThroughHeap : public HOptimization_X86 {
 public:
  explicit HValuePropagationThroughHeap(HGraph* graph, OptimizingCompilerStats* stats)
      : HOptimization_X86(graph, kValuePropagationThroughHeap, stats) {}

  void Run() OVERRIDE;

 private:
  static constexpr const char* kValuePropagationThroughHeap = "value_propagation_through_heap";

 /*
  * @brief Check if loop instructions is safe for value propagation through heap optimization.
  * @param loop the loop information.
  * @return true true if loop is valid candidate for this optimization, false otherwise.
  */
  bool Gate(HLoopInformation_X86* loop) const;

  /*
   * @brief Get candidate setter instructions where invariant value is stored in the loop preheader.
   * @param loop the loop information.
   * @param setters_set the set that stores the candidate setter instructions.
   * @return return true when candidates setter found.
   */
  bool GetCandidateSetters(HLoopInformation_X86* loop, std::set<HInstruction*>& setters_set);


  /*
   * @brief Propagate values in candidate setters to the corresponding getter instructions.
   * @param loop the loop information.
   * @param setters_set the set that stores the candidate setter instructions.
   */
  void PropagateValueToGetters(HLoopInformation_X86* loop, std::set<HInstruction*> setters__set);

  AliasCheck alias_;

  // The maximum basic block numbers in the loop for this optimization to apply
  // to save compilation time.
  static constexpr int64_t kMaximumBasicBlockNumbers = 11;

  DISALLOW_COPY_AND_ASSIGN(HValuePropagationThroughHeap);
};

}  // namespace art

#endif  // ART_COMPILER_OPTIMIZING_VALUE_PROPAGATION_THROUGH_HEAP_H_

