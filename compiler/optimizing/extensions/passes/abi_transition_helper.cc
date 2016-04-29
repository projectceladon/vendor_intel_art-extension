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

#include "abi_transition_helper.h"
#include "driver/compiler_driver-inl.h"
#include "ext_utility.h"
#include "graph_x86.h"
#include "pass_option.h"

namespace art {

void HAbiTransitionHelper::Run() {
  CollectStats();
}

void HAbiTransitionHelper::CollectStats() const {
  if (stats_ == nullptr) {
    // No point in collecting stats if there is no place to aggregate them.
    return;
  }

  const bool cannot_use_dex_cache = CannotUseDexCache();
  for (HReversePostOrderIterator it(*graph_); !it.Done(); it.Advance()) {
    HBasicBlock* block = it.Current();
    for (HInstructionIterator inst_it(block->GetInstructions()); !inst_it.Done();
        inst_it.Advance()) {
      HInstruction* instr = inst_it.Current();

      if (instr->IsCurrentMethod()) {
        DCHECK_EQ(instr->GetEnvUses().SizeSlow(), 0u);
        const HUseList<HInstruction*>& uses = instr->GetUses();
        for (auto it2 = uses.begin(), end = uses.end(); it2 != end; ++it2) {
          HInstruction* user = it2->GetUser();

          switch(user->GetKind()) {
            case HInstruction::kNewInstance:
              MaybeRecordStat(kCurrentMethodUserNewInstance);
              break;
            case HInstruction::kNewArray:
              MaybeRecordStat(kCurrentMethodUserNewArray);
              break;
            case HInstruction::kInvokeStaticOrDirect:
              MaybeRecordStat(kCurrentMethodUserInvoke);
              break;
            case HInstruction::kLoadClass:
              MaybeRecordStat(kCurrentMethodUserLoadClass);
              break;
            case HInstruction::kLoadString:
              MaybeRecordStat(kCurrentMethodUserLoadString);
              break;
            default:
              MaybeRecordStat(kCurrentMethodUserOther);
              break;
          }
        }
      }

      if (instr->IsInvokeStaticOrDirect() && !instr->AsInvoke()->IsIntrinsic()) {
        HInvokeStaticOrDirect::DispatchInfo dispatch_info =
            instr->AsInvokeStaticOrDirect()->GetDispatchInfoUnchecked();
        switch (dispatch_info.method_load_kind) {
          case HInvokeStaticOrDirect::MethodLoadKind::kStringInit:
          case HInvokeStaticOrDirect::MethodLoadKind::kDirectAddress:
          case HInvokeStaticOrDirect::MethodLoadKind::kDirectAddressWithFixup:
            MaybeRecordStat(kMethodLoadViaDirectPtr);
            break;
          case HInvokeStaticOrDirect::MethodLoadKind::kRecursive:
          case HInvokeStaticOrDirect::MethodLoadKind::kDexCacheViaMethod:
            MaybeRecordStat(kMethodLoadViaCurrentMethod);
            break;
          case HInvokeStaticOrDirect::MethodLoadKind::kDexCachePcRelative:
            if (cannot_use_dex_cache) {
              MaybeRecordStat(kMethodLoadViaCurrentMethod);
            } else {
              MaybeRecordStat(kMethodLoadViaDexCache);
            }
            break;
          default:
            // Only fail in debug builds because not being able to report stats is not fatal.
            if (kIsDebugBuild) {
              LOG(FATAL) << "Unhandled method load kind " << dispatch_info.method_load_kind;
            }
            break;
        }
      }
    }
  }
}

bool HAbiTransitionHelper::CannotUseDexCache() const {
  return graph_->HasIrreducibleLoops() && compiler_driver_->GetInstructionSet() == kX86;
}

}  // namespace art
