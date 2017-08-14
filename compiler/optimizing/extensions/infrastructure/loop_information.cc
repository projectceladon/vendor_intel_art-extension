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

#include "ext_utility.h"
#include "induction_variable.h"
#include "loop_information.h"

namespace art {

/**
 * @details Add takes a new LoopInformation and determines if info is nested_ with
 *   this instance or not.
 * If it is nested_ in this instance, we fill our nested_ information with it
 * Otherwise, we are a sibling, this is due to how Add is being called,
 *  it supposes that info has less or equal number of BasicBlocks.
 */
void HLoopInformation_X86::Add(HLoopInformation_X86* other) {
  // Check if there is anything to do.
  if (other == nullptr) {
    return;
  }

  // Clear siblings and nested in case.
  other->sibling_next_ = nullptr;
  other->sibling_previous_ = nullptr;
  other->outer_ = nullptr;

  // Do we include the current loop?
  if (other->IsIn(*this)) {
    // We contain them, so they should not contain us.
    DCHECK(!IsIn(*other));

    // Search in the children if anybody includes them.
    if (inner_ == nullptr) {
      inner_ = other;
      inner_->outer_ = this;
    } else {
      // Continue down the link.
      inner_->Add(other);
    }
  } else {
    if (sibling_next_ == nullptr) {
      other->sibling_previous_ = this;
      sibling_next_ = other;
      other->outer_ = outer_;
    } else {
      sibling_next_->Add(other);
    }
  }
}

void HLoopInformation_X86::SetDepth(int depth) {
  HLoopInformation_X86* info = this;
  while (info != nullptr) {
    info->depth_ = depth;

    if (info->inner_ != nullptr) {
      info->inner_->SetDepth(depth + 1);
    }
    info = info->sibling_next_;
  }
}

void HLoopInformation_X86::Dump(int max_depth) const {
  char buffer[256];
  char* ptr = buffer;
  int i = 0;

  for (i = 0; i < depth_ && i < max_depth; i++) {
    *ptr = '\t';
    ptr++;
  }

  snprintf(ptr, sizeof(buffer) - i, "%p: Loop depth %d with header %d, # blocks %d, contains: (inner %p, sibling %p)",
      this, depth_, GetHeader()->GetBlockId(), NumberOfBlocks(), inner_, sibling_next_);
  LOG(INFO) << buffer;

  if (inner_ != nullptr) {
    inner_->Dump();
  }

  if (sibling_next_ != nullptr) {
    sibling_next_->Dump();
  }
}

bool HLoopInformation_X86::ExecutedPerIteration(HBasicBlock* bb) const {
  DCHECK(bb != nullptr);
  // First is the instruction in the loop?
  if (!Contains(*bb)) {
    return false;
  }

  for (HBasicBlock* back_edge : GetBackEdges()) {
    if (!bb->Dominates(back_edge)) {
      return false;
    }
  }

  return true;
}

bool HLoopInformation_X86::ExecutedPerIteration(HInstruction* candidate) const {
  if (candidate == nullptr) {
    return false;
  }

  return ExecutedPerIteration(candidate->GetBlock());
}

bool HLoopInformation_X86::ComputeBoundInformation() {
  // Always start off with reinitializing the bound information.
  bound_info_ = HLoopBoundInformation();

  // Check that the loop has only one exit.
  HBasicBlock* exit_block = GetExitBlock();
  if (exit_block == nullptr) {
    return false;
  }

  // Check that we exit by IF instruction.
  HInstruction* branch = exit_block->GetPredecessors()[0]->GetLastInstruction();
  if (!branch->IsIf()) {
    return false;
  }

  // Go from the if to the compare.
  HInstruction* insn_to_check = branch->InputAt(0);

  // We want a condition here.
  if (!insn_to_check->IsCondition()) {
    return false;
  }

  IfCondition comparison_condition = insn_to_check->AsCondition()->GetCondition();
  bool is_fp = false;
  bool is_wide = false;

  // This could be a comparison on the result of a Compare instruction.
  insn_to_check = GetCompareInstruction(insn_to_check);

  // Get the first element of the compare.
  HInstruction* first_element = insn_to_check->InputAt(0);
  DCHECK(first_element != nullptr);

  // Skip the type conversion if any. It is safe to do so because if we find
  // an IV, we have checked the bounds of the conversion already.
  if (first_element->IsTypeConversion()) {
    first_element = first_element->InputAt(0);
  }

  // Get its type.
  Primitive::Type type = first_element->GetType();

  switch (type) {
    case Primitive::kPrimDouble:
      is_wide = true;
      FALLTHROUGH_INTENDED;
    case Primitive::kPrimFloat:
      is_fp = true;
      break;
    case Primitive::kPrimLong:
      is_wide = true;
      FALLTHROUGH_INTENDED;
    case Primitive::kPrimInt:
      is_fp = false;
      break;
    default:
      // The insn is not a comparison.
      return false;
  }

  // Determine if the first use is possibly an IV.
  bool is_iv_second_use = false;

  // Determine if the first use is possibly an IV.
  HInductionVariable* iv_info = GetInductionVariable(first_element);
  HInstruction* second_element = insn_to_check->InputAt(1);

  if (iv_info == nullptr) {
    // Did not find the IV in first argument. Try second.
    if (second_element->IsTypeConversion()) {
      second_element = second_element->InputAt(0);
    }

    iv_info = GetInductionVariable(second_element);

    if (iv_info == nullptr) {
      // Did not find IV in second argument either.
      return false;
    } else {
      is_iv_second_use = true;
    }
  }

  if (!iv_info->IsBasic()) {
    // The IV is not basic.
    return false;
  }

  bound_info_.loop_biv_ = iv_info;

  // We have the IV, the other must be a constant.
  HInstruction* cst = is_iv_second_use ? first_element : second_element;
  HConstant* constant = cst->AsConstant();

  if (constant == nullptr) {
    return false;
  }

  bound_info_.constant_bound_ = constant;
  bound_info_.is_bound_constant_ = true;

  // If the IV is the second use, flip the opcode so we treat it as if it is first.
  if (is_iv_second_use) {
    comparison_condition = FlipConditionForOperandSwap(comparison_condition);
  }

  // If the taken block is the exit block, negate the condition so that we deal with.
  if (branch->AsIf()->IfTrueSuccessor() == exit_block) {
    comparison_condition = NegateCondition(comparison_condition);
  }

  // Now we are at point where we know we have form: BIV cond INV.
  if (bound_info_.loop_biv_->IsIncrementPositive()) {
    // This is a simple count up loop.
    bound_info_.is_simple_count_up_ = true;
    bound_info_.comparison_condition_ = comparison_condition;
  } else if (bound_info_.loop_biv_->IsIncrementNegative()) {
    // This is a simple count down loop.
    bound_info_.is_simple_count_down_ = true;
    bound_info_.comparison_condition_ = comparison_condition;
  }

  // If this is a simple count up loop and we know the upper bound, see if we can determine
  // the number of iterations.
  HPhi* phi = bound_info_.loop_biv_->GetPhiInsn();
  HConstant* entry_value = FindBIVEntrySSA(phi);

  // Did we find a constant?
  if (entry_value != nullptr) {
    if (!is_fp) {
      int64_t increment = bound_info_.loop_biv_->GetIncrement();
      DCHECK_NE(increment, 0);

      int64_t start_value = 0, end_value = 0;

      // Try to get the border values.
      if (!GetIntConstantValue(entry_value, start_value)) {
        return false;
      }

      if (!GetIntConstantValue(bound_info_.constant_bound_, end_value)) {
        return false;
      }

      const int64_t min_value = is_wide ?
        std::numeric_limits<int64_t>::min() :
        std::numeric_limits<int32_t>::min();

      const int64_t max_value = is_wide ?
        std::numeric_limits<int64_t>::max() :
        std::numeric_limits<int32_t>::max();

      // Normalize conditions to get rid of 'equal' conditions.
      if (bound_info_.comparison_condition_ == kCondLE) {
        if (end_value == max_value) {
          // i <= MAX_VALUE is an infinite loop.
          return false;
        } else {
          // Normalize the end value to represent less than "<"
          // i <= 14 is equivalent to i < 15.
          end_value += 1;
        }
        bound_info_.comparison_condition_ = kCondLT;
      } else if (bound_info_.comparison_condition_ == kCondGE) {
        if (end_value == min_value) {
          // i >= MIN_VALUE is an infinite loop.
          return false;
        } else {
          // Normalize the end value to represent greater than ">"
          // i >= 14 is equivalent to i > 13.
          end_value -= 1;
        }
        bound_info_.comparison_condition_ = kCondGT;
      }

      if (bound_info_.is_simple_count_up_) {
        // Verify condition.
        if (bound_info_.comparison_condition_ != kCondLT) {
          return false;
        }

        // Now the borders are normalized. Make sure that the loop isn't empty.
        if (start_value >= end_value) {
          return false;
        }

        // Wouldn't we overflow? Consider the situation:
        // for (int i = 0; i < 2E+9 + 1; i += 1E+9) {...}
        // Condition will be true for i = 0, i = 1E+9 and 2E+9.
        // When we add 1E+9 to 2E+9, we overflow int. And the
        // formula for number of iterations cannot be applied.
        if ((max_value - increment + 1) < end_value) {
          return false;
        }
      } else if (bound_info_.is_simple_count_down_) {
        // The same logic is for count down loops.
        if (bound_info_.comparison_condition_ != kCondGT) {
          return false;
        }

        if (start_value <= end_value) {
          return false;
        }

        if (end_value < (min_value - increment - 1)) {
          return false;
        }
      } else {
        // Should never come here.
        DCHECK(false);
        return false;
      }

      int64_t num_iterations = (end_value - start_value) / increment;
      if (((end_value - start_value) % increment) != 0) {
        // If the mod is non-zero, then we will execute this one more iteration.
        num_iterations++;
      }

      // Fill the structure.
      bound_info_.biv_start_value_ = start_value;
      bound_info_.biv_end_value_ = end_value;
      bound_info_.num_iterations_ = num_iterations;
    } else {
      // Floating point is handled here.
      return FillFloatingPointBound(entry_value, is_wide);
    }
  } else {
    // Bail, we did not find the entry value.
    return false;
  }

  return true;
}

int64_t HLoopInformation_X86::GetNumIterations(HBasicBlock* bb) const {
  // Paranoia.
  DCHECK(bb != nullptr);
  DCHECK(ExecutedPerIteration(bb));

  int64_t number_of_biv_increments = bound_info_.num_iterations_;

  if (number_of_biv_increments == -1) {
    // The number of iterations for this loop is unknown.
    return -1;
  }

  HInductionVariable* iv = GetBasicIV();
  if (iv == nullptr || iv->linear_insn_ == nullptr) {
    return -1;
  }

  HBasicBlock* biv_increment_bb = iv->linear_insn_->GetBlock();
  DCHECK(biv_increment_bb != nullptr);
  HBasicBlock* exit_block = GetExitBlock();

  if (exit_block == nullptr) {
    return -1;
  }

  bool biv_dominates_exit = biv_increment_bb->Dominates(exit_block);
  bool bb_dominates_exit = bb->Dominates(exit_block);

  if (biv_dominates_exit == bb_dominates_exit) {
    // Both BIV increment and BB lie above or below the IF. And executed the
    // same number of times since they both are executed per iteration.
    return number_of_biv_increments;
  }

  // There can be two possible cases:
  //    CASE 1:                 CASE 2:
  //     ----------------        ----------------
  //     |              |        |              |
  //     V              |        V              |
  // |---------|        |    |---------|        |
  // | biv_inc |        |    |   bb    |        |
  // |---------|        |    |---------|        |
  //      |             |         |             |
  //    (IF)------> OUT |       (IF)------> OUT |
  //      |             |         |             |
  //      V             |         V             |
  // |---------|        |    |---------|        |
  // |   bb    |---------    | biv_inc |---------
  // |---------|             |---------|
  if (biv_dominates_exit) {
    // We are in CASE 1 and BIV increment is executed 1 time more than BB.
    return number_of_biv_increments - 1;
  } else {
    // We are in CASE 2 and BIV increment is executed 1 time less than BB.
    return number_of_biv_increments + 1;
  }
}

bool HLoopInformation_X86::FillFloatingPointBound(HConstant* entry_value, bool is_double) {
  double increment = bound_info_.loop_biv_->GetFPIncrement();
  DCHECK(increment != 0.0);

  // Get the start and end values with error checking.
  double start_value = 0, end_value = 0;

  if (!GetFPConstantValue(entry_value, start_value)) {
    return false;
  }

  if (!GetFPConstantValue(bound_info_.constant_bound_, end_value)) {
    return false;
  }

  // Deconstruct doubles into parts.
  int32_t sign_start, sign_end, sign_inc;
  int64_t mantissa_start, mantissa_end, mantissa_inc;
  int32_t power_start, power_end, power_inc;

  DeconstructFP(start_value, is_double, true, sign_start, power_start, mantissa_start);
  DeconstructFP(end_value, is_double, true, sign_end, power_end, mantissa_end);
  DeconstructFP(increment, is_double, true, sign_inc, power_inc, mantissa_inc);

  // For now, consider only case of the same sign. In this case the power cannot decrease.
  if (sign_start != sign_end || sign_start != sign_inc || power_start > power_end) {
    // TODO: This case can be handled by splitting the interval into two intervals:
    // for (x = start; x < 0; x += step) {...}; for(; x < end; x += step)
    // and calculate the number of iterations for every case. Now we consider only the case
    // where the absolute value of x always increases.
    return false;
  }

  // For count up loop, check that the loop is not empty.
  if ((sign_inc == 0) && (start_value >= end_value)) {
    return false;
  }

  // For count down loop, check that the loop is not empty.
  if ((sign_inc == 1) && (start_value <= end_value)) {
    return false;
  }
  // Consider that we iterate from start_value to end_value, and
  // abs(start_value) < abs(end_value). They are represented in
  // FP as follows below:
  //
  // start_value = [sign_start][power_start][mantissa_start]
  // end_value   = [ sign_end ][ power_end ][ mantissa_end ]
  // increment   = [ sign_inc ][ power_inc ][ mantissa_inc ]
  //
  // We want to guarantee that all intermediate values of x in loop
  //   for (float x = start; x < end; x += step) {...}
  // are representable in fp without the loss of precision.
  //
  // How can we get an non-representable value if we add two representable
  // values? Let us add x and y. First, we should normalize them to the
  // greater power. Without loss of generality, let exp_x be >= exp_y.
  //     [ ... mantissa of x ... ]
  //          [E mantissa of y ... 1] <-- One gone out of grid and was lost.
  //     |       grid of FP      |
  //
  // Here E denominates the implicit 1 that is not stored in mantissa, but
  // should be considered in adjustment if y != 0.
  // The number of bits of y that didn't fit into the grid is equal to
  // K = (exp_x - exp_y). If there was at least one non-zero bit among the
  // K last bits of y, we lose precision and cannot estimate the number of
  // iterations correctly.
  //
  // Let adjusted_pow = max(power_start, power_inc).
  // While iterating from start_value to end_value, the summary number of
  // increases of power (and bits that go out of grid) is N = (end_power - start_power).
  // Now we can formulate the required and sufficient conditions of representability
  // of all intermediate values over our iteration:
  // 1. mantissa_start has at least z_1 = (adjusted_pow - power_start) zeros in the end;
  // 2. mantissa_step has at least z_2 = (adjusted_pow - power_inc) zeros in the end;
  // 3. They both have at least N zeros in the end: min(z1, z2) >= N.
  //
  // If all three conditions are met, all intermediate numbers are representable.

  // Adjust start and step.
  const int32_t adjusted_pow = std::max(power_start, power_inc);
  const int32_t zeros_start = CountEndZerosInMantissa(mantissa_start, is_double);
  const int32_t zeros_inc = CountEndZerosInMantissa(mantissa_inc, is_double);

  // Ensure that mantissae can be adjusted to this value.
  if (zeros_start < (adjusted_pow - power_start)) {
    return false;
  }

  if (zeros_inc < (adjusted_pow - power_inc)) {
    return false;
  }

  // Estimate the number of allowed increments of power.
  const int32_t allowed_increments = std::min(zeros_start, zeros_inc);

  if ((power_end - power_start) > allowed_increments) {
    // We need to do more increments than we are allowed.
    return false;
  }

  // We have proved that all values of our IV within our loop are
  // representable in floating-point grid.
  // It means that adjusted extended mantissae (i.e. with implicit ones)
  // of these values behave as usual integers. We don't overflow here
  // because all extended mantissae do not exceed 2^53.
  const int max_power = std::max(adjusted_pow, power_end);
  mantissa_start >>= (max_power - power_start);
  mantissa_end >>= (max_power - power_end);
  mantissa_inc >>= (max_power - power_inc);

  // Now we can estimate the number of iterations.
  int64_t num_iterations = (mantissa_end - mantissa_start) / mantissa_inc;
  if ((mantissa_end - mantissa_start) % mantissa_inc != 0) {
    num_iterations++;
  }

  bound_info_.biv_start_value_ = start_value;
  bound_info_.biv_end_value_ = end_value;

  // Check for overflows and compute the number of iterations for this loop.
  bound_info_.num_iterations_ = num_iterations;
  return true;
}

HInductionVariable* HLoopInformation_X86::GetInductionVariable(HInstruction* insn) const {
  // The instruction can either be the phi node or the linear instrction.
  //   Thus check for both.

  // Now walk the induction variables.
  for (HInductionVariable* iv : iv_list_) {
    if (iv->GetLinearInsn() == insn) {
      return iv;
    }

    if (iv->GetPhiInsn() == insn) {
      return iv;
    }
  }

  // Not found: return nullptr.
  return nullptr;
}

HConstant* HLoopInformation_X86::FindBIVEntrySSA(HPhi* phi) const {
  size_t max = phi->InputCount();
  for (size_t i = 0; i < max; i++) {
    HInstruction* insn = phi->InputAt(i);
    HConstant* constant = insn->AsConstant();
    if (constant != nullptr) {
      return constant;
    }
  }
  return nullptr;
}

bool HLoopInformation_X86::HasOneExitBlock() const {
  return GetExitBlock() != nullptr;
}

HBasicBlock* HLoopInformation_X86::GetExitBlock() const {
  HBasicBlock* exit_block = nullptr;
  for (HBlocksInLoopIterator it_loop(*this); !it_loop.Done(); it_loop.Advance()) {
    HBasicBlock* loop_block = it_loop.Current();
    // Look for a successor that isn't in the loop.
    for (HBasicBlock* successor : loop_block->GetSuccessors()) {
      if (!Contains(*successor)) {
        if (exit_block == nullptr || exit_block == successor) {
          // First or duplicate exit block.
          exit_block = successor;
        } else {
          // More than one different exit blocks.
          return nullptr;
        }
      }
    }
  }

  return exit_block;
}

void HLoopInformation_X86::InsertInstructionInSuspendBlock(HInstruction* instruction) {
  // Have we already split the SuspendCheck into two pieces?
  if (!HasTestSuspend()) {
    SplitSuspendCheck();
  }

  DCHECK(HasTestSuspend());
  DCHECK(HasSuspend());
  HSuspend* suspend = GetSuspend();

  // Insert the actual instruction before the HSuspend. That will ensure that
  // successive calls insert in order.
  suspend->GetBlock()->InsertInstructionBefore(instruction, suspend);
}

void HLoopInformation_X86::SplitSuspendCheck() {
  // Have we already been split?
  if (HasTestSuspend()) {
    // We must have got here already.
    DCHECK(!HasSuspendCheck());
    return;
  }

  // We better have an HSuspendCheck to split!
  DCHECK(HasSuspendCheck());

  // Replace the SuspendCheck with a TestSuspend.
  HSuspendCheck* check = GetSuspendCheck();

  // The suspend check must be in the header.
  HBasicBlock* header = GetHeader();
  DCHECK(header == check->GetBlock());
  DCHECK(header->GetLoopInformation() == this);
  HGraph* graph = header->GetGraph();
  ArenaAllocator* arena = graph->GetArena();

  // Special case single block loops.
  if (GetBackEdges()[0] == header) {
    // Clear the back edges now, in order to prevent 2 edges after the split.
    // It will be rebuilt later.
    ClearBackEdges();
  }

  // Create a new block to hold the TestSuspend. This avoids a 'CriticalEdge' with
  // multiple predecessors and successors.
  HBasicBlock* test_suspend_block = new(arena) HBasicBlock(graph, header->GetDexPc());
  graph->AddBlock(test_suspend_block);

  // Add an HTestSuspend into this block.
  HTestSuspend* test_suspend = new(arena) HTestSuspend;
  test_suspend_block->AddInstruction(test_suspend);
  SetTestSuspend(test_suspend);

  // Create a dummy block just for the suspend to return to,
  // in order to avoid creating a critical edge.
  HBasicBlock* dummy_block = new(arena) HBasicBlock(graph, header->GetDexPc());
  graph->AddBlock(dummy_block);
  dummy_block->AddInstruction(new(arena) HGoto());

  // We need a second dummy block to avoid another critical edge, to handle
  // the non-suspend case.
  HBasicBlock* dummy_block2 = new(arena) HBasicBlock(graph, header->GetDexPc());
  graph->AddBlock(dummy_block2);
  dummy_block2->AddInstruction(new(arena) HGoto());
  dummy_block2->AddSuccessor(dummy_block);
  test_suspend_block->AddSuccessor(dummy_block2);

  // Create a new block to hold the code after the TestSuspend.
  HBasicBlock* rest_of_header = header->SplitAfterForInlining(check);

  // Function SplitAfter() divides a block into two halves but it
  // doesn't add the bottom half into reverse_post_order_. And, this
  // may cause a trouble. When HGraph::ClearDominanceInformation() is
  // called, it doesn't clear dominated_blocks_ for the bottom half
  // because the half is absent in the reverse_post_order_. Next,
  // HGraph::ComputeDominanceInformation() adds blocks into
  // dominated_blocks_ but it doesn't check for duplicate values. So,
  // it may populate dominated_blocks_ with same blocks. This is not
  // normal and violates DCHECK conditions.
  HBasicBlock* dom = rest_of_header->GetDominator();
  rest_of_header->ClearDominanceInformation();
  rest_of_header->SetDominator(dom);

  graph->AddBlock(rest_of_header);

  // Now add a Goto to the end of the header block.
  header->AddInstruction(new(arena) HGoto());
  header->AddSuccessor(test_suspend_block);

  // And link in the dummy block.
  dummy_block->AddSuccessor(rest_of_header);

  // Create a new block to hold the Suspend.
  HBasicBlock* suspend_block = new(arena) HBasicBlock(graph, header->GetDexPc());
  graph->AddBlock(suspend_block);

  // Create the new suspend instruction with the original environment.
  HSuspend* suspend = new(arena) HSuspend(check->GetDexPc());
  SetSuspend(suspend);
  suspend_block->AddInstruction(suspend);
  suspend->CopyEnvironmentFrom(check->GetEnvironment());
  suspend_block->AddInstruction(new(arena) HGoto());

  // Remove the HSuspendCheck.
  header->RemoveInstruction(check);
  SetSuspendCheck(nullptr);

  // Link the new block into the loop.
  test_suspend_block->AddSuccessor(suspend_block);
  suspend_block->AddSuccessor(dummy_block);

  // Set loop information for new blocks.
  AddToAll(suspend_block);
  AddToAll(test_suspend_block);
  AddToAll(dummy_block);
  AddToAll(dummy_block2);
  AddToAll(rest_of_header);
}

void HLoopInformation_X86::AddToAll(HBasicBlock* block) {
  block->SetLoopInformation(this);
  for (HLoopInformation_X86* current = this; current != nullptr; current = current->GetParent()) {
    static_cast<HLoopInformation*>(current)->Add(block);
  }
}

}  // namespace art
