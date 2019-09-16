/*
 * Copyright (C) 2011 The Android Open Source Project
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

#ifndef ART_RUNTIME_ARCH_X86_REGISTERS_X86_H_
#define ART_RUNTIME_ARCH_X86_REGISTERS_X86_H_

#include <iosfwd>

#include <android-base/logging.h>

#include "base/macros.h"
#include "globals.h"

namespace art {
namespace x86 {

enum Register {
  EAX = 0,
  ECX = 1,
  EDX = 2,
  EBX = 3,
  ESP = 4,
  EBP = 5,
  ESI = 6,
  EDI = 7,
  kNumberOfCpuRegisters = 8,
  kFirstByteUnsafeRegister = 4,
  kNoRegister = -1  // Signals an illegal register.
};
std::ostream& operator<<(std::ostream& os, const Register& rhs);

enum YmmRegister {
  YMM0 = 0,
  YMM1 = 1,
  YMM2 = 2,
  YMM3 = 3,
  YMM4 = 4,
  YMM5 = 5,
  YMM6 = 6,
  YMM7 = 7,
  kNumberOfYmmRegisters = 8,
  kNoYmmRegister = -1  // Signals an illegal register.
};
std::ostream& operator<<(std::ostream& os, const YmmRegister& reg);
}  // namespace x86
}  // namespace art

#endif  // ART_RUNTIME_ARCH_X86_REGISTERS_X86_H_
