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

#include "insert_profiling.h"

#include "driver/compiler_driver.h"
#include "driver/compiler_options.h"
#include "ext_utility.h"
#include "ext_profiling.h"
#include "graph_x86.h"

namespace art {

void HInsertProfiling::Run() {
  // Is there anything to do?
  CompilerOptions::ProfilingCounts profiling_counts =
      driver_->GetCompilerOptions().GetProfilingCounts();
  if (profiling_counts == CompilerOptions::kProfilingNone) {
    return;  // nothing to do.
  }

  PRINT_PASS_OSTREAM_MESSAGE(this, "Start " << GetMethodName(graph_));
  ArenaAllocator* arena = graph_->GetArena();

  // Walk the blocks, and insert profiling code.  Do this for original blocks
  // formed from the Dex code only.
  std::set<uint32_t> dex_pcs_seen;
  HX86ReturnExecutionCountTable* count_table = nullptr;
  int32_t max_profiled_block = -1;
  for (HBasicBlock* block : graph_->GetBlocks()) {
    if (block == nullptr) {
      continue;
    }

    int block_id = block->GetBlockId();

    if (block == graph_->GetExitBlock()) {
      // We don't profile the exit block.
      continue;
    }

    // We know we have to profile block 0 and 1, and they will have a duplicate dex_pc.
    bool is_entry_block = block == graph_->GetEntryBlock();
    DCHECK_EQ(is_entry_block, block_id == 0);
    if (block_id < 2) {
      if (is_entry_block) {
        // Need to address the profile area for this method.
        bool is_64_bit = Is64BitInstructionSet(graph_->GetInstructionSet());
        count_table = new(arena) HX86ReturnExecutionCountTable(is_64_bit,
                                                               graph_->GetCurrentMethod(),
                                                               0);
        PRINT_PASS_OSTREAM_MESSAGE(this, "Insert return count table into block " << block_id);
        block->InsertInstructionBefore(count_table, block->GetLastInstruction());
      }

      DCHECK(count_table != nullptr);
      HInstruction* increment =
        new(arena) HX86IncrementExecutionCount(block_id, count_table, block->GetDexPc());
      if (is_entry_block) {
        block->InsertInstructionAfter(increment, count_table);
      } else {
        HInstruction* insn = block->GetFirstInstruction();
        DCHECK(!insn->IsLoadException());
        if (insn->IsSuspendCheck()) {
          insn = insn->GetNext();
        }
        block->InsertInstructionBefore(increment, insn);
      }
      max_profiled_block = std::max(max_profiled_block, block_id);
      PRINT_PASS_OSTREAM_MESSAGE(this, "Insert increment into block " << block_id);
      if (is_entry_block && profiling_counts == CompilerOptions::kProfilingMethod) {
        // All done.
        break;
      }
      dex_pcs_seen.insert(0);
      continue;
    }

    // Have we seen this block before?
    uint32_t dex_pc = block->GetDexPc();
    if (dex_pc == kNoDexPc) {
      // Give up now.
      PRINT_PASS_OSTREAM_MESSAGE(this, "Exiting: seen kNoDexPc in block " << block_id);
      break;
    }

    if (dex_pcs_seen.find(dex_pc) != dex_pcs_seen.end()) {
      // We have a duplicate block.  Is this the special case of a try block?
      HInstruction* insn = block->GetFirstInstruction();
      if (insn->IsSuspendCheck()) {
        insn = insn->GetNext();
      }
      if (insn->IsTryBoundary()) {
        PRINT_PASS_OSTREAM_MESSAGE(this, "Seen TryBoundary in block " << block_id);
        continue;
      }
      PRINT_PASS_OSTREAM_MESSAGE(this, "Saw duplicate dex_pc in block " << block_id);
      break;
    }

    HInstruction* increment =
        new(arena) HX86IncrementExecutionCount(block_id, count_table, block->GetDexPc());
    // We have to be careful about catch blocks, as there are assumptions made
    // about instruction ordering.
    HInstruction* insn = block->GetFirstInstruction();
    if (insn->IsLoadException()) {
      // We can't insert the increment until after the ClearException.
      while (insn != nullptr && !insn->IsClearException()) {
        insn = insn->GetNext();
      }
    } else if (insn->IsSuspendCheck()) {
      insn = insn->GetNext();
    }
    DCHECK(insn != nullptr);
    block->InsertInstructionBefore(increment, insn);
    max_profiled_block = std::max(max_profiled_block, block_id);
    PRINT_PASS_OSTREAM_MESSAGE(this, "Insert increment into block " << block_id);
    dex_pcs_seen.insert(dex_pc);
  }

  GRAPH_TO_GRAPH_X86(graph_)->SetNumProfiledBlocks(max_profiled_block+1);
  PRINT_PASS_OSTREAM_MESSAGE(this, "End " << GetMethodName(graph_));
}

}
