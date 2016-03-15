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
  HGraph_X86(ArenaAllocator* arena, const DexFile& dex_file, uint32_t method_idx,
      bool should_generate_constructor_barrier, InstructionSet instruction_set,
      InvokeType invoke_type = kInvalidInvokeType, bool debuggable = false,
      bool osr = false, int start_instruction_id = 0) :
          HGraph(arena, dex_file, method_idx, should_generate_constructor_barrier,
          instruction_set, invoke_type, debuggable, osr, start_instruction_id),
          loop_information_(nullptr),
          profiled_invokes_(arena->Adapter()) {
#ifndef NDEBUG
        down_cast_checker_ = GRAPH_MAGIC;
#endif
  }

  /**
   * @brief Return the graph's loop information.
   * @return the outermost loop in the graph.
   */
  HLoopInformation_X86* GetLoopInformation() const {
    return loop_information_;
  }

  void SetLoopInformation(HLoopInformation_X86* loop) {
    loop_information_ = loop;
  }

  /**
   * @brief Clear the loop information.
   */
  void ClearLoopInformation() {
    loop_information_ = nullptr;
  }

  /**
   * @brief Add a loop to the graph.
   * @param information The loop information to be added.
   */
  void AddLoopInformation(HLoopInformation_X86* information) {
    // Due to the way this method is being called,
    //   the first call is one of the outer loops.

    // Ensure we are only adding a single loop in.
    information->ResetLinks();
    if (loop_information_ == nullptr) {
      loop_information_ = information;
    } else {
      loop_information_->Add(information);
    }
  }

  /**
   * @brief Delete a block, cleaning up all the loose ends such as
   * successors, predecessors, etc.
   * @param block The HBasicBlock to delete from the graph.
   */
  void DeleteBlock(HBasicBlock* block);

  /**
   * @brief Used to update the links between two blocks when one is added
   * that follows an existing one unconditionally.
   * @details Updates successor/predecessor info, updates domination information,
   * and maintains the ordering information.
   * @param existing_block The block that is already in the graph.
   * @param block_being_added The new block being added to follow existing_block.
   * @param add_as_dominator Whether the block being added is dominated by the exist block.
   * @param add_after Whether the block being added is before or after. When false, it means it
   * is being added as a predecessor.
   */
  void CreateLinkBetweenBlocks(HBasicBlock* existing_block,
                               HBasicBlock* block_being_added,
                               bool add_as_dominator,
                               bool add_after);

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

  /**
   * @brief Used to create a new basic that is added to graph.
   * @param dex_pc The dex pc of this block (optional).
   * @return Returns the newly created block.
   */
  HBasicBlock* CreateNewBasicBlock(uint32_t dex_pc = kNoDexPc) {
    HBasicBlock* new_block = new (arena_) HBasicBlock(this, dex_pc);
    AddBlock(new_block);
    return new_block;
  }

  /**
   * @brief Split critial edge and set loop information splitter.
   * @param from input of new critical edge.
   * @param to output of new critical edge.
   */
  void SplitCriticalEdgeAndUpdateLoopInformation(HBasicBlock* from, HBasicBlock* to);

  /**
   * @brief Called after an optimization pass in order to rebuild domination
   * information and ordering.
   * @details This also has effect of cleaning up graph and normalizing loops
   * (depending on what Google overloaded it to do).
   */
  void RebuildDomination();

  /*
   * @brief Move a Phi from one block to another block.
   * @param phi Phi to move.
   * @param to_block Block to which to move the Phi.
   */
  void MovePhi(HPhi* phi, HBasicBlock* to_block);

  /*
   * @brief Move an from one block to another block.
   * @param instr Instruction to move.
   * @param cursor The target instruction.
   * @note The instruction to move must be in a different block from the cursor.
   */
  void MoveInstructionBefore(HInstruction* instr, HInstruction* cursor);

  /**
   * @brief How many blocks have exact profiling block increments?
   * @return Returns The number of blocks with exact profile bumps.
   */
  int GetNumProfiledBlocks() const {
    return num_profiled_blocks_;
  }

  /**
   * @brief Set the number of blocks with profiling increments.
   * @param val Number of blocks.
   */
  void SetNumProfiledBlocks(int val) {
    num_profiled_blocks_ = val;
  }

  /**
   * @brief Are there any saved invoke dex pcs?
   * @ereturns 'true' if there are virtual/interface invokes in the method.
   */
  bool HasProfiledInvokesDexPcs() const {
    return !profiled_invokes_.empty();
  }

  /**
   * @brief Return the saved invoke dex pcs list.
   * @returns the vector of saved virtual/interface invokes.  Will create the
   * list if not already created.
   */
  ArenaVector<uint16_t>& GetProfiledInvokesDexPcs() {
    return profiled_invokes_;
  }

 protected:
#ifndef NDEBUG
  uint32_t down_cast_checker_;
#endif

  HLoopInformation_X86* loop_information_;

  /**
   * @brief Used to record the maximum block number that contains exact profiling.
   */
  int num_profiled_blocks_ = 0;

  /**
   * @brief Used to record the dex_pcs for virtual/interface invoke calls.
   */
  ArenaVector<uint16_t> profiled_invokes_;
};

/**
 * @brief Helper function to create a control flow graph using HGraph_X86 for
 * use in standalone tests.
 * @param allocator ArenaAllocator to use to allocate the HGraph_X86.
 * @returns the newly created HGraph_X86.
 */
HGraph_X86* CreateX86CFG(ArenaAllocator* allocator,
                         const uint16_t* data,
                         Primitive::Type return_type = Primitive::kPrimInt);
}  // namespace art

#endif  // ART_OPT_INFRASTRUCTURE_GRAPH_X86_H_
