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

#include "ext_alias.h"

namespace art {

bool AliasCheck::Instance_base_same(HInstruction* x, HInstruction* y) {
  // Skip NullCheck for aliasing.
  if (x->IsNullCheck()) {
    x = x->InputAt(0);
  }
  if (y->IsNullCheck()) {
    y = y->InputAt(0);
  }

  // Identical bases are obviously the same.
  if (x == y) {
    return true;
  }

  // If they must alias, then they are also the same.
  return Alias(x, y) == kMustAlias;
}

AliasCheck::AliasKind AliasCheck::Instance_alias(const FieldInfo& x_field,
                                       const FieldInfo& y_field,
                                       HInstruction* x_base,
                                       HInstruction* y_base) {
  if (x_field.IsVolatile() || y_field.IsVolatile()) {
    return kMayAlias;
  }
  if (x_field.GetFieldOffset().SizeValue() != y_field.GetFieldOffset().SizeValue() ||
      x_field.GetFieldType() != y_field.GetFieldType()) {
    // Not possible to alias.
    return kNoAlias;
  }
  if (Instance_base_same(x_base, y_base)) {
    // They have the same type, offset and object.
    return kMustAlias;
  }
  return kMayAlias;
}

AliasCheck::AliasKind AliasCheck::Alias(HInstanceFieldGet* x_get, HInstruction* y) {
  if (x_get == y) {
    return kMustAlias;
  }
  const FieldInfo& x_field = x_get->GetFieldInfo();
  switch (y->GetKind()) {
    case HInstruction::kInstanceFieldGet: {
      const FieldInfo& y_field = y->AsInstanceFieldGet()->GetFieldInfo();
      return Instance_alias(x_field, y_field, x_get->InputAt(0), y->InputAt(0));
    }
    case HInstruction::kInstanceFieldSet: {
      const FieldInfo& y_field = y->AsInstanceFieldSet()->GetFieldInfo();
      return Instance_alias(x_field, y_field, x_get->InputAt(0), y->InputAt(0));
    }
    case HInstruction::kStaticFieldGet:
      return y->AsStaticFieldGet()->IsVolatile() ? kMayAlias : kNoAlias;
    case HInstruction::kStaticFieldSet:
      return y->AsStaticFieldSet()->IsVolatile() ? kMayAlias : kNoAlias;
    case HInstruction::kArrayGet:
    case HInstruction::kArraySet:
      return kNoAlias;
    default:
      if (HasSideEffects(y)) {
        return kMayAlias;
      }
      return kNoAlias;
  }
}

AliasCheck::AliasKind AliasCheck::Alias(HInstanceFieldSet* x_set, HInstruction* y) {
  if (x_set == y) {
    return kMustAlias;
  }
  const FieldInfo& x_field = x_set->GetFieldInfo();
  switch (y->GetKind()) {
    case HInstruction::kInstanceFieldGet: {
      const FieldInfo& y_field = y->AsInstanceFieldGet()->GetFieldInfo();
      return Instance_alias(x_field, y_field, x_set->InputAt(0), y->InputAt(0));
    }
    case HInstruction::kInstanceFieldSet: {
      const FieldInfo& y_field = y->AsInstanceFieldSet()->GetFieldInfo();
      return Instance_alias(x_field, y_field, x_set->InputAt(0), y->InputAt(0));
    }
    case HInstruction::kStaticFieldGet:
      return y->AsStaticFieldGet()->IsVolatile() ? kMayAlias : kNoAlias;
    case HInstruction::kStaticFieldSet:
      return y->AsStaticFieldSet()->IsVolatile() ? kMayAlias : kNoAlias;
    case HInstruction::kArrayGet:
    case HInstruction::kArraySet:
    default:
      if (HasSideEffects(y)) {
        return kMayAlias;
      }
      return kNoAlias;
  }
}

static AliasCheck::AliasKind Static_alias(const FieldInfo& x_field,
                                     const FieldInfo& y_field,
                                     HInstruction* x_cls,
                                     HInstruction* y_cls) {
  if (x_field.IsVolatile() || y_field.IsVolatile()) {
    return AliasCheck::kMayAlias;
  }
  if (x_field.GetFieldOffset().SizeValue() != y_field.GetFieldOffset().SizeValue() ||
      x_field.GetFieldType() != y_field.GetFieldType()) {
    // Not possible to alias.
    return AliasCheck::kNoAlias;
  }
  if (x_cls == y_cls ||
      (x_cls->IsLoadClass() && y_cls->IsLoadClass() &&
       x_cls->AsLoadClass()->InstructionDataEquals(y_cls))) {
    // They have the same class, offset and object.
    return AliasCheck::kMustAlias;
  }
  // They might be members of the same class.
  return AliasCheck::kMayAlias;
}

AliasCheck::AliasKind AliasCheck::Alias(HStaticFieldGet* x_get, HInstruction* y) {
  if (x_get == y) {
    return kMustAlias;
  }
  const FieldInfo& x_field = x_get->GetFieldInfo();
  switch (y->GetKind()) {
    case HInstruction::kStaticFieldGet: {
      const FieldInfo& y_field = y->AsStaticFieldGet()->GetFieldInfo();
      return Static_alias(x_field, y_field, x_get->InputAt(0), y->InputAt(0));
    }
    case HInstruction::kStaticFieldSet: {
      const FieldInfo& y_field = y->AsStaticFieldSet()->GetFieldInfo();
      return Static_alias(x_field, y_field, x_get->InputAt(0), y->InputAt(0));
    }
    case HInstruction::kInstanceFieldGet:
      return y->AsInstanceFieldGet()->IsVolatile() ? kMayAlias : kNoAlias;
    case HInstruction::kInstanceFieldSet:
      return y->AsInstanceFieldSet()->IsVolatile() ? kMayAlias : kNoAlias;
    case HInstruction::kArrayGet:
    case HInstruction::kArraySet:
      return kNoAlias;
    default:
      if (HasSideEffects(y)) {
        return kMayAlias;
      }
      return kNoAlias;
  }
}

AliasCheck::AliasKind AliasCheck::Alias(HStaticFieldSet* x_set, HInstruction* y) {
  if (x_set == y) {
    return kMustAlias;
  }
  const FieldInfo& x_field = x_set->GetFieldInfo();
  switch (y->GetKind()) {
    case HInstruction::kStaticFieldGet: {
      const FieldInfo& y_field = y->AsStaticFieldGet()->GetFieldInfo();
      return Static_alias(x_field, y_field, x_set->InputAt(0), y->InputAt(0));
    }
    case HInstruction::kStaticFieldSet: {
      const FieldInfo& y_field = y->AsStaticFieldSet()->GetFieldInfo();
      return Static_alias(x_field, y_field, x_set->InputAt(0), y->InputAt(0));
    }
    case HInstruction::kInstanceFieldGet:
      return y->AsInstanceFieldGet()->IsVolatile() ? kMayAlias : kNoAlias;
    case HInstruction::kInstanceFieldSet:
      return y->AsInstanceFieldSet()->IsVolatile() ? kMayAlias : kNoAlias;
    case HInstruction::kArrayGet:
    case HInstruction::kArraySet:
      return kNoAlias;
    default:
      if (HasSideEffects(y)) {
        return kMayAlias;
      }
      return kNoAlias;
  }
}

bool AliasCheck::Array_base_same(HInstruction* x, HInstruction* y) {
  // Identical bases are obviously the same.
  if (x == y) {
    return true;
  }

  // If they must alias, then they are also the same.
  return Alias(x, y) == kMustAlias;
}

AliasCheck::AliasKind AliasCheck::Array_index_alias(HInstruction* x, HInstruction* y) {
  // See through bounds checks.
  if (x->IsBoundsCheck()) {
    x = x->InputAt(0);
  }
  if (y->IsBoundsCheck()) {
    y = y->InputAt(0);
  }

  if (x == y) {
    // Same index must be aliased.
    return kMustAlias;
  }

  if (x->IsIntConstant() && y->IsIntConstant()) {
    int32_t x_index_value = x->AsIntConstant()->GetValue();
    int32_t y_index_value = y->AsIntConstant()->GetValue();
    return (x_index_value == y_index_value) ? kMustAlias : kNoAlias;
  }
  // TODO: Improve this looking for i and i+1, etc.
  return kMayAlias;
}

AliasCheck::AliasKind AliasCheck::Array_alias(HInstruction* x, HInstruction* y) {
  if (x == y) {
    return kMustAlias;
  }
  // Do they have the same bases?
  if (Array_base_same(x->InputAt(0), y->InputAt(0))) {
    // We know the base is the same.  Can we differentiate the index?
    return Array_index_alias(x->InputAt(1), y->InputAt(1));
  }

  // Look at the type after looking at the base, as there are some cases where
  // the ArraySet is a long and the ArrayGet is a double.  This may be fixed in
  // a later AOSP.
  Primitive::Type x_type = x->IsArrayGet() ? x->GetType() : x->AsArraySet()->GetComponentType();
  Primitive::Type y_type = y->IsArrayGet() ? y->GetType() : y->AsArraySet()->GetComponentType();
  if (x_type != y_type) {
    // The types don't match, so they have to be different.
    return kNoAlias;
  }

  // TODO: Look at the instructions, and see if we can tell anything more.
  return kMayAlias;
}

AliasCheck::AliasKind AliasCheck::Alias(HArrayGet* x_get, HInstruction* y) {
  switch (y->GetKind()) {
    case HInstruction::kArrayGet:
    case HInstruction::kArraySet:
      return Array_alias(x_get, y);
    case HInstruction::kInstanceFieldGet:
      return y->AsInstanceFieldGet()->IsVolatile() ? kMayAlias : kNoAlias;
    case HInstruction::kInstanceFieldSet:
      return y->AsInstanceFieldSet()->IsVolatile() ? kMayAlias : kNoAlias;
    case HInstruction::kStaticFieldGet:
      return y->AsStaticFieldGet()->IsVolatile() ? kMayAlias : kNoAlias;
    case HInstruction::kStaticFieldSet:
      return y->AsStaticFieldSet()->IsVolatile() ? kMayAlias : kNoAlias;
    default:
      if (HasSideEffects(y)) {
        return kMayAlias;
      }
      return kNoAlias;
  }
}

AliasCheck::AliasKind AliasCheck::Alias(HArraySet* x_set, HInstruction* y) {
  switch (y->GetKind()) {
    case HInstruction::kArrayGet:
    case HInstruction::kArraySet:
      return Array_alias(x_set, y);
    case HInstruction::kInstanceFieldGet:
      return y->AsInstanceFieldGet()->IsVolatile() ? kMayAlias : kNoAlias;
    case HInstruction::kInstanceFieldSet:
      return y->AsInstanceFieldSet()->IsVolatile() ? kMayAlias : kNoAlias;
    case HInstruction::kStaticFieldGet:
      return y->AsStaticFieldGet()->IsVolatile() ? kMayAlias : kNoAlias;
    case HInstruction::kStaticFieldSet:
      return y->AsStaticFieldSet()->IsVolatile() ? kMayAlias : kNoAlias;
    default:
      if (HasSideEffects(y)) {
        return kMayAlias;
      }
      return kNoAlias;
  }
}

AliasCheck::AliasKind AliasCheck::Alias(HInstruction* x, HInstruction* y) {
  // Always have the 'lower' instruction as the first operand.  This allows
  // easier caching of results.
  if (x->GetId() > y->GetId()) {
    std::swap(x, y);
  }

  // TODO(if needed): lookup on the ids here, and return the cached result.
  DCHECK_LE(x->GetId(), y->GetId());

  AliasKind result = kNoAlias;
  switch (x->GetKind()) {
    case HInstruction::kInstanceFieldGet:
      result = Alias(x->AsInstanceFieldGet(), y);
      break;
    case HInstruction::kInstanceFieldSet:
      result = Alias(x->AsInstanceFieldSet(), y);
      break;
    case HInstruction::kStaticFieldGet:
      result = Alias(x->AsStaticFieldGet(), y);
      break;
    case HInstruction::kStaticFieldSet:
      result = Alias(x->AsStaticFieldSet(), y);
      break;
    case HInstruction::kArrayGet:
      result = Alias(x->AsArrayGet(), y);
      break;
    case HInstruction::kArraySet:
      result = Alias(x->AsArraySet(), y);
      break;
    default:
      if (HasSideEffects(x)) {
        result = kMayAlias;
      }
      break;
  }
  return result;
}

}  // namespace art
