/*
 * Copyright (C) 2016 The Android Open Source Project
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

#ifndef ART_RUNTIME_BINARY_ANALYZER_BINARY_ANALYZER_X86_H_
#define ART_RUNTIME_BINARY_ANALYZER_BINARY_ANALYZER_X86_H_

#include <iostream>
#include <list>
#include <ostream>
#include <set>
#include <string>
#include <sys/uio.h>
#include <tuple>

#include "art_field-inl.h"
#include "art_method-inl.h"
#include "base/logging.h"
#include "disassembler.h"
#include "mirror/class-inl.h"
#include "mirror/class_loader.h"
#include "mirror/object-inl.h"
#include "mirror/object_array-inl.h"
#include "mirror/string-inl.h"
#include "mirror/throwable.h"
#include "utils/assembler.h"

namespace art {
namespace x86 {

/**
 * @brief The instruction class stores information about
 * the decoded/analyzed binary instruction.
 */
class MachineInstruction {
 public:
  MachineInstruction(std::string instruction_str, uint8_t length, const uint8_t* ptr)
      : instr_(instruction_str), instr_ptr_(ptr), length_(length) {
  }

  ~MachineInstruction() {}

  /**
   * @brief prints the decoded x86 instruction in human readable/dot format to logcat.
   * @param output - output stream.
   * @param is_dot - whether the output must be in dot format.
   */
  void Print(std::ostream& output, bool is_dot);

  /**
   * @brief prints the decoded x86 instruction in human readable/dot format.
   * @param is_dot - whether the output must be in dot format.
   * @return the output stream.
   */
  std::ostringstream Print(bool is_dot);

  /**
   * @brief returns the human readable x86 instruction.
   * @return the decoded x86 instruction.
   */
  inline std::string GetInstruction() {
    return instr_;
  }

  /**
   * @brief Get the number of bytes of assembly instruction.
   * @return the number of bytes the instruction consumes.
   */
  inline uint8_t GetLength() const {
    return length_;
  }

  /**
   * @brief Get the pointer to the instruction.
   * @return the byte pointer to the x86 assembly code.
   */
  inline const uint8_t* GetInstructionPtr() const {
    return instr_ptr_;
  }

  /**
   * @brief Get the previous Instruction pointer.
   * @return the previous instruction's pointer.
   */
  inline const MachineInstruction* GetPrevInstruction() {
    return prev_instr_;
  }

  /**
   * @brief Get the next Instruction pointer.
   * @return the next instruction's pointer.
   */
  inline MachineInstruction* GetNextInstruction() {
    return next_instr_;
  }

  /**
   * @brief Set the previous instruction pointer for the current Instruction.
   * @param prev the previous instruction pointer.
   */
  inline void SetPrevInstruction(MachineInstruction* prev) {
    prev_instr_ = prev;
  }

  /**
   * @brief Set the next instruction pointer for the current Instruction.
   * @param next the next instruction pointer.
   */
  inline void SetNextInstruction(MachineInstruction* next) {
    next_instr_ = next;
  }

 private:
  std::string instr_;
  const uint8_t* instr_ptr_;
  uint8_t length_;
  MachineInstruction* prev_instr_;
  MachineInstruction* next_instr_;
};

/**
 * @brief BBlock class contains information about a Basic Block.
 * Note : A dummy basic block is the one that connects a basic block
 * that ends with a call instruction to the starting of the call site
 * Basic Block. A dummy basic block does not contain any instructions.
 * It merely serves as a link.
 */
class MachineBlock {
 public:
  explicit MachineBlock(MachineBlock* pred_bb)
      : num_of_instrs_(0),
        is_dummy_(false) {
    if (pred_bb != nullptr) {
      AddPredBBlock(pred_bb);
      pred_bb->AddSuccBBlock(this);
    }
  }

  ~MachineBlock();

  /**
   * @brief Get the last Instruction class pointer for the Basic Block.
   * @return the pointer to the last Instruction of the Basic Block.
   */
  inline MachineInstruction* GetLastInstruction() {
    if (!instrs_.empty()) {
      return instrs_.back();
    }
    return nullptr;
  }

  /**
   * @brief Set the starting address of the Basic Block.
   * @param start - Starting address of the Basic Block.
   */
  inline void SetStartAddr(const uint8_t* start) {
    start_addr_ = start;
  }

  /**
   * @brief Get the starting address of the Basic Block.
   * @return Starting address of the Basic Block.
   */
  inline const uint8_t* GetStartAddr() const {
    return start_addr_;
  }

  /**
   * @brief Set the ending address of the Basic Block.
   * @param end - Ending address of the Basic Block.
   */
  inline void SetEndAddr(const uint8_t* end) {
    end_addr_ = end;
  }

  /**
   * @brief Get the ending address of the Basic Block.
   * @return Ending address of the Basic Block.
   */
  inline const uint8_t* GetEndAddr() const {
    return end_addr_;
  }

  /**
   * @brief Set this Basic Block as Dummy.
   */
  inline void SetDummy() {
    is_dummy_ = true;
  }

  /**
   * @brief Is this Basic Block a dummy.
   * @return whether the basic block is a dummy or not.
   */
  inline bool IsDummy() const {
    return is_dummy_;
  }

  /**
   * @brief Get the number of Instructions.
   * @return number of Instructions in the Basic Block.
   */
  inline uint32_t GetInstrCnt() const {
    return num_of_instrs_;
  }

  /**
   * @brief Get The Id of this Basic Block.
   * @return the Basic Block's id.
   */
  inline uint32_t GetId() const {
    return id_;
  }

  /**
   * @brief Set the Basic Block's Id.
   * @param - the Id for the Basic Block.
   */
  inline void SetId(uint32_t id) {
    id_ = id;
  }

  /**
   * @brief Add the Instruction to the Basic Block.
   * @param instruction - The instruction to be added to the Basic Block.
   */
  inline void AddInstruction(MachineInstruction* instruction) {
    instrs_.push_back(instruction);
    ++num_of_instrs_;
  }

  /**
   * @brief Add a Basic Block to list of predecessor Basic Blocks.
   * @param bblock - the predecessor Basic Block.
   */
  void AddPredBBlock(MachineBlock* bblock);

  /**
   * @brief Add a Basic Block to list of successor Basic Blocks.
   * @param bblock - the successor Basic Block.
   */
  void AddSuccBBlock(MachineBlock* bblock);

  /**
   * @brief Delete a certain Basic Block from the list of predecessor Basic Blocks.
   * @param bblock - the Basic Block to be deleted.
   */
  void DeletePredBBlock(MachineBlock* bblock);

  /**
   * Get the list of predecessor Basic Blocks.
   * @return the predecessor Basic Block List.
   */
  inline std::list<MachineBlock*> GetPredBBlockList() {
    return pred_bblock_;
  }

  /**
   * Get the list of successor Basic Blocks.
   * @return the successor Basic Block List.
   */
  inline std::list<MachineBlock*> GetSuccBBlockList() {
    return succ_bblock_;
  }

  /**
   * @brief Make a copy of the list of predecessor Basic Blocks.
   * @param copy_list - The list that will contain a copy of predecessor Basic Blocks.
   */
  void CopyPredBBlockList(std::list<MachineBlock*> copy_list);

  /**
   * @brief Make a copy of the list of successor Basic Blocks.
   * @param copy_list - The list that will contain a copy of successor Basic Blocks.
   */
  void CopySuccBBlockList(std::list<MachineBlock*> copy_list);

  /**
   * @brief Clear the list of predecessor Basic Blocks.
   */
  inline void ClearPredBBlockList() {
    pred_bblock_.clear();
  }

  /**
   * @brief Clear the list of successor Basic Blocks.
   */
  inline void ClearSuccBBlockList() {
    succ_bblock_.clear();
  }

  /**
   * @brief Get the Instruction Class Pointer List for the Basic Block.
   * @return The Instruction class pointer list.
   */
  inline const std::list<MachineInstruction*> GetInstructions() {
    return instrs_;
  }

  /**
   * @brief prints the info on Basic Block in human readable/dot format to logcat.
   * @param output - output stream.
   * @param is_dot - whether the output must be in dot format.
   */
  void Print(std::ostream& output, bool is_dot);

  /**
   * @brief prints the info on Basic Block in human readable/dot format.
   * @param is_dot - whether the output must be in dot format.
   * @return the output stream.
   */
  std::ostringstream Print(bool is_dot);

  /**
   * @brief Visits the current Basic Block, then recursively visits
   * each predecessor Basic Block followed by recursively visiting each
   * Successor Basic Block.
   * @param bblock_set - The set of all the visited Basic Blocks.
   * @param bblock_order - The list that contains the Basic Blocks in the order
   *        they were visited.
   */
  void Visit(std::set<MachineBlock*>* bblock_set, std::list<MachineBlock*>* bblock_order);

  /**
   * @brief Copies the Instructions Starting from a certain Address to the Basic Block.
   * @param from_bblock - the Basic Block from which Instructions have to be copied.
   * @param start - The starting pointer to the Instruction Class.
   */
  void CopyInstruction(MachineBlock* from_bblock, const uint8_t* start);

  /**
   * @brief Deletes a certain Instruction from the Basic Block.
   * @param it - iterator for the Instruction List.
   */
  void DeleteInstruction(std::list<MachineInstruction*>::iterator it);

 private:
  uint32_t id_;
  const uint8_t* start_addr_;
  const uint8_t* end_addr_;
  uint32_t num_of_instrs_;
  std::list<MachineBlock*> pred_bblock_;
  std::list<MachineBlock*> succ_bblock_;
  std::list<MachineInstruction*> instrs_;
  bool is_dummy_;
};

/**
 * The BackLogDs Data Structure stores the code paths that have not yet been
 * analyzed. This could be due to call, jump (conditional/unconditional) etc.
 */
struct BackLogDs {
  MachineBlock* pred_bb;
  const uint8_t* ptr;
  MachineBlock* succ_bb;
  uint32_t call_depth;
};

/**
 * The CallInstrDs Data Structure is needed to Split the Basic Block that contains
 * a call into two - one that ends at the call site & it then branches to the call site.
 * Upon return, it connects to the next instruction after the call.
 */
struct CallInstrDs {
  MachineBlock* bb_to_be_split;
  MachineBlock* link_bb;
  uint8_t* end;
  uint8_t* start;
};

/**
 * CFGraph Class has information about the Control Flow Graph.
 */
class CFGraph {
 public:
  explicit CFGraph(std::string method_name)
      : is_simple_(true),
        num_of_bblocks_(0),
        num_of_instrs_(0),
        call_depth_(0),
        has_unknown_instructions_(false),
        has_indirect_calls_(false),
        method_name_(method_name) {}

  ~CFGraph() {
    for (auto it : cfg_bblock_list_) {
      delete it;
    }
  }

  /**
   * @brief Delete a certain Basic Block & Add a certain Basic Block from & to a
   * list of Basic Blocks respectively.
   * @param bblock_list - the list from which a Basic Block has to deleted &
   * to which a Basic Block has to be added.
   * @param bblock_to_be_deleted - the Basic Block to be deleted.
   * @param bblock_to_be_added - the Basic Block to de added.
   */
  void ChangePredecessors(std::list<MachineBlock*> bblock_list,
                          MachineBlock* bblock_to_be_deleted,
                          MachineBlock* bblock_to_be_added);

  /**
   * In case a Basic Block is a dummy, returns it's predecessor.
   * @param bblock - the Basic Block which is being tested.
   * @return bblock itself if not dummy; else returns it's predecessor.
   */
  MachineBlock* GetCorrectBB(MachineBlock* bblock);

  /**
   * @brief Get the name of the method being analyzed.
   * @return the method's PrettyName.
   */
  inline const std::string GetMethodName() {
    return method_name_;
  }

  /**
   * @brief Does the CFG have an unknown/unsupported Instruction.
   * @return whether the CFG has unknown/unsupported Instruction.
   */
  inline bool HasUnknownInstructions() {
    return has_unknown_instructions_;
  }

  /**
   * @brief The CFG has an unknown/unsupported Instruction.
   */
  inline void SetHasUnknownInstructions() {
    has_unknown_instructions_ = true;
  }

  /**
   * @brief Does the CFG have an indirect Call.
   * @return whether the CFG has indirect Call.
   */
  inline bool HasIndirectCalls() {
    return has_indirect_calls_;
  }

  /**
   * @brief The CFG has an Indirect Call.
   */
  inline void SetHasIndirectCalls() {
    has_indirect_calls_ = true;
  }

  /**
   * @brief Get the levels of call nesting.
   * @return the levels of call nesting.
   */
  inline uint32_t GetCallDepth() {
    return call_depth_;
  }

  /**
   * @brief Sets the call nesting level
   * @param depth - the level of call nesting.
   */
  inline void SetCallDepth(uint32_t depth) {
    call_depth_ = depth;
  }

  /**
   * @brief Increase level of Call nesting in CFG by one.
   */
  inline void IncCallDepth() {
    ++call_depth_;
  }

  /**
   * @brief Get the Number of Instructions in the CFG.
   * @return the number of Instructions.
   */
  inline uint32_t GetInstructionCnt() const {
    return num_of_instrs_;
  }

  /**
   * @brief Get the Number of Basic Blocks in the CFG.
   * @return the number of Basic Blocks.
   */
  inline uint32_t GetBBlockCnt() const {
    return num_of_bblocks_;
  }

  /**
   * @brief Add a Basic Block as the Starting Basic Block for CFG.
   * @param start_bblock - the starting Basic Block.
   */
  inline void AddStartBBlock(MachineBlock* start_bblock) {
    start_bblock_ = start_bblock;
  }

  /**
   * @brief Creates a Basic Block in the CFG & updates the predecessor.
   * @param predecessor_bblock - Predecessor Basic Block.
   * @return the created Basic Block.
   */
  MachineBlock* CreateBBlock(MachineBlock* predecessor_bblock);

  /**
   * @brief Update the Predecessors & successors caused by backlog due
   * to calls/jumps in the CFG.
   * @param old_pred - The old predecessor to be updated.
   * @param new_pred - The new predecessor.
   * @param backlog - List of backlog Data Structure.
   */
  void ChangePredForBacklog(MachineBlock* old_pred,
                            MachineBlock* new_pred,
                            std::list<BackLogDs*>* backlog);

  /**
   * @brief Set the CFG as not being Simple.
   */
  inline void SetNotSimple() {
    is_simple_ = false;
  }

  /**
   * @brief Returns if CFG is Simple.
   * @return whether the CFG is simple or not.
   */
  inline bool IsSimple() const {
    return is_simple_;
  }

  /**
   * @brief Add a Basic Block as one of the many Ending Basic Block for CFG.
   * @param bblock - one of the many ending Basic Block.
   */
  inline void AddEndBBlock(MachineBlock* bblock) {
    end_bblock_list_.insert(bblock);
  }

  /**
   * @brief Get the First Basic Block of the CFG.
   * @return The first Basic Block.
   */
  inline MachineBlock* GetStartBBlock() {
    return start_bblock_;
  }

  /**
   * @brief Keeps track of visited Basic Blocks.
   * @param bblock - the Basic Block that was visited.
   * @param start - Starting instruction pointer.
   * @param end - ending instruction pointer.
   */
  void AddTuple(MachineBlock* bblock, const uint8_t* start, const uint8_t* end);

  /**
   * @brief Check whether the Basic Block was already visited.
   * @param addr - the pointer to a certain location that has to be tested.
   * @param prev_bblock - Predecessor Basic Block.
   * @return true if visited. False otherwise.
   */
  bool IsVisitedForCall(uint8_t* addr, MachineBlock* prev_bblock);

  /**
   * @brief Check if an entry in the backlog was analyzed already.
   * @param entry - the backlog entry.
   * @param backlog - list of backlog entries.
   * @return true of visited; false otherwise.
   */
  bool IsVisitedForBacklog(struct BackLogDs* entry, std::list<BackLogDs*>* backlog);

  /**
   * @brief Check if a certain location(instruction) was already analyzed or not.
   * @param addr - The address that is being checked.
   * @param prev_bblock - Predecessor Basic Block.
   * @param succ_bblock - Successor Basic Block.
   * @param backlog - List of backlog entries.
   * @return true if analyzed already; false otherwise.
   */
  bool IsVisited(const uint8_t* addr,
                 MachineBlock* prev_bblock,
                 MachineBlock* succ_bblock,
                 std::list<BackLogDs*>* backlog);

  /**
   * @brief prints the CFG in human readable/dot format to logcat.
   * @param output - output stream.
   * @param is_dot - whether the output must be in dot format.
   */
  void Print(std::ostringstream &output, bool is_dot);

  /**
   * @brief Determine if the CFG falls within the budget to call it Fast or not.
   * @return true if method's CFG satisfies budget; false otherwise.
   */
  bool IsWithinBudget();

 private:
  MachineBlock* start_bblock_;
  bool is_simple_;
  uint32_t num_of_bblocks_;
  uint32_t num_of_instrs_;
  uint32_t call_depth_;
  bool has_unknown_instructions_;
  bool has_indirect_calls_;
  std::list<MachineBlock*> cfg_bblock_list_;
  std::list<MachineBlock*> visited_bblock_list_;
  std::set<MachineBlock*> end_bblock_list_;
  std::string method_name_;
};

/**
 * @brief BinaryAnalyzer class has methods & helpers to analyze a native method.
 */
class BinaryAnalyzer {
 public:
  explicit BinaryAnalyzer() : cfg_(nullptr) {
    disassembler_ = new Disassembler(Runtime::Current()->GetInstructionSet());
  }

  ~BinaryAnalyzer() {
    delete cfg_;
    delete disassembler_;
  }

  /**
   * @brief Analyze a method and determine whether it can be marked fast or not.
   * @param method_idx - dex method Index.
   * @param dex_file - dex File.
   * @param fnPtr - Function pointer of method to be analyzed.
   * @return true if fast; false otherwise.
   */
  bool AnalyzeMethod(uint32_t method_idx, const DexFile& dex_file, const void* fnPtr);

  /**
   * @brief Binary Analyze an x86 Instruction using Capstone Disassembler.
   * @param instr - Instruction Pointer.
   * @param curr_bb - Current Basic Block.
   * @param is_bb_end - Pointer to whether this instruction marks the end of a Basic Block.
   * @param target_disp - Displacement in case of jump/call.
   * @param cfg - The CFG.
   * @return the number of bytes the x86 instruction occupies.
   */
  size_t AnalyzeInstruction(const uint8_t* instr,
                            MachineBlock* curr_bb,
                            int32_t* is_bb_end,
                            int32_t* target_disp,
                            CFGraph* cfg);

  /**
   * @brief The Helper function to build CFG for the given method.
   * @param cfg - The CFG.
   * @param ptr - Instruction Pointer.
   * @param curr_bb - Current Basic Block.
   * @param backlog - Backlog List.
   * @param depth - Call depth/ levels of call nesting.
   * @param dummy_end - Dummy basic block.
   */
  void CFGHelper(CFGraph* cfg,
                 const uint8_t* ptr,
                 MachineBlock* curr_bb,
                 std::list<BackLogDs*>* backlog,
                 uint32_t depth,
                 MachineBlock* dummy_end);

  /**
   * @brief Constructs the CFG for the method by binary analysis.
   * @param ptr - the method function pointer.
   * @param method_name - Pretty Name of method.
   * @return the CFG for the analyzed method.
   */
  CFGraph* BuildCFG(const uint8_t* ptr, std::string method_name);

 private:
  CFGraph* cfg_;
  Disassembler* disassembler_;
};

}  // namespace x86
}  // namespace art

#endif  // ART_RUNTIME_BINARY_ANALYZER_BINARY_ANALYZER_X86_H_

