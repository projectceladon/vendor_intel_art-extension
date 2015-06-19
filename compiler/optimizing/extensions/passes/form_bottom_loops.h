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
#ifndef ART_COMPILER_OPTIMIZING_FORM_BOTTOM_LOOPS_H_
#define ART_COMPILER_OPTIMIZING_FORM_BOTTOM_LOOPS_H_

#include "driver/dex_compilation_unit.h"
#include "nodes.h"
#include "optimization_x86.h"

namespace art {

class HInstructionCloner;

class HFormBottomLoops : public HOptimization_X86 {
 public:
  explicit HFormBottomLoops(HGraph* graph, OptimizingCompilerStats* stats = nullptr)
      : HOptimization_X86(graph, kFormBottomLoopsPassName, stats) {}

  void Run() OVERRIDE;

 private:
  static constexpr const char* kFormBottomLoopsPassName = "form_bottom_loops";

  // Mapping from Environment to instruction containing the environment.
  typedef SafeMap<HEnvironment*, HInstruction*> EnvToInstrMap;

  /**
   * @brief Should this loop be rewritten as a bottom tested loop?
   * @param loop_info The current loop.
   * @param loop_header The top of the loop.
   * @param exit_block The exit from the loop.
   * @returns 'true' if the loop should be rewritten.
   */
  bool ShouldTransformLoop(HLoopInformation_X86* loop_info,
                           HBasicBlock* loop_header,
                           HBasicBlock* exit_block);

  /**
   * @brief Is the loop header block safe to rewrite?
   * @param loop_info The current loop.
   * @param loop_block The top of the loop.
   * @returns 'true' if all instructions in the loop are safe to rewrite.
   */
  bool CheckLoopHeader(HLoopInformation_X86* loop_info, HBasicBlock* loop_block);

  /**
   * @brief Rewrite the loop as a bottom tested loop.
   * @param loop_info The current loop.
   * @param loop_header The top of the loop.
   * @param exit_block The exit from the loop.
   */
  void RewriteLoop(HLoopInformation_X86* loop_info,
                   HBasicBlock* loop_header,
                   HBasicBlock* exit_block);

  /**
   * @brief Fix up any references to the old loop header Phis that are used
   * in a following instruction.
   * @param loop_info The current loop.
   * @param first_block The future loop header.
   * @param exit_block The block in which to insert a new Phi.
   * @param phi The phi to fix up.
   * @param computed_in_header Whether our Phi node is computed in header.
   * @param outside_value The phi input that comes from outside the loop.
   * @param inside_value The phi input that comes from inside the loop.
   * @param exit_phis set of Phis created in the exit block.
   */
  void FixPhiUses(HLoopInformation_X86* loop_info,
                  HBasicBlock* first_block,
                  HBasicBlock* exit_block,
                  HPhi* phi,
                  bool computed_in_header,
                  HInstruction* outside_value,
                  HInstruction* inside_value,
                  std::set<HPhi*>& exit_phis);

  std::set<HInstruction*> header_values_used_in_exit_block;

  DISALLOW_COPY_AND_ASSIGN(HFormBottomLoops);
};

}  // namespace art

#endif  // ART_COMPILER_OPTIMIZING_FORM_BOTTOM_LOOPS_H_
