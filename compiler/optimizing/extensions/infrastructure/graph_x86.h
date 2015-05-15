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

#ifndef ART_OPT_INFRASTRUCTURE_GRAPH_X86_H_
#define ART_OPT_INFRASTRUCTURE_GRAPH_X86_H_

#include "nodes.h"
#include "loop_information.h"
#include "optimizing_compiler_stats.h"

namespace art {

#ifndef NDEBUG
#define GRAPH_MAGIC 0xdeadcafe
#define GRAPH_TO_GRAPH_X86(X) HGraph_X86::DownCast(X)
#else
#define GRAPH_TO_GRAPH_X86(X) static_cast<HGraph_X86*>(X)
#endif

class HGraph_X86 : public HGraph {
 public:
#if 0	//neeraj - resolve build errors
  HGraph_X86(ArenaAllocator* arena, const DexFile& dex_file, uint32_t method_idx,
      bool should_generate_constructor_barrier, InstructionSet instruction_set,
      InvokeType invoke_type = kInvalidInvokeType, bool debuggable = false,
      int start_instruction_id = 0) :
          HGraph(arena, dex_file, method_idx, should_generate_constructor_barrier, 
             instruction_set, invoke_type, debuggable, start_instruction_id),
          loop_information_(nullptr) {
#else
  HGraph_X86(ArenaAllocator* arena, const DexFile& dex_file, uint32_t method_idx,
      InstructionSet instruction_set, InvokeType invoke_type = kInvalidInvokeType, 
      bool debuggable = false, bool osr = false, int start_instruction_id = 0) :
          HGraph(arena, dex_file, method_idx, instruction_set, invoke_type, 
            debuggable, osr, start_instruction_id),
          loop_information_(nullptr) {
#endif
#ifndef NDEBUG
        down_cast_checker_ = GRAPH_MAGIC;
#endif
  }

  HLoopInformation_X86* GetLoopInformation() const {
    return loop_information_;
  }

  void AddLoopInformation(HLoopInformation_X86* information) {
    // Due to the way this method is being called,
    //   the first call is one of the outer loops.
    if (loop_information_ == nullptr) {
      loop_information_ = information;
    } else {
      loop_information_->Add(information);
    }
  }

  /**
   * @brief Delete a block, cleaning up all the loose ends such as
   * successors, predecessors, etc.
   * @param block Block to delete.
   */
  void DeleteBlock(HBasicBlock* block);

  void Dump();

#ifndef NDEBUG
  static HGraph_X86* DownCast(HGraph* graph) {
    HGraph_X86* res = static_cast<HGraph_X86*>(graph);

    if (res != nullptr) {
      DCHECK_EQ(res->down_cast_checker_, GRAPH_MAGIC);
    }
    return res;
  }
#endif

 protected:
#ifndef NDEBUG
  uint32_t down_cast_checker_;
#endif

  HLoopInformation_X86* loop_information_;
};

/**
 * @brief Helper function to create a HGraph_X86 for use in standalone tests.
 * @param allocator ArenaAllocator to use to allocate the HGraph_X86.
 * @returns the newly created HGraph_X86.
 */
inline HGraph_X86* CreateGraph_X86_for_test(ArenaAllocator* allocator) {
#if 0	//neeraj - resolve build errors
  return new (allocator) HGraph_X86(
      allocator, *reinterpret_cast<DexFile*>(allocator->Alloc(sizeof(DexFile))), -1,
      false, kRuntimeISA);
#else
  return new (allocator) HGraph_X86(allocator, *reinterpret_cast<DexFile*>(allocator->Alloc(sizeof(DexFile))), -1, kRuntimeISA);
#endif
}
}  // namespace art

#endif  // ART_OPT_INFRASTRUCTURE_GRAPH_X86_H_
