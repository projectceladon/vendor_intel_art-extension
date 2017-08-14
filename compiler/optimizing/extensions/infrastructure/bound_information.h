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

#ifndef VENDOR_INTEL_ART_EXTENSION_OPT_INFRASTRUCTURE_BOUND_INFORMATION_H_
#define VENDOR_INTEL_ART_EXTENSION_OPT_INFRASTRUCTURE_BOUND_INFORMATION_H_

namespace art {

// Forward declarations.
class HConstant;
class HInductionVariable;

/**
 * @brief This structure is used to keep bound information.
 * @see ComputeBoundInformation which outlines the algorithm used to compute this.
 * @details This is not guaranteed to be filled completely.
 * - loop_biv may be null.
 * - constant_bound is only valid if is_bound_constant is true.
 * - comparison_condition is only valid if is_simple_count_up or is_simple_count_down are true.
 * - num_iterations, biv_start_value, and biv_end_value are only valid if knowIterations is true.
 */
struct HLoopBoundInformation {
  HLoopBoundInformation() :
    loop_biv_(nullptr), is_bound_constant_(false), constant_bound_(nullptr),
    is_simple_count_up_(false), is_simple_count_down_(false),
    comparison_condition_(0), num_iterations_(-1), biv_start_value_(0),
    biv_end_value_(0) {
    }

  /**
   * @brief Used to store the loop's BIV.
   * @details In order for this to exist, the loop must have only one backedge and exit.
   * Additionally this BIV must be used in the condition that iterates the loop.
   */
  HInductionVariable* loop_biv_;

  /**
   * @brief Whether the loop's bound is constant.
   */
  bool is_bound_constant_;

  /**
   * @brief The constant of the loop bound. Only valid if is_bound_constant is true.
   */
  HConstant* constant_bound_;

  /**
   * @brief Whether the loop is a simple count up loop.
   * @details The IV must increment in positive manner, the loop condition must use BIV
   * and constant/invariant.
   */
  bool is_simple_count_up_;

  /**
   * @brief Whether the loop is a simple count down loop.
   * @details The IV must increment in negative manner, the loop condition must use BIV
   * and constant/invariant.
   */
  bool is_simple_count_down_;

  /**
   * @brief Contains the comparison condition as an OP_IF bytecode.
   * @details Only valid if is_simple_count_up or is_simple_count_down are true.
   */
  int comparison_condition_;

  /**
   * @brief The number of iterations through the loop.
   * @details If this value is -1, then number of iterations is not known.
   */
  int64_t num_iterations_;

  /**
   * @brief The start value of the induction variable.
   */
  int64_t biv_start_value_;

  /**
   * @brief The end value of the induction variable.
   */
  int64_t biv_end_value_;
};
}  // namespace art

#endif  // VENDOR_INTEL_ART_EXTENSION_OPT_INFRASTRUCTURE_BOUND_INFORMATION_H_
