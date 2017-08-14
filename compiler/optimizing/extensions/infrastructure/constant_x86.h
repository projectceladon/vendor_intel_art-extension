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

#ifndef ART_COMPILER_OPTIMIZING_EXTENSIONS_INFRASTRUCTURE_CONSTANT_X86_H_
#define ART_COMPILER_OPTIMIZING_EXTENSIONS_INFRASTRUCTURE_CONSTANT_X86_H_

#include "nodes.h"

namespace art {

class HConstant_X86 : public ArenaObject<kArenaAllocMisc> {
 protected:
  bool is_wide_;
  bool is_fp_;
  HConstant* constant_;

 public:
  HConstant_X86(HConstant* constant, bool is_wide, bool is_fp) :
      is_wide_(is_wide), is_fp_(is_fp), constant_(constant) {
        // Do not allow the creation of a nullptr constant...
        DCHECK(constant != nullptr);
  }

  bool IsWide() const {
    return (constant_->IsDoubleConstant() || constant_->IsLongConstant());
  }

  bool IsFP() const {
    return (constant_->IsDoubleConstant() || constant_->IsFloatConstant());
  }

  /**
   * @brief Used to Get the loop increment (BIV must be non-FP typed).
   * @return Returns the increment.
   */
  int64_t GetValue() const {
    DCHECK(IsFP() == false);
    if (IsWide()) {
      HLongConstant* tmp = constant_->AsLongConstant();
      DCHECK(tmp != nullptr);
      return tmp->GetValue();
    } else {
      HIntConstant* tmp = constant_->AsIntConstant();
      DCHECK(tmp != nullptr);
      return tmp->GetValue();
    }
  }

  /**
   * @brief Used to Get the FP loop increment(BIV must be FP typed).
   * @return Returns the increment.
   */
  double GetFPValue() const {
    DCHECK(IsFP());
    if (IsWide()) {
      HDoubleConstant* tmp = constant_->AsDoubleConstant();
      DCHECK(tmp != nullptr);
      return tmp->GetValue();
    } else {
      HFloatConstant* tmp = constant_->AsFloatConstant();
      DCHECK(tmp != nullptr);
      return tmp->GetValue();
    }
  }

  bool IsOne() const {
    return constant_->IsOne();
  }

  bool IsDoubleConstant() const {
    return constant_->IsDoubleConstant();
  }

  bool IsIntConstant() const {
    return constant_->IsIntConstant();
  }

  bool IsLongConstant() const {
    return constant_->IsLongConstant();
  }

  bool IsFloatConstant() const {
    return constant_->IsFloatConstant();
  }
};

}  // namespace art
#endif  // ART_COMPILER_OPTIMIZING_EXTENSIONS_INFRASTRUCTURE_CONSTANT_X86_H_

