/*
 * Copyright (C) 2016 Intel Corporation.
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

#include "binary_analyzer_x86.h"

#include <inttypes.h>
#include <iostream>
#include <ostream>
#include <sstream>

#include "base/logging.h"
#include "base/stringprintf.h"
#include "thread.h"

namespace art {
namespace x86 {

// Budgetary constraints for classifying a native method as fast.
static constexpr size_t kCallDepthLimit = 3;
static constexpr size_t kBasicBlockLimit = 20;
static constexpr size_t kInstructionLimit = 100;

enum ctrl_transfer_type {
  kNone,                 // Instructions with no control flow transfer.
  kConditionalBranch,    // Conditional branch.
  kUnconditionalBranch,  // Unconditional branch.
  kInterrupt,            // Software Interrupt.
  kCall,                 // Direct call.
  kUnknown,              // Unsupported instruction.
  kIndirect,             // Indirect call.
  kReturn                // Return instruction.
};

/**
 * @brief Analyze the instruction & update the characteristics of the method.
 * CFG for a given method is not simple if one or more instructions have -
 * 1. Unknown opcode.
 * 2. Indirect call/jmp.
 * 3. Interrupt.
 * 4. rep, repne and lock prefix.
 * 5. Loops (Any cycle in the CFG).
 * @param instr - The instruction pointer.
 * @param curr_bb - Pointer to the Current Basic Block.
 * @param is_bb_end - Does the instruction mark the end of Basic Block.
 * @param target_disp - Relative target of a branch.
 * @param cfg - Pointer to the CFG.
 * @return Size of analyzed instruction in bytes. -1 in case of error.
 */
size_t BinaryAnalyzer::AnalyzeInstruction(const uint8_t* instr,
                                          MachineBlock* curr_bb,
                                          int32_t* is_bb_end,
                                          int32_t* target_disp,
                                          CFGraph* cfg) {
  *is_bb_end = kNone;
  if (!disassembler_->IsDisassemblerValid()) {
    return -1;
  }
  disassembler_->Seek(instr);
  const cs_insn* insn = disassembler_->Next();
  const cs_x86& insn_x86 = insn->detail->x86;
  switch(insn->id) {
  case X86_PREFIX_REP:
  case X86_PREFIX_REPNE:
  case X86_PREFIX_LOCK:
  case X86_INS_INVALID:
    cfg->SetNotSimple();
    break;
  case X86_INS_INT:
  case X86_INS_INT1:
  case X86_INS_INT3:
    cfg->SetNotSimple();
    *is_bb_end = kInterrupt;
    break;
  case X86_INS_JMP:
    *is_bb_end = kUnconditionalBranch;
    if (insn_x86.operands[0].type != X86_OP_IMM) {
      *is_bb_end = kIndirect;
    }
    break;
  case X86_INS_JA:
  case X86_INS_JAE:
  case X86_INS_JB:
  case X86_INS_JBE:
  case X86_INS_JCXZ:
  case X86_INS_JE:
  case X86_INS_JECXZ:
  case X86_INS_JG:
  case X86_INS_JGE:
  case X86_INS_JL:
  case X86_INS_JLE:
  case X86_INS_JNE:
  case X86_INS_JNO:
  case X86_INS_JNP:
  case X86_INS_JNS:
  case X86_INS_JO:
  case X86_INS_JP:
  case X86_INS_JRCXZ:
  case X86_INS_JS:
    *is_bb_end = kConditionalBranch;
    if (insn_x86.operands[0].type != X86_OP_IMM) {
      *is_bb_end = kIndirect;
    }
    break;
  case X86_INS_RET:
    *is_bb_end = kReturn;
    break;
  case X86_INS_CALL:
    *is_bb_end = kCall;
    if (insn_x86.operands[0].type != X86_OP_IMM) {
      *is_bb_end = kIndirect;
    }
    break;
  }
  *target_disp = insn_x86.disp;
  MachineInstruction* ir = new MachineInstruction(insn->op_str, (uint8_t) (insn->size),
                                                  reinterpret_cast<const uint8_t*>(instr));
  MachineInstruction* prev_ir = curr_bb->GetLastInstruction();
  ir->SetPrevInstruction(prev_ir);
  if (prev_ir != nullptr) {
    prev_ir->SetNextInstruction(ir);
  }
  curr_bb->AddInstruction(ir);
  return insn->size;
}

MachineBlock* CFGraph::GetCorrectBB(MachineBlock* bblock) {
  if (bblock->IsDummy()) {
    if (!bblock->GetPredBBlockList().empty()) {
      return bblock->GetPredBBlockList().front();
    } else {
      return nullptr;
    }
  } else {
    return bblock;
  }
}

bool CFGraph::IsVisited(const uint8_t* addr,
                        MachineBlock* prev_bblock,
                        MachineBlock* succ_bblock,
                        std::list<BackLogDs*>* backlog) {
  for (std::list<MachineBlock*>::iterator it = visited_bblock_list_.begin();
      (it != visited_bblock_list_.end());
      it++) {
    if (!(*it)->IsDummy()) {
      const uint8_t* start = (*it)->GetStartAddr();
      const uint8_t* end = (*it)->GetEndAddr();

      // In case we are branching to the beginning of an existing Basic Block,
      // it is already visited.
      if (addr == start) {
        MachineBlock* bb_existing = (*it);
        if (prev_bblock->IsDummy()) {
          prev_bblock->AddSuccBBlock(bb_existing);
          bb_existing->AddPredBBlock(prev_bblock);
          succ_bblock->AddPredBBlock(bb_existing);
          bb_existing->AddSuccBBlock(succ_bblock);
        } else {
          prev_bblock->AddSuccBBlock(bb_existing);
          bb_existing->AddPredBBlock(prev_bblock);
        }
        this->SetNotSimple();
        return true;
      }

      // In case we are branching to some address in the middle of an existing Basic Block,
      // then that Basic Block needs to be split.
      if ((addr > start) && (addr < end)) {
        MachineBlock* bb_to_be_split = (*it);

        const uint8_t* prev_instr = nullptr;
        const uint8_t* curr_instr = start;

        // Scan the bblock that is being split to identify the end instruction.
        std::list<MachineInstruction*> instructions = bb_to_be_split->GetInstructions();

        for (std::list<MachineInstruction*>::iterator it_instr = instructions.begin();
            it_instr != instructions.end();
            it_instr++) {
          prev_instr = curr_instr;
          curr_instr = reinterpret_cast<const uint8_t*>(curr_instr + (*it_instr)->GetLength());

          // The previous instruction is the last instruction of the Basic Block being split.
          if (curr_instr == addr) {
            // Change the last instruction.
            bb_to_be_split->SetEndAddr(prev_instr);
            MachineBlock* new_bb = CreateBBlock(nullptr);
            std::list<MachineBlock*> succ_list = bb_to_be_split->GetSuccBBlockList();
            bb_to_be_split->ClearSuccBBlockList();
            bb_to_be_split->AddSuccBBlock(new_bb);
            new_bb->AddPredBBlock(bb_to_be_split);
            new_bb->CopySuccBBlockList(succ_list);
            if (prev_bblock->IsDummy()) {
              prev_bblock->AddSuccBBlock(new_bb);
              new_bb->AddPredBBlock(prev_bblock);
              succ_bblock->AddPredBBlock(new_bb);
              new_bb->AddSuccBBlock(succ_bblock);
            } else {
              prev_bblock->AddSuccBBlock(new_bb);
              new_bb->AddPredBBlock(prev_bblock);
            }
            AddTuple(new_bb, reinterpret_cast<const uint8_t*>(addr), end);
            new_bb->CopyInstruction(bb_to_be_split, addr);
            ChangePredForBacklog(bb_to_be_split, new_bb, backlog);
            ChangePredecessors(succ_list, bb_to_be_split, new_bb);
            if (prev_bblock == bb_to_be_split) {
              new_bb->AddPredBBlock(new_bb);
              new_bb->AddSuccBBlock(new_bb);
            }
            return true;
          } else if (curr_instr > addr) {
            return false;
          }
        }
      }
    }
  }
  // Else, we have not visited this earlier.
  return false;
}

void CFGraph::ChangePredecessors(std::list<MachineBlock*> bblock_list,
                                 MachineBlock* bblock_to_be_deleted,
                                 MachineBlock* bblock_to_be_added) {
  for (const auto& it : bblock_list) {
    it->DeletePredBBlock(bblock_to_be_deleted);
    it->AddPredBBlock(bblock_to_be_added);
  }
}

void CFGraph::ChangePredForBacklog(MachineBlock* old_pred,
                                   MachineBlock* new_pred,
                                   std::list<BackLogDs*>* backlog) {
  for (std::list<BackLogDs*>::iterator it = backlog->begin();
      it != backlog->end();
      it++) {
    if ((*it)->pred_bb->GetId() == old_pred->GetId()) {
      (*it)->pred_bb = new_pred;
      return;
    }
  }
}

void MachineBlock::CopyInstruction(MachineBlock* from_bblock, const uint8_t* start) {
  const uint8_t* ptr = from_bblock->GetStartAddr();
  std::list<MachineInstruction*>::iterator it_bb = from_bblock->instrs_.begin();
  while (it_bb != from_bblock->instrs_.end()) {
    if (ptr >= start) {
      AddInstruction((*it_bb));
      ptr += (*it_bb)->GetLength();
      from_bblock->DeleteInstruction(it_bb++);
    } else {
      ptr += (*it_bb)->GetLength();
      it_bb++;
    }
  }
}

void MachineBlock::DeleteInstruction(std::list<MachineInstruction*>::iterator it) {
  instrs_.erase(it);
  --num_of_instrs_;
}

bool CFGraph::IsVisitedForBacklog(struct BackLogDs* entry, std::list<BackLogDs*>* backlog) {
  if (entry != nullptr) {
    MachineBlock* prev_bb = entry->pred_bb;
    const uint8_t* addr = entry->ptr;

    if (prev_bb != nullptr) {
      return IsVisited(addr, prev_bb, entry->succ_bb, backlog);
    }
  }
  return false;
}

MachineBlock* GetTailBB(MachineBlock* bblock) {
  MachineBlock* first_succ_bb = bblock->GetSuccBBlockList().front();
  if (first_succ_bb == nullptr) {
    return bblock;
  } else {
    return first_succ_bb;
  }
}

void BinaryAnalyzer::CFGHelper(CFGraph* cfg,
                               const uint8_t* instr_ptr,
                               MachineBlock* curr_bblock,
                               std::list<BackLogDs*>* backlog,
                               uint32_t depth,
                               MachineBlock* dummy_end) {
  size_t len = 0;
  int32_t is_bb_end = kNone;
  int32_t displacement = 0;
  const uint8_t* ptr = reinterpret_cast<const uint8_t*>(instr_ptr);
  const uint8_t* start_ptr = reinterpret_cast<const uint8_t*>(ptr);

  while ((len = BinaryAnalyzer::AnalyzeInstruction(ptr, curr_bblock, &is_bb_end, &displacement, cfg)) > 0) {
    switch (is_bb_end) {
    case kUnconditionalBranch: {
      // Push the jmp target to backlog.
      struct BackLogDs* uncond_jmp = new BackLogDs();
      uncond_jmp->pred_bb = curr_bblock;
      if (depth > 0) {
        uncond_jmp->succ_bb = dummy_end;
      } else {
        uncond_jmp->succ_bb = nullptr;
      }
      uncond_jmp->ptr = reinterpret_cast<const uint8_t*>(ptr + displacement + len);
      uncond_jmp->call_depth = 0;
      backlog->push_back(uncond_jmp);
      // Add the current BB to CFG.
      cfg->AddTuple(curr_bblock, start_ptr, reinterpret_cast<const uint8_t*>(ptr));
      break;
    }
    case kConditionalBranch: {
      // Push the conditional (if) jmp target to backlog.
      struct BackLogDs* cond_if_jmp = new BackLogDs();
      cond_if_jmp->pred_bb = curr_bblock;
      if (depth > 0) {
        cond_if_jmp->succ_bb = dummy_end;
      } else {
        cond_if_jmp->succ_bb = nullptr;
      }
      cond_if_jmp->ptr = reinterpret_cast<const uint8_t*>(ptr + displacement + len);
      cond_if_jmp->call_depth = 0;
      backlog->push_back(cond_if_jmp);
      // Push the else (subsequent instruction) to the backlog.
      struct BackLogDs* cond_else_jmp = new BackLogDs();
      cond_else_jmp->pred_bb = curr_bblock;
      if (depth > 0) {
        cond_else_jmp->succ_bb = dummy_end;
      } else {
        cond_else_jmp->succ_bb = nullptr;
      }
      cond_else_jmp->ptr = reinterpret_cast<const uint8_t*>(ptr + len);
      cond_else_jmp->call_depth = 0;
      backlog->push_back(cond_else_jmp);
      // Add the current Basic Block to the CFG.
      cfg->AddTuple(curr_bblock, start_ptr, reinterpret_cast<const uint8_t*>(ptr));
      break;
    }
    case kCall: {
      // Add the current Basic Block to the CFG.
      cfg->AddTuple(curr_bblock, start_ptr, reinterpret_cast<const uint8_t*>(ptr));
      MachineBlock* start_bb = cfg->CreateBBlock(curr_bblock);
      MachineBlock* end_bb = cfg->CreateBBlock(nullptr);
      start_bb->SetDummy();
      end_bb->SetDummy();
      struct BackLogDs* call_entry = new BackLogDs();
      call_entry->pred_bb = start_bb;
      call_entry->ptr = reinterpret_cast<const uint8_t*>(ptr + displacement + len);
      call_entry->succ_bb = end_bb;
      call_entry->call_depth = depth + 1;
      if (call_entry->call_depth > cfg->GetCallDepth()) {
        cfg->SetCallDepth(call_entry->call_depth);
      }
      backlog->push_back(call_entry);
      MachineBlock* bb_after_call = cfg->CreateBBlock(end_bb);
      curr_bblock = bb_after_call;
      is_bb_end = kNone;
      start_ptr = ptr + len;
      displacement = 0;
      cfg->AddTuple(start_bb, nullptr, nullptr);
      cfg->AddTuple(end_bb, nullptr, nullptr);
      break;
    }
    case kReturn: {
      if (dummy_end != nullptr) {
        dummy_end->AddPredBBlock(curr_bblock);
        curr_bblock->AddSuccBBlock(dummy_end);
      }
      cfg->AddTuple(curr_bblock, start_ptr, reinterpret_cast<const uint8_t*>(ptr));
      break;
    }
    case kInterrupt: {
      cfg->AddTuple(curr_bblock, start_ptr, reinterpret_cast<const uint8_t*>(ptr));
      break;
    }
    case kNone:
      break;
    default:
      cfg->SetHasUnknownInstructions();
      cfg->AddTuple(curr_bblock, start_ptr, reinterpret_cast<const uint8_t*>(ptr));
      break;
    }

    ptr += len;
    if (is_bb_end == kNone || is_bb_end == kCall) {
      continue;
    } else {
      break;
    }
  }
}

CFGraph* BinaryAnalyzer::BuildCFG(const uint8_t* ptr, std::string method_name) {
  CFGraph* cfg = new CFGraph(method_name);
  MachineBlock* predecessor_bb = nullptr;
  MachineBlock* start_bb = cfg->CreateBBlock(predecessor_bb);
  MachineBlock* curr_bb = start_bb;
  cfg->AddStartBBlock(start_bb);
  MachineBlock* dummy_end = nullptr;
  uint32_t depth = 0;
  std::list<BackLogDs*> backlog;
  CFGHelper(cfg, ptr, curr_bb, &backlog, depth, dummy_end);
  do {
    BackLogDs* entry = nullptr;
    if (!backlog.empty()) {
      entry = backlog.back();
      backlog.pop_back();
      if (!cfg->IsVisitedForBacklog(entry, &backlog)) {
        ptr = entry->ptr;
        predecessor_bb  = entry->pred_bb;
        curr_bb = cfg->CreateBBlock(predecessor_bb);
        dummy_end = entry->succ_bb;
        depth = entry->call_depth;
        CFGHelper(cfg, ptr, curr_bb, &backlog, depth, dummy_end);
      }
    }
    delete entry;
    if (!cfg->IsWithinBudget()) {
      for (auto& e : backlog) {
        delete e;
      }
      delete cfg;
      return nullptr;
    }
  } while ((!backlog.empty()));
  cfg->AddEndBBlock(curr_bb);
  return cfg;
}

bool CFGraph::IsWithinBudget() {
  return ((call_depth_ < kCallDepthLimit)
      && (num_of_bblocks_ <= kBasicBlockLimit)
      && (num_of_instrs_ <= kInstructionLimit)
      && !has_unknown_instructions_
      && !has_indirect_calls_);
}

void MachineInstruction::Print(std::ostream& os, bool is_dot) {
  os << Print(is_dot).str();
}

void ReplaceString(std::string& subject,
                   const std::string& search,
                   const std::string& replace) {
  size_t pos = 0;
  while ((pos = subject.find(search, pos)) != std::string::npos) {
    subject.replace(pos, search.length(), replace);
    pos += replace.length();
  }
}

std::ostringstream MachineInstruction::Print(bool is_dot) {
  std::ostringstream os;
  if (is_dot) {
    ReplaceString(instr_, "0x", "");
    os << instr_ << "\\l";
  } else {
    os << "\t\t" << instr_ << std::endl;
  }
  return os;
}

void MachineBlock::Print(std::ostream& os, bool is_dot) {
  os << Print(is_dot).str();
}

std::ostringstream MachineBlock::Print(bool is_dot) {
  std::ostringstream os;
  if (is_dot) {
    std::string label = StringPrintf("BB#%d\\n", id_);
    if (!is_dummy_) {
      std::ostringstream o_instr;
      for (auto it : instrs_) {
        it->Print(o_instr, true);
      }
      label = label + o_instr.str();
    } else {
      label = label + " (Dummy)";
    }
    os << StringPrintf("\nB_%d [shape=rectangle, label=\"%s\"];", id_, label.c_str());
    for (std::list<MachineBlock*>::iterator it = succ_bblock_.begin();
        it != succ_bblock_.end();
        it++) {
      os << StringPrintf("\nB_%d -> B_%d;", id_, (*it)->GetId());
    }
  } else {
    os << "   Basic Block Id : " << id_
        << "\n BB -No. of instructions " << GetInstrCnt()
        << "\n    List of predecessor BBs : ";
    for (auto it : pred_bblock_) {
      os << it->GetId() << "   ";
    }
    os << "\n    List of successor BBs : ";
    for (auto it : succ_bblock_) {
      os << it->GetId() << "   ";
    }
    if (!is_dummy_) {
      os << "\n   Instructions begin at : " << StringPrintf("%p", start_addr_) << "\n";
      for (auto it : instrs_) {
        it->Print(os, false);
      }
      os << "   Instructions end at : " << StringPrintf("%p", end_addr_) << "\n";
    } else {
      os << "\n Dummy BB \n";
    }
  }
  return os;
}

void CFGraph::Print(std::ostringstream& os, bool is_dot) {
  if (is_dot) {
    std::string method_name = GetMethodName();
    ReplaceString(method_name, ".", "_");
    ReplaceString(method_name, " ", "_");
    ReplaceString(method_name, ",", "_");
    ReplaceString(method_name, ")", "_");
    ReplaceString(method_name, "(", "_");
    os << "\ndigraph G_" << method_name <<" {";
  } else {
    os << "--- CFG begins ---\nCFG -No. of instructions " << GetInstructionCnt();
  }
  for (auto it = visited_bblock_list_.begin();
      it != visited_bblock_list_.end();
      it++) {
    (*it)->Print(os, is_dot);
  }
  if (is_dot) {
    os << std::endl << "}";
  } else {
    os << "--- CFG ends ---\n";
  }
}

void MachineBlock::Visit(std::set<MachineBlock*>* bb_set, std::list<MachineBlock*>* bb_order) {
  for (auto it = succ_bblock_.begin();
      it != succ_bblock_.end();
      it++) {
    if (bb_set->find(*it) != bb_set->end()) {
      (*it)->Visit(bb_set, bb_order);
    }
  }
  for (auto it = pred_bblock_.begin(); it != pred_bblock_.end(); it++) {
    if (*bb_set->find(*it) != *bb_set->end()) {
      (*it)->Visit(bb_set, bb_order);
    }
  }
  bb_set->insert(this);
  bb_order->push_back(this);
}

MachineBlock::~MachineBlock() {
  for (auto it : instrs_) {
    delete it;
  }
}

bool BinaryAnalyzer::AnalyzeMethod(uint32_t method_idx, const DexFile& dex_file, const void* fnPtr) {
  bool is_fast = false;
  if (cfg_ != nullptr) {
    delete cfg_;
    cfg_ = nullptr;
  }
  cfg_ = BuildCFG((unsigned char*) fnPtr, PrettyMethod(method_idx, dex_file));
  if (cfg_ != nullptr) {
    is_fast = cfg_->IsSimple();
  }
  return is_fast;
}

void MachineBlock::AddPredBBlock(MachineBlock* bblock) {
  auto it = std::find(pred_bblock_.begin(), pred_bblock_.end(), bblock);
  if (it == pred_bblock_.end()) {
    pred_bblock_.push_back(bblock);
  }
}

void MachineBlock::AddSuccBBlock(MachineBlock* bblock) {
  auto it = std::find(succ_bblock_.begin(), succ_bblock_.end(), bblock);
  if (it == succ_bblock_.end()) {
    succ_bblock_.push_back(bblock);
  }
}

void MachineBlock::DeletePredBBlock(MachineBlock* bblock) {
  for (auto it = pred_bblock_.begin();
      it != pred_bblock_.end();) {
    if ((*it) == bblock) {
      it = pred_bblock_.erase(it);
    } else {
      it++;
    }
  }
}

void MachineBlock::CopyPredBBlockList(std::list<MachineBlock*> copy_list) {
  for (auto it : copy_list) {
    pred_bblock_.push_back(it);
  }
}

void MachineBlock::CopySuccBBlockList(std::list<MachineBlock*> copy_list) {
  for (auto it : copy_list) {
    succ_bblock_.push_back(it);
  }
}

MachineBlock* CFGraph::CreateBBlock(MachineBlock* predecessor_bb) {
  MachineBlock* new_bb = new MachineBlock(predecessor_bb);
  new_bb->SetId(num_of_bblocks_);
  new_bb->SetStartAddr(nullptr);
  new_bb->SetEndAddr(nullptr);
  ++num_of_bblocks_;
  cfg_bblock_list_.push_back(new_bb);
  return new_bb;
}

void CFGraph::AddTuple(MachineBlock* bblock, const uint8_t* start, const uint8_t* end) {
  bblock->SetStartAddr(start);
  bblock->SetEndAddr(end);
  visited_bblock_list_.push_back(bblock);
  num_of_instrs_ += bblock->GetInstrCnt();
}

}  // namespace x86
}  // namespace art
