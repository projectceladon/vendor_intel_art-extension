/*Copyright (C) 2018 The Android Open Source Project
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

#ifndef ART_COMPILER_OPTIMIZING_INSTRUCTION_SIMPLIFIER_X86_H_
#define ART_COMPILER_OPTIMIZING_INSTRUCTION_SIMPLIFIER_X86_H_

#include "nodes.h"
#include "optimization.h"
#include "arch/x86/instruction_set_features_x86.h"
#include "arch/x86_64/instruction_set_features_x86_64.h"
#include "driver/compiler_driver.h"

namespace art {

class CompilerDriver;

namespace x86 {

class InstructionSimplifierX86Visitor : public HGraphVisitor {
 public:
  InstructionSimplifierX86Visitor(HGraph* graph, CompilerDriver* compiler_driver, OptimizingCompilerStats* stats)
      : HGraphVisitor(graph),
        compiler_driver_(compiler_driver),
        stats_(stats) {}

 private:
  void RecordSimplification() {
    MaybeRecordStat(stats_, MethodCompilationStat::kInstructionSimplificationsArch);
  }

  bool HasCpuFeatureFlag() {
     const InstructionSetFeatures* features = compiler_driver_->GetInstructionSetFeatures();
     if (features->AsX86InstructionSetFeatures()->HasAVX2())
       return true;
     return false;
  }

  void VisitBasicBlock(HBasicBlock* block) OVERRIDE {
    for (HInstructionIterator it(block->GetInstructions()); !it.Done(); it.Advance()) {
      HInstruction* instruction = it.Current();
      if (instruction->IsInBlock()) {
        instruction->Accept(this);
      }
    }
  }

  void VisitNot(HNot * instruction) OVERRIDE;
  void VisitNeg(HNeg* instruction) OVERRIDE;
  void VisitAdd(HAdd* instruction) OVERRIDE;



  CompilerDriver* compiler_driver_;

  OptimizingCompilerStats* stats_;
};


class InstructionSimplifierX86 : public HOptimization {
 public:
  InstructionSimplifierX86(HGraph* graph, CompilerDriver* compiler_driver, OptimizingCompilerStats* stats)
      : HOptimization(graph, kInstructionSimplifierX86PassName, stats),
        compiler_driver_(compiler_driver) {}

  static constexpr const char* kInstructionSimplifierX86PassName = "instruction_simplifier_x86";

  void Run() OVERRIDE {
    InstructionSimplifierX86Visitor visitor(graph_, compiler_driver_, stats_);
    visitor.VisitReversePostOrder();
  }
 private:
   CompilerDriver* compiler_driver_;
};

}  // namespace x86
}  // namespace art

#endif  // ART_COMPILER_OPTIMIZING_INSTRUCTION_SIMPLIFIER_X86_H_
