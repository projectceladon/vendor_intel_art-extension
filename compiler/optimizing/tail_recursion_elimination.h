/*
 * tail recursion elimination
 */

#ifndef ART_COMPILER_OPTIMIZING_TAIL_RECURSION_ELIMINATION_H_
#define ART_COMPILER_OPTIMIZING_TAIL_RECURSION_ELIMINATION_H_

#include "optimization.h"
#include "nodes.h"
#include "data_type.h"

namespace art {

/* The purpose of this class is to contain all variables which are required to convert
 * current tail-recursive method to iterative. With this, we also take care of thread safety.
 */
class TREContext {
public:
  // return type of method
  DataType::Type ret_type_;

  // whether recursion deciding if condition exiting in true or false case
  bool if_true_exit_;

  // method parameter whose change in value impact recursion (will become induction var of loop)
  HInstruction* imp_param_;

  // method parameter which act as accumulator variable i.e. hold result of recursive method
  HInstruction* acc_param_;

  // recursion deciding if condition
  HIf* inst_if_exit_;

  // input of return instruction
  HInstruction* ret_op_;

  // vector which contains recursive invoke instructions
  std::map<HBasicBlock*,std::vector<HInvoke*>> recursive_invoke_map_;
  std::vector<HInvoke*> recursive_invoke_instr_;

  // vector which keeps checked instructions during identification of tail-recursive method (avoid deadlock)
  std::vector<HInstruction*> checked_instr_for_name_;

  // method parameter list
  std::vector<HInstruction*> param_list_;

  TREContext(): ret_type_(DataType::Type::kLast), if_true_exit_(false),
      imp_param_(nullptr), acc_param_(nullptr), inst_if_exit_(nullptr),
      ret_op_(nullptr), recursive_invoke_map_()  {}
};

/*
 * This optimization pass performs tail recursion elimination. It identify tail-recursive methods & convert 
 * identified methods to iterative (i.e. eliminates recursion). Please find below method selection criteria.
 * - Return instruction of method should contain (or refer to) invoke to same method (i.e. tail-recursive).
 * - Method shouldn't have loop or try-catch.
 * - First successor of entry block should have last instruction as "IF".
 * - There should be single predecessor of exit block.
 * - Method should have only one incoming parameter which impact recursion.
 * - Method should not contain 2 or more recursive invokes in same block with different 
 *   induction variable operations as input.
 * - Method should not contain more than 1 accumulator input parameter (parameter which is modified inside
 *   method and included in method output i.e. return).
 */
class TailRecursionElimination : public HOptimization {
 public:
  TailRecursionElimination(HGraph* graph,
                         const char* name = kTailRecursionEliminationPassName)
      : HOptimization(graph, name) {}

  void Run() OVERRIDE;

  static constexpr const char* kTailRecursionEliminationPassName = "TRE";

 private:
  bool CheckTailRecursive(HInstruction* instr, const char* method_name, TREContext& trec);
  bool IdentifyTailRecursion(HInstruction* intsr, const char* method_name, TREContext& trec);
  bool TransformMethodGraph(HBasicBlock* exit_blk, HBasicBlock* first_blk, TREContext& trec);
  bool IdentifyIfRetBlkMainParam(HBasicBlock* exit_blk, HBasicBlock* first_blk, TREContext& trec);
  HInstruction* GetAccInstruction(HInstruction* val1, HInstruction* val2, TREContext& trec);

  DISALLOW_COPY_AND_ASSIGN(TailRecursionElimination);
};

}  // namespace art

#endif  // ART_COMPILER_OPTIMIZING_TAIL_RECURSION_ELIMINATION_H_
