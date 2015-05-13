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

#ifndef ART_COMPILER_OPTIMIZING_EXTENSIONS_INFRASTRUCTURE_INDUCTION_VARIABLE_H_
#define ART_COMPILER_OPTIMIZING_EXTENSIONS_INFRASTRUCTURE_INDUCTION_VARIABLE_H_

#include "nodes.h"
#include "constant_x86.h"

namespace art {
/*
 * An induction variable is represented by "m*i + c", where i is a basic
 * induction variable.
 */
class HInductionVariable {
 public:
  int ssa_reg_;                   // The ssa register defined by expression for IV.
  HConstant_X86 loop_increment_;  // Loop increment. Only relevant for basic IV to keep the loop increment/decrement.
  HInstruction* linear_insn_;     // HInstruction associated with the linear operation.
  HInstruction* phi_insn_;        // HInstruction associated with the phi node. May be null but never for Basic IV.

  /**
   * @brief Constructor for a Basic IV.
   * @param ssa_sreg The ssa reg that is the define of linear operation.
   * @param increment The increment/decrement.
   * @param is_wide is the IV wide?
   * @param is_fp is the IV FP?
   * @param linear_insn The linear operation (cannot be null).
   * @param phi_insn The phi for this BIV (cannot be null).
   */
  HInductionVariable(int ssa_sreg, HConstant* increment, bool is_wide, bool is_fp,
                     HInstruction* linear_insn, HInstruction* phi_insn) :
    ssa_reg_(ssa_sreg),
    loop_increment_(increment, is_wide, is_fp),
    linear_insn_(linear_insn), phi_insn_(phi_insn) {
      DCHECK(linear_insn_ != nullptr);
      DCHECK(phi_insn_ != nullptr);
  }

  /**
   * @brief Used to obtain the ssa register.
   * @details For basic IVs, it provides same ssa register as GetBasicSsaReg.
   * @return Returns the ssa register that is define of linear operation.
   */
  int GetSsaReg() const {
    return ssa_reg_;
  }

  /**
   * @brief Used to Get the HInstruction that represents the linear operation.
   * @return Returns the linear mir if one exists. Guaranteed to be not-null for BIVs.
   */
  HInstruction* GetLinearInsn() const {
    return linear_insn_;
  }

  /**
   * @brief Used to Get the HInstruction that represents the phi node.
   * @return Returns the phi mir if one exists. Guaranteed to be not-null for BIVs.
   */
  HInstruction* GetPhiInsn() const {
    return phi_insn_;
  }

  /**
   * @brief Used to Get the loop increment (BIV must be non-FP typed).
   * @return Returns the increment.
   */
  int64_t GetIncrement() const {
    DCHECK(loop_increment_.IsFP() == false);
    return loop_increment_.GetValue();
  }

  /**
   * @brief Used to Get the FP loop loop_increment(BIV must be FP typed).
   * @return Returns the loop_increment.
   */
  double GetFPIncrement() const {
    DCHECK(loop_increment_.IsFP() == true);
    return loop_increment_.GetFPValue();
  }

  /**
   * @brief Used to check if the increment of the loop is positive.
   * @return Returns true if increment is positive.
   */
  bool IsIncrementPositive() const {
    if (loop_increment_.IsFP() == true) {
      return GetFPIncrement() > 0.0;
    } else {
      return GetIncrement() > 0;
    }
  }

  /**
   * @brief Used to check if the increment of the loop is negative.
   * @return Returns true if increment is negative.
   */
  bool IsIncrementNegative() const {
    if (loop_increment_.IsFP() == true) {
      return GetFPIncrement() < 0.0;
    } else {
      return GetIncrement() < 0;
    }
  }

  /**
   * @brief Used to check if the increment of the loop is one.
   * @return Returns true if increment is one.
   */
  bool IsIncrementOne() const {
    return loop_increment_.IsOne();
  }

  /**
   * @brief Used to check whether an IV is basic.
   * @return Returns true if IV is basic.
   */
  bool IsBasic() const {
    // All IVs are currently basic.
    return true;
  }

  /**
   * @brief Used to check whether an IV is dependent.
   * @return Returns true if IV is not basic.
   */
  bool IsDependent() const {
    // All IVs are currently not dependent.
    return false;
  }

  /**
   * @brief Used to check if IV is floating point type.
   * @return Returns true if FP typed (float/double cases).
   */
  bool IsFP() const {
    return loop_increment_.IsFP() == true;
  }

  /**
   * @return Returns whether the IV is integer typed.
   */
  bool IsInteger() const {
    return loop_increment_.IsIntConstant();
  }

  /**
   * @return Returns whether the IV is long typed.
   */
  bool IsLong() const {
    return loop_increment_.IsLongConstant();
  }

  /**
   * @return Returns whether the IV is float typed.
   */
  bool IsFloat() const {
    return loop_increment_.IsFloatConstant();
  }

  /**
   * @return Returns whether the IV is double typed.
   */
  bool IsDouble() const {
    return loop_increment_.IsDoubleConstant();
  }

  /**
   * @brief Used to check if IV is a wide register.
   * @return Returns true if IV is wide (long/double cases).
   */
  bool IsWide() const {
    return loop_increment_.IsWide() == true;
  }

  bool IsBasicAndIncrementOf1() const {
    return IsBasic() && IsIncrementOne();
  }

  void Dump() const {
    if (IsFP()) {
      LOG(INFO) << "IV: ssa_reg: " << ssa_reg_ <<
        " wide: " << IsWide() << ", FP: " << IsFP() << ", loop increment: " << GetFPIncrement();
    } else {
      LOG(INFO) << "IV: ssa_reg: " << ssa_reg_ <<
        " wide: " << IsWide() << ", FP: " << IsFP() << ", loop increment: " << GetIncrement();
    }
  }

  static void* operator new(size_t size ATTRIBUTE_UNUSED, ArenaAllocator* arena) {
    return arena->Alloc(sizeof(HInductionVariable), kArenaAllocMisc);
  }

  static void operator delete(void* p ATTRIBUTE_UNUSED) {}  // Nop.
};
}  // namespace art
#endif  // ART_COMPILER_OPTIMIZING_EXTENSIONS_INFRASTRUCTURE_INDUCTION_VARIABLE_H_

