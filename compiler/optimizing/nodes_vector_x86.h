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

#ifndef ART_COMPILER_OPTIMIZING_NODES_VECTOR_X86_H_
#define ART_COMPILER_OPTIMIZING_NODES_VECTOR_X86_H_

#include "nodes_vector.h"

namespace art {

// viz. [ a1, .. , an ] + [ x1, .. , xn ] * [ y1, .. , yn ] = [ a1 + x1 * y1, .. , an + xn * yn ].
class HVecAvxAdd FINAL : public HVecOperation {
 public:
  HVecAvxAdd(ArenaAllocator* allocator,
                         HInstruction* dst,
                         HInstruction* src1,
                         HInstruction* src2,
                         DataType::Type packed_type,
                         size_t vector_length,
                         uint32_t dex_pc)
      : HVecOperation(kVecAvxAdd,
                      allocator,
                      packed_type,
                      SideEffects::None(),
                      /* number_of_inputs */ 3,
                      vector_length,
                      dex_pc){

    DCHECK(HasConsistentPackedTypes(dst, packed_type));
    DCHECK(HasConsistentPackedTypes(src1, packed_type));
    DCHECK(HasConsistentPackedTypes(src2, packed_type));
    SetRawInputAt(0, dst);
    SetRawInputAt(1, src1);
    SetRawInputAt(2, src2);
  }

  bool CanBeMoved() const OVERRIDE { return true; }

  DECLARE_INSTRUCTION(VecAvxAdd);

 protected:
  DEFAULT_COPY_CONSTRUCTOR(VecAvxAdd);
};

class HVecAvxSub FINAL : public HVecOperation {
 public:
  HVecAvxSub(ArenaAllocator* allocator,
                         HInstruction* dst,
                         HInstruction* src1,
                         HInstruction* src2,
                         DataType::Type packed_type,
                         size_t vector_length,
                         uint32_t dex_pc)
      : HVecOperation(kVecAvxSub,
                      allocator,
                      packed_type,
                      SideEffects::None(),
                      /* number_of_inputs */ 3,
                      vector_length,
                      dex_pc){

    DCHECK(HasConsistentPackedTypes(dst, packed_type));
    DCHECK(HasConsistentPackedTypes(src1, packed_type));
    DCHECK(HasConsistentPackedTypes(src2, packed_type));
    SetRawInputAt(0, dst);
    SetRawInputAt(1, src1);
    SetRawInputAt(2, src2);
  }

  bool CanBeMoved() const OVERRIDE { return true; }

  DECLARE_INSTRUCTION(VecAvxSub);

 protected:
  DEFAULT_COPY_CONSTRUCTOR(VecAvxSub);
};

}  // namespace art

#endif  // ART_COMPILER_OPTIMIZING_NODES_VECTOR_X86_H_
