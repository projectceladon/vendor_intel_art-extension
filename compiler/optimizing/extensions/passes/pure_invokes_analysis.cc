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
 */

#include "loop_iterators.h"
#include "pure_invokes_analysis.h"

namespace art {

void HPureInvokesAnalysis::Run() {
  // Reduce the scope to method with loops only to not hurt compile time.
  HOnlyInnerLoopIterator loop_iter(GRAPH_TO_GRAPH_X86(graph_)->GetLoopInformation());
  if (loop_iter.Done()) {
    // TODO: This limitation can be removed when the purity checks gets cheaper.
    PRINT_PASS_OSTREAM_MESSAGE(this, "Skip the method " << GetMethodName(graph_)
                                     << " because it has no loops");
    return;
  }
  PRINT_PASS_OSTREAM_MESSAGE(this, "Start " << GetMethodName(graph_));
  ArenaAllocator* arena = graph_->GetArena();
  ArenaVector<HInvokeStaticOrDirect*> hoisting_candidates(arena->Adapter(kArenaAllocMisc));

  if (ProcessPureInvokes(hoisting_candidates)) {
    PRINT_PASS_OSTREAM_MESSAGE(this, "Found " << hoisting_candidates.size()
                               << " candidates for pure invoke hoisting");
    // TODO: Not implemented yet. We can hoist a pure invoke from its loop if:
    // 1. The loop is innermost;
    // 2. The object for which the method is invoke is defined outside the loop;
    // 3. All its args are invariant;
    // 4. No non-pure method is called for the object in the loop.
  } else {
    PRINT_PASS_MESSAGE(this, "Found no candidates for pure invokes hoisting");
  }

  PRINT_PASS_OSTREAM_MESSAGE(this, "End " << GetMethodName(graph_));
}

bool HPureInvokesAnalysis::CanReturnNull(HInstruction* insn) {
  if (insn->IsNewInstance()) {
    // Constructors never return null.
    return false;
  }

  if (insn->IsNullCheck()) {
    // Only if we proved that the arg of the NullCheck is not null.
    return CanReturnNull(insn->InputAt(0));
  }

  if (insn->IsInvokeStaticOrDirect()) {
    // Check the non-null method whitelist for this.
    return IsInvokeThatCanReturnNull(insn->AsInvokeStaticOrDirect());
  }

  // We can guarantee nothing for other instructions.
  return true;
}

bool HPureInvokesAnalysis::IsPureMethodInvoke(HInvokeStaticOrDirect* call) {
  DCHECK(call != nullptr);
  const MethodReference target_method = call->GetTargetMethod();
  // Do we already know the answer for this method?
  auto it_pure = pure_invokes_.find(target_method);
  if (it_pure != pure_invokes_.end()) {
    return it_pure->second;
  }

  // The whitelist contains known pure methods.
  const char* whitelist[] = {
    // String methods.
    "int java.lang.String.indexOf(java.lang.String)",
    "int java.lang.String.indexOf(java.lang.String, int)",
    "int java.lang.String.length()",
    "java.lang.String java.lang.String.toString()",
    // StringBuffer methods.
    "java.lang.String java.lang.StringBuffer.length()",
    "java.lang.String java.lang.StringBuffer.toString()",
    // StringBuilder methods.
    "java.lang.String java.lang.StringBuilder.length()",
    "java.lang.String java.lang.StringBuilder.toString()",
  };
  const size_t len = arraysize(whitelist);
  const std::string method_name = PrettyMethod(target_method.dex_method_index,
                                               *target_method.dex_file);
  const char* char_name = method_name.c_str();
  for (size_t i = 0; i < len; i++) {
    if (strcmp(char_name, whitelist[i]) == 0) {
        pure_invokes_.Put(target_method, true);
        // Remember that this one is pure.
        return true;
    }
  }
  // Remember that this one is not pure.
  pure_invokes_.Put(target_method, false);
  return false;
}

bool HPureInvokesAnalysis::IsInvokeThatCanReturnNull(HInvokeStaticOrDirect* call) {
  DCHECK(call != nullptr);
  const MethodReference target_method = call->GetTargetMethod();
  // Do we already know the answer for this method?
  auto it_null = null_invokes_.find(target_method);
  if (it_null != null_invokes_.end()) {
    return it_null->second;
  }

  // We assume that all invokes except the whitelist can return null.
  const char* whitelist[] = {
    // String methods.
    "java.lang.String java.lang.String.toString()",
    // StringBuffer methods.
    "java.lang.String java.lang.StringBuffer.toString()",
    // StringBuilder methods.
    "java.lang.String java.lang.StringBuilder.toString()",
  };
  const size_t len = arraysize(whitelist);
  const std::string method_name = PrettyMethod(target_method.dex_method_index,
                                               *target_method.dex_file);
  const char* char_name = method_name.c_str();
  for (size_t i = 0; i < len; i++) {
    if (strcmp(char_name, whitelist[i]) == 0) {
        null_invokes_.Put(target_method, false);
        return false;
    }
  }
  null_invokes_.Put(target_method, true);
  return true;
}

bool HPureInvokesAnalysis::ProcessPureInvokes(ArenaVector<HInvokeStaticOrDirect*>&
                                              hoisting_candidates) {
  DCHECK(hoisting_candidates.empty());
  for (HPostOrderIterator block_iter(*graph_); !block_iter.Done(); block_iter.Advance()) {
    HBasicBlock* block = block_iter.Current();
    bool block_is_in_loop = block->IsInLoop();

    for (HBackwardInstructionIterator insn_iter(block->GetInstructions());
         !insn_iter.Done(); insn_iter.Advance()) {
      HInstruction* insn = insn_iter.Current();
      // Paranoid.
      DCHECK(insn != nullptr);

      if (insn->IsNullCheck() && !insn->HasUses() && !CanReturnNull(insn->InputAt(0))) {
        PRINT_PASS_OSTREAM_MESSAGE(this, "Eliminated unused nullcheck " << insn
                                   << " because its argument is guaranteed to be not null");
        MaybeRecordStat(MethodCompilationStat::kIntelUselessNullCheckDeleted);
        block->RemoveInstruction(insn);
      }

      auto call = insn->AsInvokeStaticOrDirect();
      if (call == nullptr) {
        // We are interested in static/direct invokes only.
        continue;
      }

      if (!IsPureMethodInvoke(call)) {
        // We can optimize away only invokes with no side effects.
        continue;
      }

      if (call->IsStatic()) {
        // For static pure invoke, it is enough to check that the result is unused.
        // TODO Check carefully do we have problems with class init before adding
        // static methods to the pure list.
        if (!insn->HasUses()) {
          PRINT_PASS_OSTREAM_MESSAGE(this, "Eliminated static invoke " << call
                                     << " of pure method "
                                     << CalledMethodName(call));
          MaybeRecordStat(MethodCompilationStat::kIntelPureStaticCallDeleted);
          block->RemoveInstruction(call);
        }
      } else {
        // The argument should not be null to prove that this invoke cannot throw
        // NullPointerException.
        HInstruction* callee_object = call->InputAt(0);
        if (CanReturnNull(callee_object)) {
          continue;
        }

        if (!insn->HasUses()) {
          PRINT_PASS_OSTREAM_MESSAGE(this, "Eliminated direct invoke " << call
                                     << " of pure method "
                                     << CalledMethodName(call)
                                     << " because it is called for not-null object "
                                     << callee_object);
          MaybeRecordStat(MethodCompilationStat::kIntelPureDirectCallDeleted);
          block->RemoveInstruction(call);
        } else if (block_is_in_loop) {
          PRINT_PASS_OSTREAM_MESSAGE(this, "Found candidate for pure invoke hoisting: " << call);
          // This invoke is a potential candidate for hoisting.
          hoisting_candidates.push_back(call);
        }
      }
    }
  }

  return !hoisting_candidates.empty();
}

}  // namespace art
