/*
 * Copyright (C) 2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ART_COMPILER_OPTIMIZING_NODES_X86_H_
#define ART_COMPILER_OPTIMIZING_NODES_X86_H_

namespace art {

// Compute the address of the method for X86 Constant area support.
class HX86ComputeBaseMethodAddress FINAL : public HExpression<0> {
 public:
  // Treat the value as an int32_t, but it is really a 32 bit native pointer.
  HX86ComputeBaseMethodAddress()
      : HExpression(Primitive::kPrimInt, SideEffects::None(), kNoDexPc) {}

  bool CanBeMoved() const OVERRIDE { return true; }

  DECLARE_INSTRUCTION(X86ComputeBaseMethodAddress);

 private:
  DISALLOW_COPY_AND_ASSIGN(HX86ComputeBaseMethodAddress);
};

// Load a constant value from the constant table.
class HX86LoadFromConstantTable FINAL : public HExpression<2> {
 public:
  HX86LoadFromConstantTable(HX86ComputeBaseMethodAddress* method_base,
                            HConstant* constant)
      : HExpression(constant->GetType(), SideEffects::None(), kNoDexPc) {
    SetRawInputAt(0, method_base);
    SetRawInputAt(1, constant);
  }

  HX86ComputeBaseMethodAddress* GetBaseMethodAddress() const {
    return InputAt(0)->AsX86ComputeBaseMethodAddress();
  }

  HConstant* GetConstant() const {
    return InputAt(1)->AsConstant();
  }

  DECLARE_INSTRUCTION(X86LoadFromConstantTable);

 private:
  DISALLOW_COPY_AND_ASSIGN(HX86LoadFromConstantTable);
};

// Version of HNeg with access to the constant table for FP types.
class HX86FPNeg FINAL : public HExpression<2> {
 public:
  HX86FPNeg(Primitive::Type result_type,
            HInstruction* input,
            HX86ComputeBaseMethodAddress* method_base,
            uint32_t dex_pc)
      : HExpression(result_type, SideEffects::None(), dex_pc) {
    DCHECK(Primitive::IsFloatingPointType(result_type));
    SetRawInputAt(0, input);
    SetRawInputAt(1, method_base);
  }

  HX86ComputeBaseMethodAddress* GetBaseMethodAddress() const {
    return InputAt(1)->AsX86ComputeBaseMethodAddress();
  }

  DECLARE_INSTRUCTION(X86FPNeg);

 private:
  DISALLOW_COPY_AND_ASSIGN(HX86FPNeg);
};

// X86 version of HPackedSwitch that holds a pointer to the base method address.
class HX86PackedSwitch FINAL : public HTemplateInstruction<2> {
 public:
  HX86PackedSwitch(int32_t start_value,
                   int32_t num_entries,
                   HInstruction* input,
                   HX86ComputeBaseMethodAddress* method_base,
                   uint32_t dex_pc)
    : HTemplateInstruction(SideEffects::None(), dex_pc),
      start_value_(start_value),
      num_entries_(num_entries) {
    SetRawInputAt(0, input);
    SetRawInputAt(1, method_base);
  }

  bool IsControlFlow() const OVERRIDE { return true; }

  int32_t GetStartValue() const { return start_value_; }

  int32_t GetNumEntries() const { return num_entries_; }

  HX86ComputeBaseMethodAddress* GetBaseMethodAddress() const {
    return InputAt(1)->AsX86ComputeBaseMethodAddress();
  }

  HBasicBlock* GetDefaultBlock() const {
    // Last entry is the default block.
    return GetBlock()->GetSuccessors()[num_entries_];
  }

  DECLARE_INSTRUCTION(X86PackedSwitch);

 private:
  const int32_t start_value_;
  const int32_t num_entries_;

  DISALLOW_COPY_AND_ASSIGN(HX86PackedSwitch);
};

// X86/X86-64 version of HBoundsCheck that checks length in array descriptor.
class HX86BoundsCheckMemory : public HExpression<2> {
 public:
  HX86BoundsCheckMemory(HInstruction* index, HInstruction* array, uint32_t dex_pc)
      : HExpression(index->GetType(), SideEffects::CanTriggerGC(), dex_pc) {
    DCHECK_EQ(array->GetType(), Primitive::kPrimNot);
    DCHECK(Primitive::IsIntegralType(index->GetType()));
    SetRawInputAt(0, index);
    SetRawInputAt(1, array);
  }

  bool CanBeMoved() const OVERRIDE { return true; }
  bool InstructionDataEquals(const HInstruction* other ATTRIBUTE_UNUSED) const OVERRIDE {
    return true;
  }

  bool CanDoImplicitNullCheckOn(HInstruction* obj) const OVERRIDE {
    return obj == InputAt(1);
  }

  bool NeedsEnvironment() const OVERRIDE { return true; }

  bool CanThrow() const OVERRIDE { return true; }

  virtual size_t GetBaseInputIndex() const OVERRIDE { return 1; }

  HInstruction* GetIndex() const { return InputAt(0); }

  HInstruction* GetArray() const { return InputAt(1); }

  DECLARE_INSTRUCTION(X86BoundsCheckMemory);

 private:
  DISALLOW_COPY_AND_ASSIGN(HX86BoundsCheckMemory);
};

// neeraj - modified according to O-Master (with O, InputCount is being returned as size of array & hence
// HInstructionRHSMemory should be derived from HVariableInputSizeInstruction instead of HTemplateInstruction<3>,
// otherwise it leads to input corruption for user instruction)
class HInstructionRHSMemory : public HVariableInputSizeInstruction {
 public:

  DECLARE_INSTRUCTION(InstructionRHSMemory);

  size_t GetOffset() const { return offset_; }

  Primitive::Type GetType() const OVERRIDE { return type_; }

  bool CanDoImplicitNullCheckOn(HInstruction* obj) const OVERRIDE {
    // We can do an implicit check if we don't have an index and the offset is small.
    return obj == InputAt(1) && from_static_ == false &&
           offset_ < kPageSize && InputCount() == 2;
  }

  void SetFromStatic() { from_static_ = true; }

  virtual size_t GetBaseInputIndex() const { return 1; }

  bool InstructionDataEquals(const HInstruction* other) const OVERRIDE {
    return GetOffset() == other->AsInstructionRHSMemory()->GetOffset()
           && from_static_ == other->AsInstructionRHSMemory()->from_static_;
  }

  size_t ComputeHashCode() const OVERRIDE {
    return (HInstruction::ComputeHashCode() << 7) | GetOffset();
  }

 protected:
  HInstructionRHSMemory(Primitive::Type type,
                        HInstruction* lhs,
                        HInstruction* base,
                        HInstruction* index,
                        size_t offset,
                        ArenaAllocator* arena,
                        uint32_t dex_pc = kNoDexPc)
    : HVariableInputSizeInstruction(SideEffects::FieldReadOfType(type, false), dex_pc, arena, (index == nullptr) ? 2 : 3, kArenaAllocMisc),
      type_(type),
      from_static_(false),
      offset_(offset) {
      if (index != nullptr) {
        SetRawInputAt(2, index);
      }
      SetRawInputAt(0, lhs);
      SetRawInputAt(1, base);
    }

 private:
  const Primitive::Type type_;
  bool from_static_;
  const size_t offset_;

  DISALLOW_COPY_AND_ASSIGN(HInstructionRHSMemory);
};

class HAddRHSMemory : public HInstructionRHSMemory {
 public:
  HAddRHSMemory(Primitive::Type type,
                HInstruction* lhs,
                HInstruction* base,
                HInstruction* index,
                size_t offset,
                ArenaAllocator* arena)
      : HInstructionRHSMemory(type, lhs, base, index, offset, arena) {}

  DECLARE_INSTRUCTION(AddRHSMemory);

 private:
  DISALLOW_COPY_AND_ASSIGN(HAddRHSMemory);
};

class HSubRHSMemory : public HInstructionRHSMemory {
 public:
  HSubRHSMemory(Primitive::Type type,
                HInstruction* lhs,
                HInstruction* base,
                HInstruction* index,
                size_t offset,
                ArenaAllocator* arena)
      : HInstructionRHSMemory(type, lhs, base, index, offset, arena) {}

  DECLARE_INSTRUCTION(SubRHSMemory);

 private:
  DISALLOW_COPY_AND_ASSIGN(HSubRHSMemory);
};

class HMulRHSMemory : public HInstructionRHSMemory {
 public:
  HMulRHSMemory(Primitive::Type type,
                HInstruction* lhs,
                HInstruction* base,
                HInstruction* index,
                size_t offset,
                ArenaAllocator* arena)
      : HInstructionRHSMemory(type, lhs, base, index, offset, arena) {}

  DECLARE_INSTRUCTION(MulRHSMemory);

 private:
  DISALLOW_COPY_AND_ASSIGN(HMulRHSMemory);
};

class HDivRHSMemory : public HInstructionRHSMemory {
 public:
  HDivRHSMemory(Primitive::Type type,
                HInstruction* lhs,
                HInstruction* base,
                HInstruction* index,
                size_t offset,
                ArenaAllocator* arena)
      : HInstructionRHSMemory(type, lhs, base, index, offset, arena) {}

  DECLARE_INSTRUCTION(DivRHSMemory);

 private:
  DISALLOW_COPY_AND_ASSIGN(HDivRHSMemory);
};

}  // namespace art

#endif  // ART_COMPILER_OPTIMIZING_NODES_X86_H_
