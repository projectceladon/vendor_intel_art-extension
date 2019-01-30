/*
Copyright (c) 2018 Corporation
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
    * this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright notice,
    * this list of conditions and the following disclaimer in the documentation
    * and/or other materials provided with the distribution.

    * Neither the name of Intel Corporation nor the names of its contributors
    * may be used to endorse or promote products derived from this software
    * without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef ART_COMPILER_OPTIMIZING_INDUCTION_VAR_SIMPLIFICATION_H_
#define ART_COMPILER_OPTIMIZING_INDUCTION_VAR_SIMPLIFICATION_H_

#include "nodes.h"
#include "induction_var_analysis.h"
#include <tuple>

namespace art {

class InductionVarSimplification {
 public:
  explicit  InductionVarSimplification(HInductionVarAnalysis* analysis);
  
  /**
   * @brief Performs Simplification on the loop
   * @param loop The input loop in the graph.
   */
  bool SimplifyLoop(HLoopInformation* loop);
  /**
   * @brief Checks if the phi is a basic induction variable
   *        the loop
   * @param loop The loop to be checked.
   * @param phi The phi to be checked.
   */
  bool IsCandidatePhi(HLoopInformation* loop, HPhi* phi);
 private:
  /**
   * @brief Performs Strength Reduction to eliminate multplication and
   *        replace with equivalent addition
   * @param loop The loop to be simplified.
   * @param derived_var The derived induction variable upon which
   *        simplification is applied
   * @param candidate The mapping of derived induction variables.
   */
  void PerformReduction(HLoopInformation* loop, HInstruction* derived_var,   
                  std::tuple<HInstruction*, HConstant*, HConstant*> val);
   /**
   * @brief Checks if an instruction is suitable candidate to perform
   *         strength reducion 
   * @param loop The loop to be simplified.
   * @param to_check The instruction in the loop to be checked
   * @param candidate A mapping from biv to corresponding 
   *        derived induction variables
   *        Stores an induction of the form j = a*i +b where i is biv,
   *        as an entry [j, <i, a, b>]
   */
  bool IsCandidateForReduction(HLoopInformation* loop, HPhi* biv, HBinaryOperation* to_check,
                                std::map<HBinaryOperation*, std::tuple<HInstruction*,HConstant*, HConstant*>>* candidate);

  HInductionVarAnalysis* induction_analysis_;
  HBinaryOperation* biv_increment_;
  /* Results of induction var analysis */
  friend class HInductionVarAnalysis;

  DISALLOW_COPY_AND_ASSIGN(InductionVarSimplification);
};

}  // namespace art

#endif  // ART_COMPILER_OPTIMIZING_INDUCTION_VAR_SIMPLIFICATION_H_
