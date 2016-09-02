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
#ifndef ART_COMPILER_OPTIMIZING_REMOVE_UNUSED_LOOPS_H_
#define ART_COMPILER_OPTIMIZING_REMOVE_UNUSED_LOOPS_H_

#include "optimization_x86.h"

namespace art {

class HRemoveUnusedLoops : public HOptimization_X86 {
 public:
  explicit HRemoveUnusedLoops(HGraph* graph, OptimizingCompilerStats* stats = nullptr)
      : HOptimization_X86(graph, kRemoveUnusedLoopsPassName, stats) {}

  void Run() OVERRIDE;

 private:
  static constexpr const char* kRemoveUnusedLoopsPassName = "remove_unused_loops";

  bool CheckInstructionsInBlock(HLoopInformation_X86* loop_info, HBasicBlock* loop_block);
  bool CheckPhisInBlock(HLoopInformation_X86* loop_info, HBasicBlock* loop_block);
  void RemoveLoop(HLoopInformation_X86* loop_info,
                  HBasicBlock*pre_header,
                  HBasicBlock* exit_block);
  void UpdateExternalPhis();

  std::set<HPhi*> external_loop_phis_;

  DISALLOW_COPY_AND_ASSIGN(HRemoveUnusedLoops);
};

}  // namespace art

#endif  // ART_COMPILER_OPTIMIZING_REMOVE_UNUSED_LOOPS_H_
