/*
 * Copyright (C) 2015 Intel Corporation
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
 *
 */

#ifndef ART_COMPILER_OPTIMIZING_EXTENSIONS_PASSES_GVN_AFTER_PEELING_H_
#define ART_COMPILER_OPTIMIZING_EXTENSIONS_PASSES_GVN_AFTER_PEELING_H_

#include "nodes.h"
#include "optimization_x86.h"

namespace art {

class GVNAfterPeeling : public HOptimization_X86 {
 public:
  explicit GVNAfterPeeling(HGraph* graph)
    : HOptimization_X86(graph, kGVNAfterPeelingPassName) {}

  static constexpr const char* kGVNAfterPeelingPassName = "GVN_after_peeling";

  void Run() OVERRIDE;
};

}  // namespace art

#endif  // ART_COMPILER_OPTIMIZING_EXTENSIONS_PASSES_GVN_PEELING_H_
