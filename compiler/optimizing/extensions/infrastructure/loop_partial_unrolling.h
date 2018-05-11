/*
 * Copyright (C) 2018 Intel Corporation
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

#ifndef ART_OPT_INFRASTRUCTURE_LOOP_PARTIAL_UNROLLING_H_
#define ART_OPT_INFRASTRUCTURE_LOOP_PARTIAL_UNROLLING_H_

#include "nodes.h"

namespace art {

// Forward declaration.
class HInstructionCloner;

/**
 * @brief This data structure represents the body of the loop. It is used in the methods,
 * which implement the loop unrolling optimization.
 */
class HLoopPartialUnrolling {
 public:
  HLoopPartialUnrolling(HLoopInformation_X86* loop, HOptimization_X86* optim);

  /**
   * @brief Partially unrolls the loop by the provided factor if possible. The user
   * must check the feasability of the unrolling before with a call to Gate().
   * @return Returns true if the unrolling was successful, or false otherwise.
   * @sa Gate.
   */
  bool PartialUnroll();
  /**
   * @brief Makes sure the provided loop complies with the restrictions of loop unrolling.
   * @param max_unrolled_instructions The maximum amount of instructions tolerated for unrolling.
   * @return Returns true if the loop complies with the unrolling restrictions, or false otherwise.
   */
  bool Gate(uint64_t max_unrolled_instructions, uint64_t unroll_factor);

 private:
  /**
   * @brief Helper functions to debug loop
   */

  void DumpBB(HBasicBlock* block);
  
  void DumpLoop(HLoopInformation_X86* loop);
 
  void DumpInstructions(HBasicBlock* block);
  
  void DumpInstruction(HInstruction* intsr);

 /**
  *@brief Unrolls the body the loop by a given unrolling factor
  *@param num_iterations number of iterations of the loop
  *@param unroll_factor unrolling factor
   @return Returns true if it was possible to unroll
  */
  
  bool UnrollBody(uint64_t num_iterations, uint64_t unrolling_factor);
  
 /**
  * @brief Clone instructions in loop body
  */

  void CloneInstructionsForUnroll(HInstructionCloner& cloner);
  
  /**
   * @brief Adds cloned instructions to the specified basic block
   * @param copy_bb the block to which instruction should be copied
   */
 
  void AddClonedInstructions(HInstructionCloner& cloner, HBasicBlock* copy_bb, uint64_t iteration);
  
  /**
   * @brief Fix loop header phis to use appropriate in loop inputs
   */

  void FixLoopHeaderPhis();
  /**
   * @brief During the unrolling iterations, we need to take the in-loop
   * phi inputs into account. This mapping is computed
   * when this method is called.
   */
  void UpdateLoopPhiInputsMap(HInstructionCloner& cloner);
  /**
   *@brief Stores the instructions in the loop body which should be
   * considered for unrolling
   */
  
  void GetInLoopInstructions();
  /**
   *@brief Checks whether the give instruction is a loop condition instruction
   *@param ins Instruction to be checked
   */

  bool IsLoopConditionInstruction(HInstruction* ins);
  
  /**
   *@brief Identifies the instructions which are part of loop
   * condition checking
   */
  void ListLoopConditionInstructions();
  
  /**
   * For the first unrolled version, we need to use in loop inputs of phi.
   * This method computes the mapping to phi and its in loop input
   */
  void MapLoopPhiInputs();
  
   /**
    * Used to check if the loop body is empty and 
    * the loop does not exit the body via break;
    */
  bool IsSimpleLoop();
 
 /**
  * Used to check if the loop does not have any
  * other instructions expect induction variable
  * and linear operation corresponding to it
  */
  bool IsLoopEmpty();
 
 /**
  * Used to fix the users of loop instructions
  * outside the loop
  */
  void FixLoopUsers(HInstructionCloner& cloner);
  
 /**
  * Utility function to increment constant of the linear operation
  * as per the iteartion number and unroll factor
  */
  void IncrementConstantForUnrolling(HInstruction* linear_operation, uint64_t iteration);

   // The loop that needs to be unrolled.
  HLoopInformation_X86* loop_;

  // The graph the loop belongs to.
  HGraph_X86* graph_;

  ArenaVector<HInstruction*> loop_instructions_;
  
  // To get instructions that are part of loop condition check
  std::unordered_set<HInstruction*> loop_condition_instructions_;
  
  //Map to maintain phi and in loop input mapping
  SafeMap<HInstruction*, HInstruction*>phi_to_loop_inputs_;
 
  //Map each loop instruction to its corresponding replacement after unrolling 
  SafeMap<HInstruction*,HInstruction*>ins_to_use_;

  // The instruction cloner is used to duplicate loop instructions.
  HInstructionCloner cloner_;

  // The optional optimization the loop unrolling is attached to.
  HOptimization_X86* optim_;
};

}  // namespace art

#endif
