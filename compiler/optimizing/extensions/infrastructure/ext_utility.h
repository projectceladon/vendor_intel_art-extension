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

#ifndef COMPILER_OPTIMIZING_EXTENSIONS_INFRASTRUCTURE_EXT_UTILITY_H
#define COMPILER_OPTIMIZING_EXTENSIONS_INFRASTRUCTURE_EXT_UTILITY_H

#include "base/stringprintf.h"
#include "driver/dex_compilation_unit.h"
#include "nodes.h"
#include <ostream>
#include <unordered_set>

namespace art {

class HInstruction;

  // This macro can be used to print messages within passes. The possible usage is:
  // PRINT_PASS_MESSAGE(this, "Value of x = %.2f", x);
  #define PRINT_PASS_MESSAGE(pass, format, ...) \
    do { \
      if (pass->IsVerbose()) { \
        LOG(INFO) << pass->GetPassName() << ": " << StringPrintf(format, ##__VA_ARGS__); \
      } \
    } while (false)

  // PRINT_PASS_OSTREAM_MESSAGE(this, x << yy << zz);
  #define PRINT_PASS_OSTREAM_MESSAGE(pass, ...) \
    do { \
      if (pass->IsVerbose()) { \
        LOG(INFO) << pass->GetPassName() << ": " << __VA_ARGS__; \
      } \
    } while (false)

class HAllUseIterator {
 public:
  explicit HAllUseIterator(HInstruction* insn) :
      env_use_it_(insn->GetEnvUses().begin()),
      env_use_it_end_(insn->GetEnvUses().end()),
      use_it_(insn->GetUses().begin()),
      use_it_end_(insn->GetUses().end()),
      index_(0),
      env_user_(nullptr),
      user_(nullptr) {
    Advance();
  }

  bool Done() {
    return (use_it_ == use_it_end_) &&
           (env_use_it_ == env_use_it_end_) &&
           env_user_ == nullptr &&
           user_ == nullptr;
  }
  
  HInstruction* Current() {
    return IsEnv() ? env_user_->GetHolder() : user_;
  }

  bool IsEnv() {
    return env_user_ != nullptr;
  }

  void Advance() {
    if (use_it_ == use_it_end_) {
      if (env_use_it_ == env_use_it_end_) {
        env_user_ = nullptr;
        user_ = nullptr;
      } else {
        index_ = env_use_it_->GetIndex();
        env_user_ = env_use_it_->GetUser();
        ++env_use_it_;
      }
    } else {
      index_ = use_it_->GetIndex();
      user_ = use_it_->GetUser();
      ++use_it_;
    }
  }

  void ReplaceInput(HInstruction* new_input) {
    if (IsEnv()) {
      DCHECK(new_input == nullptr || new_input->GetBlock() != nullptr);
      env_user_->RemoveAsUserOfInput(index_);
      env_user_->SetRawEnvAt(index_, new_input);
      if (new_input != nullptr) {
        new_input->AddEnvUseAt(env_user_, index_);
      }
    } else {
      DCHECK(new_input->GetBlock() != nullptr);
      user_->ReplaceInput(new_input, index_);
    }
  }

 private:
  HUseList<HEnvironment*>::const_iterator env_use_it_;
  HUseList<HEnvironment*>::const_iterator env_use_it_end_;
  HUseList<HInstruction*>::const_iterator use_it_;
  HUseList<HInstruction*>::const_iterator use_it_end_;
  size_t index_;
  HEnvironment* env_user_;
  HInstruction* user_;
};

  /**
   * @brief Facility to get the name of the current method.
   * @param graph The HGgraph corresponding to the method.
   * @return The name of the current method.
   */
  std::string GetMethodName(const HGraph* graph);

  /**
   * @brief Split string by delimeter and put tokens to the set.
   * @param s The string to split.
   * @param delim The delimeter character.
   * @param tokens The output set for tokens.
   */
  void SplitStringIntoSet(const std::string& s, char delim, std::unordered_set<std::string>& tokens);

  /**
   * @brief Get the integer constant value from the HConstant.
   * @param constant the HConstant we care about.
   * @param value the destination where we put the HConstant value.
   * @return if we obtained the constant value.
   */
  bool GetIntConstantValue(HConstant* constant, int64_t& value);

  /**
   * @brief Get the compare instruction from the uses of instruction.
   * @param instruction the base instruction.
   * @return the compare instruction based on the uses of the instruction,
             instruction if neither is one.
   */
  HInstruction* GetCompareInstruction(HInstruction* instruction);

  /**
   * @brief Get the FP Constant value.
   * @param constant the HConstant.
   * @param value the value of the constant filled out.
   * @return whether or not the constant is assessed.
   */
  bool GetFPConstantValue(HConstant* constant, double& value);

  /**
   * @brief Flip the condition if we flip the operands.
   * @details if we have A OP B, this returns OP2 with B OP2 A being equivalent.
   * @param cond the original condition.
   * @return the new flipped condition.
   */
  IfCondition FlipConditionForOperandSwap(IfCondition cond);

  /**
   * @brief Get the negated conditional operation.
   * For example, for if-ge, it returns if-lt.
   * @param cond The conditional if operation to negate.
   * @return Returns the negated conditional if.
   */
  IfCondition NegateCondition(IfCondition cond);

  /**
   * @brief Get sign, power and mantissa of floating point value.
   * @param value The FP value represented as double.
   * @param is_double Whether the value is double-precision FP or not.
   * @param with_implicit_one Whether we should get mantissa with implicit 1 or not.
   * @param sign The output value for sign (0 or 1).
   * @param power The output value for power. Derived from the exponent of FP.
                  For non-zeros, power = exponent - 1111...11; power(1) = 0.
   * @param mantissa The output value for mantissa. If with_implicit_one was true, it will
   *                 also contain the 23-rd (52-nd for double) bit set to 1 for non-zero value.
   */
  void DeconstructFP(double value, bool is_double, bool with_implicit_one, int32_t& sign, int32_t& power, int64_t& mantissa);

  /**
   * @brief Get the number of zeros in the end of mantissa of floating point value.
   * @param mantissa The mantissa.
   * @param is_double Whether the mantissa is of double-precision FP or not.
   * @return The number of zeros in the end of mantissa. The maximum return
   *         value is mantissa length (23 for single FP and 52 for double FP).
   */
  int CountEndZerosInMantissa(int64_t mantissa, bool is_double);

  /**
   * @brief Write an instruction to an output stream.
   * @param os Output stream.
   * @param instruction Instruction to write.
   */
  std::ostream& operator<<(std::ostream& os, HInstruction* instruction);

  /**
   * @brief Remove the given instruction as user of its inputs and its environment inputs.
   * @param instruction The HInstruction to handle.
   * @details RemoveEnvAsUser is called internally to handle environment inputs.
   */
  void RemoveAsUser(HInstruction* instruction);

  /**
   * @brief Remove the given instruction's environment as user of its inputs.
   * @param instruction The HInstruction to handle.
   */
  void RemoveEnvAsUser(HInstruction* instruction);

  /**
   * @brief Used to mark the current instruction as not being used by any environments.
   * @param instruction The HInstruction to handle.
   */
  void RemoveFromEnvironmentUsers(HInstruction* instruction);

  /**
   * @brief Get the character equivalent of the type for print or debug purposes.
   * @param type The type we want to get the equivalent of.
   * @return The resulting character equivalent.
   */
  char GetTypeId(Primitive::Type type);

  /**
   * @brief Returns pretty method name for the invoke.
   * @param call The invoke.
   * @return Called method name as string.
   */
  std::string CalledMethodName(HInvokeStaticOrDirect* call);

  /**
   * @brief Dumps a basic block to logcat. For debugging purposes only!
   * @param name The name of a block being dumped (to distinguish them).
   * @param block The block to dump.
   */
  void DumpBasicBlock(const char* name, HBasicBlock* block);

  /**
   * @brief Dumps a loop to logcat. For debugging purposes only!
   * @param loop The loop to dump.
   */
  void DumpLoop(HLoopInformation_X86* loop);
}  // namespace art
#endif  // COMPILER_OPTIMIZING_EXTENSIONS_INFRASTRUCTURE_EXT_UTILITIES_H
