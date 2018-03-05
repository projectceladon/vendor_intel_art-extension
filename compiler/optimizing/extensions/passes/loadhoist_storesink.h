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
#ifndef ART_COMPILER_OPTIMIZING_LOADHOIST_STORESINK_H_
#define ART_COMPILER_OPTIMIZING_LOADHOIST_STORESINK_H_

#include "ext_alias.h"
#include "optimization_x86.h"
#include "graph_x86.h"

namespace art {

typedef std::map<HInstruction*, HInstruction*> GetSetToHoistSink;
typedef std::set<HInstruction*> SetsToSink;

class LoadHoistStoreSink : public HOptimization_X86 {
 public:
  explicit LoadHoistStoreSink(HGraph* graph, OptimizingCompilerStats* stats = nullptr)
      : HOptimization_X86(graph, kLoadHoistStoreSinkPassName, stats) {}

  void Run() OVERRIDE;

 private:
  static constexpr const char* kLoadHoistStoreSinkPassName = "loadhoist_storesink";

  /*
   * @brief Find load/store matches and check for dangerous instructions.
   * @param loop Loop information for current loop.
   * @param get_to_set matching get/sets are added to this parameter.
   * @param sets_to_sink unmatched candidate sets are added to this parameter.
   * @return 'true' if no dangerous instructions were found in the loop.
   */
  bool FindLoadStoreCouples(HLoopInformation_X86* loop,
                            GetSetToHoistSink& get_to_set,
                            SetsToSink& sets_to_sink);

  bool LoopGate(HLoopInformation_X86* loop) const;

  /*
   * @brief Find load/store matches and check for dangerous instructions.
   * @param loop Loop information for current loop.
   * @param get_to_set matching get/sets to be hoisted/sunk.
   * @param sets_to_sink unmatched sets to be sunk.
   * @return 'true' if at least one instruction was added to the suspend block.
   */
  bool DoLoadHoistStoreSink(HLoopInformation_X86* loop,
                            GetSetToHoistSink& get_to_set,
                            SetsToSink& sets_to_sink);


  AliasCheck alias_;

  DISALLOW_COPY_AND_ASSIGN(LoadHoistStoreSink);
};

}  // namespace art

#endif  // ART_COMPILER_OPTIMIZING_LOADHOIST_STORESINK_H_
