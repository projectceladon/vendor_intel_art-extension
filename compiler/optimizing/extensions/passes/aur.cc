/*
 * INTEL CONFIDENTIAL
 * Copyright (c) 2016, Intel Corporation All Rights Reserved.
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

#include "aur.h"
#include "driver/compiler_driver.h"
#include "pass_option.h"

namespace art {

static void RemoveEnvAsUser(HInstruction* candidate,
                            bool remove_references,
                            OptimizingCompilerStats* const stats) {
  for (HEnvironment* environment = candidate->GetEnvironment();
      environment != nullptr;
      environment = environment->GetParent()) {
    for (size_t i = 0, e = environment->Size(); i < e; ++i) {
      HInstruction* instruction = environment->GetInstructionAt(i);
      if (instruction != nullptr) {
        bool should_remove = true;

        if (!remove_references) {
          if (instruction->GetType() == Primitive::kPrimNot) {
            if (!instruction->IsNullConstant()) {
              // So the reference is not null. Well it might still be dead if this
              // environment use is the last real use. One way we can determine if this
              // if all the uses strictly dominate this candidate.
              for (HUseIterator<HInstruction*> it(instruction->GetUses());
                  !it.Done();
                  it.Advance()) {
                HInstruction* user = it.Current()->GetUser();
                if (!user->StrictlyDominates(candidate)) {
                  should_remove = false;
                  break;
                }
              }
            }
          }
        }

        if (should_remove) {
          environment->RemoveAsUserOfInput(i);
          environment->SetRawEnvAt(i, nullptr);

          // Verify that the instruction has a definition - it must be the case since
          // it was used.
          DCHECK_NE(instruction->GetType(), Primitive::kPrimVoid);

          // We removed this instruction as input. Now consider it for complete removal.
          if (!instruction->GetSideEffects().HasSideEffectsExcludingGC()
              && !instruction->CanThrow()
              && !instruction->IsParameterValue()
              && !instruction->IsPhi()
              && !instruction->HasUses()) {
            instruction->GetBlock()->RemoveInstruction(instruction);

            if (stats != nullptr) {
              stats->RecordStat(MethodCompilationStat::kRemovedDeadInstruction);
              // This stat is a lower estimate since we do not actively remove instructions
              // that were caused to be dead from the current removal.
              stats->RecordStat(MethodCompilationStat::kIntelRemovedDeadInstructionViaAUR);
              if (instruction->GetType() == Primitive::kPrimNot) {
                // We also count reference removals since those are great for GC
                // since the root set becomes smaller.
                stats->RecordStat(MethodCompilationStat::kIntelRemovedDeadReferenceViaAUR);
              }
            }
          }
        }
      }
    }
  }
}

class AggressiveEnvUseRemover : public HGraphVisitor {
 public:
  AggressiveEnvUseRemover(HGraph* graph,
                          OptimizingCompilerStats* const stats,
                          bool handle_invokes) :
        HGraphVisitor(graph),
        handle_invokes_(handle_invokes),
        stats_(stats) {}
  virtual ~AggressiveEnvUseRemover() {}

  // These are throwers which are guaranteed to end current frame if there are no catches.
  // Thus everything live into them is automatically dead.
  void VisitBoundsCheck(HBoundsCheck* instr) OVERRIDE { HandleThrower(instr); }
  void VisitDivZeroCheck(HDivZeroCheck* instr) OVERRIDE { HandleThrower(instr); }
  void VisitNullCheck(HNullCheck* instr) OVERRIDE { HandleThrower(instr); }
  void VisitThrow(HThrow* instr) OVERRIDE { HandleThrower(instr); }

  // The following are guaranteed to trigger GC.
  void VisitSuspend(HSuspend* instr) OVERRIDE { HandleSuspend(instr); }
  void VisitSuspendCheck(HSuspendCheck* instr) OVERRIDE { HandleSuspend(instr); }

  // The following runtime calls may trigger GC but won't trigger deopt when app is not debuggable.
  void VisitLoadClass(HLoadClass* instr) OVERRIDE { HandlePotentialGC(instr); }
  void VisitClinitCheck(HClinitCheck* instr) OVERRIDE { HandlePotentialGC(instr); }
  void VisitInstanceOf(HInstanceOf* instr) OVERRIDE { HandlePotentialGC(instr); }
  void VisitCheckCast(HCheckCast* instr) OVERRIDE { HandlePotentialGC(instr); }
  void VisitMonitorOperation(HMonitorOperation* instr) OVERRIDE { HandlePotentialGC(instr); }
  void VisitNewArray(HNewArray* instr) OVERRIDE { HandlePotentialGC(instr); }
  void VisitNewInstance(HNewInstance* instr) OVERRIDE { HandlePotentialGC(instr); }
  void VisitArraySet(HArraySet* instr) OVERRIDE { HandlePotentialGC(instr); }

  // Debuggable applications need to support full-stack deopt. But non-debuggable
  // applications do not need to - this is because only last frame can get deoptimized
  // via HDeoptimize. However, tests which do not understand this constraint and walk
  // the stack on non-debuggable applications might fail finding the relevant values
  // which were killed via AUR.
  void VisitInvokeInterface(HInvokeInterface* instr) OVERRIDE {
    if (handle_invokes_) {
      HandlePotentialGC(instr);
    }
  }
  void VisitInvokeVirtual(HInvokeVirtual* instr) OVERRIDE {
    if (handle_invokes_) {
      HandlePotentialGC(instr);
    }
  }
  void VisitInvokeStaticOrDirect(HInvokeStaticOrDirect* instr) OVERRIDE {
    if (handle_invokes_) {
      HandlePotentialGC(instr);
    }
  }

  // The following are cases which are intentionally not handled.
  void VisitDeoptimize(HDeoptimize* instr ATTRIBUTE_UNUSED) OVERRIDE {
    /* Cannot remove any uses since needed to recover frame. */
  }

  void VisitBasicBlock(HBasicBlock* block) OVERRIDE {
    // Only instructions need visited.
    HBackwardInstructionIterator it(block->GetInstructions());
    DCHECK(it.Current()->IsControlFlow());
    for (it.Advance(); !it.Done(); it.Advance()) {
      it.Current()->Accept(this);
    }
  }

 private:
  bool IsInTryBlock(HInstruction* instr) {
    return instr->GetBlock()->IsTryBlock();
  }

  void HandleCaller(HInstruction* instr, bool may_trigger_gc) {
    if (IsInTryBlock(instr)) {
      // We cannot remove environment uses because we are in try block - they
      // may be live into catch block via runtime transition.
      return;
    }
    // Only remove references if we cannot trigger GC (or triggering GC
    // does not affect current frame).
    const bool remove_references = !may_trigger_gc;
    RemoveEnvAsUser(instr, remove_references, stats_);
  }

  void HandleThrower(HInstruction* instr) {
    DCHECK(instr->HasEnvironment());
    // We mark that we may not trigger GC because throwers are guaranteed to kill
    // the current frame (thus those objects are also dead).
    const bool may_trigger_gc = false;
    HandleCaller(instr, may_trigger_gc);
  }

  void HandlePotentialGC(HInstruction* instr) {
    if (!instr->HasEnvironment()) {
      return;
    }
    const bool may_trigger_gc = instr->GetSideEffects().Includes(SideEffects::CanTriggerGC());
    HandleCaller(instr, may_trigger_gc);
  }

  void HandleSuspend(HInstruction* suspend) {
    DCHECK(suspend->HasEnvironment());
    // The whole point of suspend check is that we are not removing objects.
    const bool remove_references = false;
    RemoveEnvAsUser(suspend, remove_references, stats_);
  }

  bool handle_invokes_;
  OptimizingCompilerStats* const stats_;
  DISALLOW_COPY_AND_ASSIGN(AggressiveEnvUseRemover);
};

void HAggressiveUseRemoverPass::Run() {
  // The aggressive use removal makes it so method is no longer debuggable. Thus if debuggability
  // is needed, we cannot run this pass.
  if (graph_->IsDebuggable()) {
    return;
  }

  // Setting this to false by default since some of the art unit tests fail (this happens because
  // the test which is intended to be debuggable is compiled with non-debuggable setting :)).
  // This can be enabled when QA infrastructure is aware of this problem.
  static PassOption<bool> handle_invokes(this, driver_, "HandleInvokes", false);

  AggressiveEnvUseRemover aur(graph_, stats_, handle_invokes.GetValue());
  // We use same walk as DCE.
  for (HPostOrderIterator b(*graph_); !b.Done(); b.Advance()) {
    HBasicBlock* block = b.Current();
    aur.VisitBasicBlock(block);
  }
}

}  // namespace art
