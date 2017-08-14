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

#ifndef ART_OPT_INFRASTRUCTURE_LOOP_INFORMATION_INSIDE_H_
#define ART_OPT_INFRASTRUCTURE_LOOP_INFORMATION_INSIDE_H_

/*
 * This is done without namespace in order to be included
 * both in loop_information.h and nodes.h.
 */

#ifndef NDEBUG
#define LOOPINFO_MAGIC 0xdeadcafe
#define LOOPINFO_TO_LOOPINFO_X86(X) HLoopInformation_X86::DownCast(X)
#else
#define LOOPINFO_TO_LOOPINFO_X86(X) static_cast<HLoopInformation_X86*>(X)
#endif

// Forward declaration.
class HInductionVariable;

class HLoopInformation_X86 : public HLoopInformation {
 public:
  HLoopInformation_X86(HBasicBlock* block, HGraph* graph) :
      HLoopInformation(block, graph),
      depth_(0), count_up_(false),
      outer_(nullptr), sibling_previous_(nullptr),
      sibling_next_(nullptr), inner_(nullptr),
      test_suspend_(nullptr), suspend_(nullptr),
      iv_list_(graph->GetArena()->Adapter(kArenaAllocMisc)),
      inter_iteration_variables_(graph->GetArena()->Adapter(kArenaAllocMisc)) {
#ifndef NDEBUG
        down_cast_checker_ = LOOPINFO_MAGIC;
#endif
  }

  /**
   * @brief Clear back edges.
   */
  void ClearBackEdges() {
    back_edges_.clear();
  }

  /**
   * @brief Get next sibling loop.
   * @return the next sibling loop.
   */
  HLoopInformation_X86* GetNextSibling() const {
    return sibling_next_;
  }

  /**
   * @brief Get previous sibling loop.
   * @return the previous sibling loop.
   */
  HLoopInformation_X86* GetPrevSibling() const {
    return sibling_previous_;
  }

  /**
   * @brief Get previous sibling loop.
   * @return the previous sibling loop.
   */
  HLoopInformation_X86* GetInner() const {
    return inner_;
  }

  /**
   * @brief Get previous sibling loop.
   * @return the previous sibling loop.
   */
  HLoopInformation_X86* GetParent() const {
    return outer_;
  }

  /**
   * @brief Add the parameter in the hierarchy or it is a sibling loop.
   * @param other the other HLoopInformation.
   */
  void Add(HLoopInformation_X86* other);

  /**
   * @brief Dump the loop hierarchy.
   * @param max_depth the maximum depth for the printout (default: 10).
   */
  void Dump(int max_depth = 10) const;

  /**
   * @brief Get the number of blocks.
   * @return the number of blocks in the HLoopInformation_X86.
   */
  int NumberOfBlocks() const {
    return blocks_.NumSetBits();
  }

  /**
   * @brief Set the depth for each HLoopInformation_X86.
   * @param depth the current depth (default 0).
   */
  void SetDepth(int depth = 0);

  /**
   * @brief Get Depth.
   * @return the depth of the loop.
   */
  int GetDepth() const {
    return depth_;
  }

  /**
   * @brief Is this the inner loop?
   * @return whether or not this loop is the inner loop?
   */
  bool IsInner() const {
    return inner_ == nullptr;
  }

  /**
    * @brief Get the loop basic induction variable.
    * @return the basic induction variable.
    */
  HInductionVariable* GetBasicIV() const {
    return bound_info_.loop_biv_;
  }

  /**
   * @brief Get the induction variables.
   * @return the induction variables.
   */
  ArenaVector<HInductionVariable*>& GetInductionVariables() {
    return iv_list_;
  }

  /**
   * @brief Get the induction variable using the instruction.
   * @param insn the HInstruction we care about, it can be the linear or phi.
   * @return the induction variable associated to the register.
   */
  HInductionVariable* GetInductionVariable(HInstruction* insn) const;

  /**
   * @brief Clear inter iteration variables.
   */
  void ClearInterIterationVariables() {
    inter_iteration_variables_.clear();
  }

  /**
   * @brief Get the inter-iteration variables.
   * @return the set of inter-iteration variables.
   */
  const ArenaSet<int>& GetInterIterationVariables() const {
    return inter_iteration_variables_;
  }

  /**
   * @brief Add a variable as an inter-iteration variable.
   * @param reg the register.
   */
  void AddInterIterationVariable(int reg) {
    // inter_iteration_variables_ is a set, inserting it does not
    //   mean it really gets inserted if it is already there.
    inter_iteration_variables_.insert(reg);
  }

  /**
   * @brief Set the count up loop boolean.
   */
  void SetCountUpLoop(bool b) {
    count_up_ = b;
  }

  /**
   * @brief Get the count up loop boolean.
   * @return whether or not the HLoopInformation_X86 is count up.
   */
  bool IsCountUp() const {
    return count_up_;
  }

  /**
   * @brief Is the basic block executed every iteration?
   * @param bb the basic block.
   * @return whether or not the block is executed every iteration.
   */
  bool ExecutedPerIteration(HBasicBlock* bb) const;

  /**
   * @brief Is the instruction executed every iteration?
   * @param candidate the HInstruction.
   * @return whether or not the instruction is executed every iteration.
   */
  bool ExecutedPerIteration(HInstruction* candidate) const;

  /**
   * @brief Does the loop only have one exit block?
   * @return whether or not the loop has one single exit block.
   */
  bool HasOneExitBlock() const;

  /**
   * @brief Get the exit block if there is only one.
   * @return the exit block, nullptr if more than one.
   */
  HBasicBlock* GetExitBlock() const;

  /**
   * @brief Get the number of iterations of given basic block.
   *        Before calling this method, ensure that bb is
   *        ExecutedPerIteration.
   * @param bb The basic block.
   * @return the number of iterations, -1 if unknown.
   */
  int64_t GetNumIterations(HBasicBlock* bb) const;

  /**
   * @brief Do we know the number of iterations?
   * @return whether we know the number of iterations.
   */
  bool HasKnownNumIterations() const {
    return (bound_info_.num_iterations_ != -1);
  }

  /**
   * @brief Split a SuspendCheck into a TestSuspend and an Suspend.
   */
  void SplitSuspendCheck();

  /**
   * @brief Insert an HInstruction before the call to Suspend.  Will
   * call SplitSuspendCheck if there isn't already a split suspend.
   * @param instruction HInstruction to insert just before call to Suspend.
   * @note RebuildDominators() must be called before an optimization that uses
   * this method is returns.
   */
  void InsertInstructionInSuspendBlock(HInstruction* instruction);

  /**
   * @brief Add a block to all nested loops, and set the loop_information
   * for the block to 'this'.
   * @param block Block to add to loop nest.
   */
  void AddToAll(HBasicBlock* block);

  /**
   * @brief Return a pointer to the HSuspend instruction for this loop, if present.
   * @returns HSuspend instruction, or nullptr.
   */
  HSuspend* GetSuspend() const { return suspend_; }

  /**
   * @brief Set the pointer to the HSuspend instruction for this loop.
   * @param suspend HSuspend instruction for the loop.
   */
  void SetSuspend(HSuspend* suspend) { suspend_ = suspend; }

  /**
   * @brief Does this loop have an HSuspend instruction?
   * @returns 'true' if there is an HSuspend instruction present in the loop.
   */
  bool HasSuspend() const { return suspend_ != nullptr; }

  /**
   * @brief Return a pointer to the HTestSuspend instruction for this loop, if present.
   * @returns HTestSuspend instruction, or nullptr.
   */
  HTestSuspend* GetTestSuspend() const { return test_suspend_; }

  /**
   * @brief Set the pointer to the HTestSuspend instruction for this loop.
   * @param test_suspend HTestSuspend instruction for the loop.
   */
  void SetTestSuspend(HTestSuspend* test_suspend) { test_suspend_ = test_suspend; }

  /**
   * @brief Does this loop have an HTestSuspend instruction?
   * @returns 'true' if there is an HTestSuspend instruction present in the loop.
   */
  bool HasTestSuspend() const { return test_suspend_ != nullptr; }

  /**
   * @brief Does this loop need to have a HSuspendCheck added?
   * @returns 'true' if a SuspendCheck should not be added.
   * @note If we have converted to HTestSuspend/HSuspend, we don't want
   * another HSuspendCheck to be added to the loop.
   */
  bool DontAddSuspendCheck() const OVERRIDE { return HasTestSuspend(); }

#ifndef NDEBUG
  static HLoopInformation_X86* DownCast(HLoopInformation* info) {
    HLoopInformation_X86* res = static_cast<HLoopInformation_X86*>(info);

    if (res != nullptr) {
      DCHECK_EQ(res->down_cast_checker_, LOOPINFO_MAGIC);
    }
    return res;
  }
#endif

  /**
   * @brief Compute the bound information.
   */
  bool ComputeBoundInformation();

  /**
   * @brief Get the HBoundInformation.
   * @return the bound information.
   */
  const HLoopBoundInformation& GetBoundInformation() const {
    return bound_info_;
  }

  /*
   * @brief Ensure that the links from this HLoopInformation_X86 are reset.
   */
  void ResetLinks() {
    sibling_next_ = nullptr;
    sibling_previous_ = nullptr;
    inner_ = nullptr;
    outer_ = nullptr;
  }

 protected:
  /**
   * @brief Find the constant entry SSA associated to the Phi instruction.
   * @param phi the HPhi instruction.
   * @return the constant that is the entry point to the Phi, nullptr otherwise.
   */
  HConstant* FindBIVEntrySSA(HPhi* phi) const;

  /**
   * @brief Fill the floating-point bound information.
   * @param entry_value the constant definining the loop's start value.
   * @param is_double are we dealing with 64-bit value.
   * @return whether or not the information was filled.
   */
  bool FillFloatingPointBound(HConstant* entry_value, bool is_double);

#ifndef NDEBUG
  uint32_t down_cast_checker_;
#endif

  int depth_;
  bool count_up_;

  HLoopInformation_X86* outer_;
  HLoopInformation_X86* sibling_previous_;
  HLoopInformation_X86* sibling_next_;
  HLoopInformation_X86* inner_;

  /** @brief The bound information. */
  HLoopBoundInformation bound_info_;

  /** @brief Pointer to the split HTestSuspend, if present. */
  HTestSuspend* test_suspend_;

  /** @brief Pointer to the split HSuspend, if present. */
  HSuspend* suspend_;

  /** @brief The Induction Variable list. */
  ArenaVector<HInductionVariable*> iv_list_;

  /** @brief Inter-iteration dependent variables. */
  ArenaSet<int> inter_iteration_variables_;
};

#endif  // ART_OPT_INFRASTRUCTURE_LOOP_INFORMATION_INSIDE_H_
