/*
 * Copyright (C) 2016 Intel Corporation
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

#ifndef ART_COMPILER_OPTIMIZING_INSERT_PROFILING_H_
#define ART_COMPILER_OPTIMIZING_INSERT_PROFILING_H_

#include "optimization_x86.h"

namespace art {

class CompilerDriver;

class HInsertProfiling : public HOptimization_X86 {
 public:
  HInsertProfiling(HGraph* graph,
                   const CompilerDriver* driver,
                   OptimizingCompilerStats* stats = nullptr)
    : HOptimization_X86(graph, kInsertProfiling, stats), driver_(driver) {}

  void Run() OVERRIDE;

 private:
  static constexpr const char* kInsertProfiling = "insert_profiling";

  const CompilerDriver* driver_;

  DISALLOW_COPY_AND_ASSIGN(HInsertProfiling);
};

}  // namespace art

#endif  // ART_COMPILER_OPTIMIZING_INSERT_PROFILING_H_
