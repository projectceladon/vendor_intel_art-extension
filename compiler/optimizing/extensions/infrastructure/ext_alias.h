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

#ifndef COMPILER_OPTIMIZING_EXTENSIONS_INFRASTRUCTURE_EXT_ALIAS_H
#define COMPILER_OPTIMIZING_EXTENSIONS_INFRASTRUCTURE_EXT_ALIAS_H

#include "nodes.h"

namespace art {

class AliasCheck {
 public:
  enum AliasKind {
    kNoAlias,           // These objects do not point to the same memory.
    kMayAlias,          // These objects might point to the same memory.
    kMustAlias          // These objects definitely point to the same memory.
  };

 /**
  * @brief Do these two memory instructions alias?
  * @returns kNoAlias if the instructions cannot alias,
  * kMayAlias if they might alias, and kMustAlias if they refer to the same memory.
  */
  AliasKind Alias(HInstruction* x, HInstruction* y) const;

 /**
  * @brief Could this instruction potentially alias another instruction?
  * @param insn Instruction to check.
  * @returns 'true' if this is a potentially aliasing instruction.
  */
  bool HasWriteSideEffects(HInstruction* insn) const {
    return insn->DoesAnyWrite() || insn->IsMonitorOperation();
  }

 private:
  inline AliasKind Alias(HInstanceFieldGet* x, HInstruction* y) const;
  inline AliasKind Alias(HInstanceFieldSet* x, HInstruction* y) const;
  inline AliasKind Alias(HStaticFieldGet* x, HInstruction* y) const;
  inline AliasKind Alias(HStaticFieldSet* x, HInstruction* y) const;
  inline AliasKind Alias(HArrayGet* x, HInstruction* y) const;
  inline AliasKind Alias(HArraySet* x, HInstruction* y) const;
  inline AliasKind Alias(HInstructionLHSMemory* x, HInstruction* y) const;
  AliasKind Array_index_alias(HInstruction* x, HInstruction *y) const;
  bool Array_base_same(HInstruction* x, HInstruction* y) const;
  bool Instance_base_same(HInstruction* x, HInstruction* y) const;
  bool May_Types_Alias(HInstruction* x, HInstruction* y) const;
  AliasKind Array_alias(HInstruction* x, HInstruction* y) const;
  AliasKind Instance_alias(const FieldInfo& x_field, const FieldInfo& y_field,
                           HInstruction* x_base, HInstruction* y_base) const;
  AliasKind Static_alias(const FieldInfo& x_field, const FieldInfo& y_field,
                         HInstruction* x_cls, HInstruction* y_cls) const;
  AliasKind LHSMemory_array_alias(HInstructionLHSMemory* x, HInstruction* index,
                                  HInstruction* y) const;
  AliasKind LHSMemory_field_alias(HInstructionLHSMemory* x, HInstruction* base,
                                  const FieldInfo& field) const;
};

}  // namespace art

#endif  // COMPILER_OPTIMIZING_EXTENSIONS_INFRASTRUCTURE_EXT_ALIAS_H
