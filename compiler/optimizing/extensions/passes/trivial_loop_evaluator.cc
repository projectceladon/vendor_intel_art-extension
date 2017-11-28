/*
 * INTEL CONFIDENTIAL
 * Copyright (c) 2015, Intel Corporation All Rights Reserved.
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
 *
 */

//neeraj - resolve build errors
#include "android-base/stringprintf.h"
using android::base::StringPrintf;

#include "ext_utility.h"
#include "graph_x86.h"
#include "loop_iterators.h"
#include "nodes.h"
#include "trivial_loop_evaluator.h"

namespace art {

class TLEVisitor : public HGraphVisitor {
 public:
  union Value {
    explicit Value() { i = 0; }
    explicit Value(int32_t _i) { i = _i; }
    explicit Value(int64_t _l) { l = _l; }
    explicit Value(float _f) { f = _f; }
    explicit Value(double _d) { d = _d; }
    int32_t i;
    int64_t l;
    float f;
    double d;
  };

  explicit TLEVisitor(HGraph_X86* graph, HOptimization_X86* opt) :
      HGraphVisitor(graph),
      pred_index_(0),
      is_error_(false),
      next_bb_(nullptr),
      values_(std::less<HInstruction*>(), graph->GetArena()->Adapter()),
      opt_(opt) {}

#define NOTHING_IF_ERROR if (is_error_) return
#define SWITCH_FOR_TYPES(instr, condition, int_case, long_case, float_case, double_case) \
        do { switch (condition) { \
          case Primitive::kPrimInt: int_case; break; \
          case Primitive::kPrimLong: long_case; break; \
          case Primitive::kPrimFloat: float_case; break; \
          case Primitive::kPrimDouble: double_case; break; \
          default: SetError(instr); \
        }} while (false)

  void VisitInstruction(HInstruction* instruction) OVERRIDE {
    NOTHING_IF_ERROR;
    // If this instruction does not have a visitor then we do not support it.
    SetError(instruction);
  }

  void VisitSuspendCheck(HSuspendCheck* instr ATTRIBUTE_UNUSED) OVERRIDE {
    // We can ignore suspend checks for this optimization.
  }
  void VisitGoto(HGoto* instr) OVERRIDE {
    NOTHING_IF_ERROR;
    next_bb_ = instr->GetSuccessor();
  }

  void VisitIf(HIf* instr) OVERRIDE {
    NOTHING_IF_ERROR;
    next_bb_ =  GetValue(instr->InputAt(0)).i == 1 ?
               instr->IfTrueSuccessor() :
               instr->IfFalseSuccessor();
  }

  void VisitPhi(HPhi* instr) OVERRIDE {
    NOTHING_IF_ERROR;
    Value value = GetValue(instr->InputAt(pred_index_));
    NOTHING_IF_ERROR;
    values_.Overwrite(instr, value);
  }

#define INTEGRAL_TO_FP_CONV(vmin, vmax, cast, value) \
  (static_cast<cast>(std::isnan(value) ? 0 : (value >= vmax ? vmax : (value <= vmin ? vmin : value))))
  void VisitTypeConversion(HTypeConversion* instr) OVERRIDE {
    NOTHING_IF_ERROR;
    HInstruction* input = instr->InputAt(0);
    Primitive::Type in_type = instr->GetInputType();
    // We keep shorter integral types as int so we fine with
    // shorter in type but out type shuld be handled carefully.
    Primitive::Type orig_out_type = HPhi::ToPhiType(instr->GetResultType());
    Primitive::Type out_type = HPhi::ToPhiType(orig_out_type);

    Value in_value = GetValue(input);
    NOTHING_IF_ERROR;

    SWITCH_FOR_TYPES(instr, in_type,
      // int case.
      SWITCH_FOR_TYPES(instr, out_type,
        values_.Overwrite(instr, Value(static_cast<int32_t>(in_value.i))),
        values_.Overwrite(instr, Value(static_cast<int64_t>(in_value.i))),
        values_.Overwrite(instr, Value(static_cast<float>(in_value.i))),
        values_.Overwrite(instr, Value(static_cast<double>(in_value.i)))),
      // long case.
      SWITCH_FOR_TYPES(instr, out_type,
        values_.Overwrite(instr, Value(static_cast<int32_t>(in_value.l))),
        values_.Overwrite(instr, Value(static_cast<int64_t>(in_value.l))),
        values_.Overwrite(instr, Value(static_cast<float>(in_value.l))),
        values_.Overwrite(instr, Value(static_cast<double>(in_value.l)))),
      // float case.
      SWITCH_FOR_TYPES(instr, out_type,
        values_.Overwrite(instr, Value(INTEGRAL_TO_FP_CONV(kPrimIntMax, kPrimIntMin, int32_t, in_value.f))),
        values_.Overwrite(instr, Value(INTEGRAL_TO_FP_CONV(kPrimLongMax, kPrimLongMin, int64_t, in_value.f))),
        values_.Overwrite(instr, Value(static_cast<float>(in_value.f))),
        values_.Overwrite(instr, Value(static_cast<double>(in_value.f)))),
      // double case.
      SWITCH_FOR_TYPES(instr, out_type,
        values_.Overwrite(instr, Value(INTEGRAL_TO_FP_CONV(kPrimIntMax, kPrimIntMin, int32_t, in_value.d))),
        values_.Overwrite(instr, Value(INTEGRAL_TO_FP_CONV(kPrimLongMax, kPrimLongMin, int64_t, in_value.d))),
        values_.Overwrite(instr, Value(static_cast<float>(in_value.d))),
        values_.Overwrite(instr, Value(static_cast<double>(in_value.d)))
      ));

    switch (orig_out_type) {
      case Primitive::kPrimByte:
        values_.Overwrite(instr, Value(static_cast<int32_t>(static_cast<int8_t>(GetValue(instr).i))));
        break;
      case Primitive::kPrimShort:
        values_.Overwrite(instr, Value(static_cast<int32_t>(static_cast<int16_t>(GetValue(instr).i))));
        break;
      case Primitive::kPrimChar:
        values_.Overwrite(instr, Value(static_cast<int32_t>(static_cast<uint16_t>(GetValue(instr).i))));
        break;
      default:
        break;
    }
  }
#undef INTEGRAL_TO_FP_CONV

  template <typename T> static bool FpEqual(T x, T y) {
    return (((x) > (y)) ?
           (((x) - (y)) < std::numeric_limits<T>::epsilon()) :
           (((y) - (x)) < std::numeric_limits<T>::epsilon()));
  }

  template <typename E, typename T> static uint32_t Compare(E eq, T x, T y) {
    return eq(x, y) ? 0 : (x > y ? 1 : -1);
  }

  int32_t Compare(HInstruction* instr,
                  HInstruction* left,
                  HInstruction* right,
                  ComparisonBias bias) {
    DCHECK_EQ(right->GetType(), right->GetType());

    Value left_value = GetValue(left);
    Value right_value = GetValue(right);
    if (is_error_) {
      return 0;
    }
    int32_t res = 0;
    SWITCH_FOR_TYPES(instr, left->GetType(),
      res = left_value.i == right_value.i ? 0 : (left_value.i > right_value.i ? 1 : -1),
      res = left_value.l == right_value.l ? 0 : (left_value.l > right_value.l ? 1 : -1),
      res = (isnan(left_value.f) || isnan(right_value.f))
            ? (bias == ComparisonBias::kGtBias ? 1 : -1)
            : FpEqual(left_value.f, right_value.f) ? 0 : (left_value.f > right_value.f ? 1 : -1),
      res = (isnan(left_value.d) || isnan(right_value.d))
            ? (bias == ComparisonBias::kGtBias ? 1 : -1)
            : (FpEqual(left_value.d, right_value.d) ? 0 : (left_value.d > right_value.d ? 1 : -1)));
    return res;
  }

  void VisitCompare(HCompare* instr) OVERRIDE {
    NOTHING_IF_ERROR;
    int32_t res = Compare(instr, instr->GetLeft(), instr->GetRight(), instr->GetBias());
    NOTHING_IF_ERROR;
    values_.Overwrite(instr, Value(res));
  }

  void VisitEqual(HEqual* instr) OVERRIDE {
    NOTHING_IF_ERROR;
    values_.Overwrite(instr,
      Value(Compare(instr, instr->GetLeft(), instr->GetRight(), instr->GetBias()) == 0 ? 1 : 0));
  }
  void VisitNotEqual(HNotEqual* instr) OVERRIDE {
    NOTHING_IF_ERROR;
    values_.Overwrite(instr,
      Value(Compare(instr, instr->GetLeft(), instr->GetRight(), instr->GetBias()) != 0 ? 1 : 0));
  }
  void VisitLessThan(HLessThan* instr) OVERRIDE {
    NOTHING_IF_ERROR;
    values_.Overwrite(instr,
      Value(Compare(instr, instr->GetLeft(), instr->GetRight(), instr->GetBias()) < 0 ? 1 : 0));
  }
  void VisitLessThanOrEqual(HLessThanOrEqual* instr) OVERRIDE {
    NOTHING_IF_ERROR;
    values_.Overwrite(instr,
      Value(Compare(instr, instr->GetLeft(), instr->GetRight(), instr->GetBias()) <= 0 ? 1 : 0));
  }
  void VisitGreaterThan(HGreaterThan* instr) OVERRIDE {
    NOTHING_IF_ERROR;
    values_.Overwrite(instr,
      Value(Compare(instr, instr->GetLeft(), instr->GetRight(), instr->GetBias()) > 0 ? 1 : 0));
  }
  void VisitGreaterThanOrEqual(HGreaterThanOrEqual* instr) OVERRIDE {
    NOTHING_IF_ERROR;
    values_.Overwrite(instr,
      Value(Compare(instr, instr->GetLeft(), instr->GetRight(), instr->GetBias()) >= 0 ? 1 : 0));
  }

  template <typename F> void VisitUnsignedComparision(HCondition* instr, F comparator) {
    HInstruction* left = instr->GetLeft();
    HInstruction* right = instr->GetRight();
    DCHECK_EQ(left->GetType(), right->GetType());

    Value left_value = GetValue(left);
    Value right_value = GetValue(right);
    NOTHING_IF_ERROR;

    bool res = true;
    SWITCH_FOR_TYPES(instr, left->GetType(),
      res = comparator(static_cast<uint32_t>(left_value.i), static_cast<uint32_t>(right_value.i)),
      res = comparator(static_cast<uint64_t>(left_value.l), static_cast<uint64_t>(right_value.l)),
      SetError(instr),
      SetError(instr));
    values_.Overwrite(instr, Value(res ? 1 : 0));
  }

  void VisitBelow(HBelow* instr) OVERRIDE {
    NOTHING_IF_ERROR;
    VisitUnsignedComparision(instr, [] (uint64_t x, uint64_t y) -> bool { return x < y; });
  }
  void VisitBelowOrEqual(HBelowOrEqual* instr) OVERRIDE {
    NOTHING_IF_ERROR;
    VisitUnsignedComparision(instr, [] (uint64_t x, uint64_t y) -> bool { return x <= y; });
  }
  void VisitAbove(HAbove* instr) OVERRIDE {
    NOTHING_IF_ERROR;
    VisitUnsignedComparision(instr, [] (uint64_t x, uint64_t y) -> bool { return x > y; });
  }
  void VisitAboveOrEqual(HAboveOrEqual* instr) OVERRIDE {
    NOTHING_IF_ERROR;
    VisitUnsignedComparision(instr, [] (uint64_t x, uint64_t y) -> bool { return x >= y; });
  }

  void VisitNeg(HNeg* instr) OVERRIDE {
    NOTHING_IF_ERROR;
    Value val = GetValue(instr->GetInput());
    NOTHING_IF_ERROR;

    SWITCH_FOR_TYPES(instr, instr->GetInput()->GetType(),
      values_.Overwrite(instr, Value(-val.i)),
      values_.Overwrite(instr, Value(-val.l)),
      values_.Overwrite(instr, Value(-val.f)),
      values_.Overwrite(instr, Value(-val.d)));
  }

  void VisitAdd(HAdd* instr) OVERRIDE {
    NOTHING_IF_ERROR;
    HInstruction* left = instr->GetLeft();
    HInstruction* right = instr->GetRight();
    DCHECK_EQ(right->GetType(), right->GetType());

    Value left_value = GetValue(left);
    Value right_value = GetValue(right);
    NOTHING_IF_ERROR;

    SWITCH_FOR_TYPES(instr, left->GetType(),
      values_.Overwrite(instr, Value(left_value.i + right_value.i)),
      values_.Overwrite(instr, Value(left_value.l + right_value.l)),
      values_.Overwrite(instr, Value(left_value.f + right_value.f)),
      values_.Overwrite(instr, Value(left_value.d + right_value.d)));
  }

  void VisitSub(HSub* instr) OVERRIDE {
    NOTHING_IF_ERROR;
    HInstruction* left = instr->GetLeft();
    HInstruction* right = instr->GetRight();
    DCHECK_EQ(right->GetType(), right->GetType());

    Value left_value = GetValue(left);
    Value right_value = GetValue(right);
    NOTHING_IF_ERROR;

    SWITCH_FOR_TYPES(instr, left->GetType(),
      values_.Overwrite(instr, Value(left_value.i - right_value.i)),
      values_.Overwrite(instr, Value(left_value.l - right_value.l)),
      values_.Overwrite(instr, Value(left_value.f - right_value.f)),
      values_.Overwrite(instr, Value(left_value.d - right_value.d)));
  }

  void VisitMul(HMul* instr) OVERRIDE {
    NOTHING_IF_ERROR;
    HInstruction* left = instr->GetLeft();
    HInstruction* right = instr->GetRight();
    DCHECK_EQ(right->GetType(), right->GetType());

    Value left_value = GetValue(left);
    Value right_value = GetValue(right);
    NOTHING_IF_ERROR;

    SWITCH_FOR_TYPES(instr, left->GetType(),
      values_.Overwrite(instr, Value(left_value.i * right_value.i)),
      values_.Overwrite(instr, Value(left_value.l * right_value.l)),
      values_.Overwrite(instr, Value(left_value.f * right_value.f)),
      values_.Overwrite(instr, Value(left_value.d * right_value.d)));
  }

  void VisitDiv(HDiv* instr) OVERRIDE {
    NOTHING_IF_ERROR;
    HInstruction* left = instr->GetLeft();
    HInstruction* right = instr->GetRight();
    DCHECK_EQ(right->GetType(), right->GetType());

    Value left_value = GetValue(left);
    Value right_value = GetValue(right);
    NOTHING_IF_ERROR;

    // neeraj - fix klockwork 99356 issue (divide by zero)
    DCHECK(right_value.i != 0);

    SWITCH_FOR_TYPES(instr, left->GetType(),
      values_.Overwrite(instr, Value(left_value.i / right_value.i)),
      values_.Overwrite(instr, Value(left_value.l / right_value.l)),
      values_.Overwrite(instr, Value(left_value.f / right_value.f)),
      values_.Overwrite(instr, Value(left_value.d / right_value.d)));
  }

  template <typename T, typename F> T FpRem(T x, T y, F f) {
    if (std::isnan(x) || std::isnan(y) ||
        std::isinf(x) || FpEqual(y, static_cast<T>(0))) {
      return std::numeric_limits<T>::quiet_NaN();
    }

    if (std::isinf(y)) {
      return x;
    }

    if (FpEqual(x, static_cast<T>(0))) {
      return x;
    }

    return f(x, y);
  }

  void VisitRem(HRem* instr) OVERRIDE {
    NOTHING_IF_ERROR;
    HInstruction* left = instr->GetLeft();
    HInstruction* right = instr->GetRight();
    DCHECK_EQ(right->GetType(), right->GetType());

    Value left_value = GetValue(left);
    Value right_value = GetValue(right);
    NOTHING_IF_ERROR;

    // neeraj - fix klockwork 99357 issue (divide by zero)
    DCHECK(right_value.i != 0);

    SWITCH_FOR_TYPES(instr, left->GetType(),
      values_.Overwrite(instr, Value(right_value.i == -1 ? 0 : left_value.i % right_value.i)),
      values_.Overwrite(instr, Value(right_value.l == -1 ? 0 : left_value.l % right_value.l)),
      values_.Overwrite(instr,
        Value(FpRem(left_value.f, right_value.f, [] (float x, float y) -> float { return std::fmodf(x, y); }))),
      values_.Overwrite(instr,
        Value(FpRem(left_value.d, right_value.d, [] (double x, double y) -> double { return std::fmod(x, y); }))));
  }

  int32_t ComputeShiftCount(HInstruction* instr) {
    int32_t shift_count = 0;
    Primitive::Type type = instr->GetType();
    if (type == Primitive::kPrimInt) {
      shift_count = GetValue(instr).i & kMaxIntShiftDistance;
    } else if (type == Primitive::kPrimLong) {
      shift_count = GetValue(instr).i & kMaxLongShiftDistance;
    } else {
      SetError(instr);
    }
    return shift_count;
  }

  void VisitShl(HShl* instr) OVERRIDE {
    NOTHING_IF_ERROR;

    Value left_value = GetValue(instr->GetLeft());
    NOTHING_IF_ERROR;

    int32_t shift_count = ComputeShiftCount(instr->GetRight());
    NOTHING_IF_ERROR;

    SWITCH_FOR_TYPES(instr, instr->GetLeft()->GetType(),
      values_.Overwrite(instr, Value(left_value.i << shift_count)),
      values_.Overwrite(instr, Value(left_value.l << shift_count)),
      SetError(instr),
      SetError(instr));
  }

  void VisitShr(HShr* instr) OVERRIDE {
    NOTHING_IF_ERROR;

    Value left_value = GetValue(instr->GetLeft());
    NOTHING_IF_ERROR;

    int32_t shift_count = ComputeShiftCount(instr->GetRight());
    NOTHING_IF_ERROR;

    SWITCH_FOR_TYPES(instr, instr->GetLeft()->GetType(),
      values_.Overwrite(instr, Value(left_value.i >> shift_count)),
      values_.Overwrite(instr, Value(left_value.l >> shift_count)),
      SetError(instr),
      SetError(instr));
  }

  void VisitUShr(HUShr* instr) OVERRIDE {
    NOTHING_IF_ERROR;

    Value left_value = GetValue(instr->GetLeft());
    NOTHING_IF_ERROR;

    int32_t shift_count = ComputeShiftCount(instr->GetRight());
    NOTHING_IF_ERROR;

    SWITCH_FOR_TYPES(instr, instr->GetLeft()->GetType(),
      values_.Overwrite(instr,
        Value(static_cast<int32_t>(static_cast<uint32_t>(left_value.i) >> shift_count))),
      values_.Overwrite(instr,
        Value(static_cast<int64_t>(static_cast<uint64_t>(left_value.l) >> shift_count))),
      SetError(instr),
      SetError(instr));
  }

  void VisitAnd(HAnd* instr) OVERRIDE {
    NOTHING_IF_ERROR;
    SWITCH_FOR_TYPES(instr, instr->GetType(),
      values_.Overwrite(instr,
        Value(static_cast<int32_t>(GetValueAsLong(instr->GetLeft()) & GetValueAsLong(instr->GetRight())))),
      values_.Overwrite(instr,
        Value(GetValueAsLong(instr->GetLeft()) & GetValueAsLong(instr->GetRight()))),
      SetError(instr),
      SetError(instr));
  }

  void VisitOr(HOr* instr) OVERRIDE {
    NOTHING_IF_ERROR;
    SWITCH_FOR_TYPES(instr, instr->GetType(),
      values_.Overwrite(instr,
        Value(static_cast<int32_t>(GetValueAsLong(instr->GetLeft()) | GetValueAsLong(instr->GetRight())))),
      values_.Overwrite(instr,
        Value(GetValueAsLong(instr->GetLeft()) | GetValueAsLong(instr->GetRight()))),
      SetError(instr),
      SetError(instr));
  }

  void VisitXor(HXor* instr) OVERRIDE {
    NOTHING_IF_ERROR;
    SWITCH_FOR_TYPES(instr, instr->GetType(),
      values_.Overwrite(instr,
        Value(static_cast<int32_t>(GetValueAsLong(instr->GetLeft()) ^ GetValueAsLong(instr->GetRight())))),
      values_.Overwrite(instr,
        Value(GetValueAsLong(instr->GetLeft()) ^ GetValueAsLong(instr->GetRight()))),
      SetError(instr),
      SetError(instr));
  }

  void VisitNot(HNot* instr) OVERRIDE {
    NOTHING_IF_ERROR;
    SWITCH_FOR_TYPES(instr, instr->GetType(),
      values_.Overwrite(instr, Value(~GetValue(instr->GetInput()).i)),
      values_.Overwrite(instr, Value(~GetValue(instr->GetInput()).l)),
      SetError(instr),
      SetError(instr));
  }

  void VisitBooleanNot(HBooleanNot* instr) OVERRIDE {
    NOTHING_IF_ERROR;
    SWITCH_FOR_TYPES(instr, instr->GetType(),
      values_.Overwrite(instr, Value(GetValue(instr->GetInput()).i == 1 ? 0 : 1)),
      SetError(instr),
      SetError(instr),
      SetError(instr));
  }

  HBasicBlock* GetNextBasicBlock() { return next_bb_; }

  bool IsError() { return is_error_; }

  void setPredecessorIndex(int idx) {
    pred_index_ = idx;
  }

  int64_t GetValueAsLong(HInstruction* instr) {
    Value v = GetValue(instr);
    SWITCH_FOR_TYPES(instr, instr->GetType(),
      return static_cast<int64_t>(v.i),
      return static_cast<int64_t>(v.l),
      return static_cast<int64_t>(v.f),
      return static_cast<int64_t>(v.d));
    return 0;
  }

  Value GetValue(HInstruction* instr) {
    Value v;
    if (instr->IsConstant()) {
      SWITCH_FOR_TYPES(instr, instr->GetType(),
        v.i = instr->AsIntConstant()->GetValue(),
        v.l = instr->AsLongConstant()->GetValue(),
        v.f = instr->AsFloatConstant()->GetValue(),
        v.d = instr->AsDoubleConstant()->GetValue());
    } else {
      auto value_it = values_.find(instr);
      if (value_it != values_.end()) {
        return value_it->second;
      } else {
        PRINT_PASS_OSTREAM_MESSAGE(opt_,
          "Input \"" << instr << "\" is not constant.");
        SetError(instr);
      }
    }
    return v;
  }

  HConstant* GetConstant(HGraph* graph, HInstruction* instr, Value v) {
    SWITCH_FOR_TYPES(instr, instr->GetType(),
      return graph->GetIntConstant(v.i),
      return graph->GetLongConstant(v.l),
      return graph->GetFloatConstant(v.f),
      return graph->GetDoubleConstant(v.d));
    return nullptr;
  }

#undef NOTHING_IF_ERROR
#undef SWITCH_FOR_TYPES

  void SetError(HInstruction* instr) {
    is_error_ = true;
    PRINT_PASS_OSTREAM_MESSAGE(opt_, "TLE could not handle " << instr);
  }

  ArenaSafeMap<HInstruction*, Value> GetConstants() const {
    return values_;
  }

 private:
  int pred_index_;
  bool is_error_;
  HBasicBlock* next_bb_;
  ArenaSafeMap<HInstruction*, Value> values_;
  HOptimization_X86* opt_;
};

void TrivialLoopEvaluator::Run() {
  // Build the list of loops belonging to the CFG.
  HGraph_X86* graph = GRAPH_TO_GRAPH_X86(graph_);
  HLoopInformation_X86* loop_start = graph->GetLoopInformation();
  bool graph_updated = false;
  PRINT_PASS_OSTREAM_MESSAGE(this, "TrivialLoopEvaluator: Begin " << GetMethodName(graph));

  // For each inner loop in the graph, we will try to apply TLE.
  for (HOnlyInnerLoopIterator it(loop_start); !it.Done(); it.Advance()) {
    HLoopInformation_X86* loop = it.Current();

    // First, we need to check that the loop is a valid candidate for TLE.
    if (LoopGate(loop) == false) {
      continue;
    }

    // This container will hold all the evaluated values of the loop.
    TLEVisitor visitor(graph, this);

    // Then, we will evaluate the loop if possible.
    if (!EvaluateLoop(loop, visitor)) {
      continue;
    }

    // The last step is to write back the values to the registers using the evaluated values.
    UpdateRegisters(loop, visitor);

    // At this point, we have updated the graph at least once while executing this method.
    graph_updated = true;

    // Finally, we want to remove the now-useless loop from the graph.
    if (!loop->RemoveFromGraph()) {
      LOG(FATAL) << "TrivialLoopEvaluator: Could not remove loop from the graph.";
    } else {
      MaybeRecordStat(MethodCompilationStat::kIntelRemoveTrivialLoops);
      PRINT_PASS_OSTREAM_MESSAGE(this, "TrivialLoopEvaluator: Loop \"" << loop << "\" of method \""
                << GetMethodName(graph) << "\" has been statically evaluated by TLE.");
    }
  }

  if (graph_updated) {
    graph->RebuildDomination();
  }
  PRINT_PASS_OSTREAM_MESSAGE(this, "TrivialLoopEvaluator: End " << GetMethodName(graph));
}

bool TrivialLoopEvaluator::EvaluateLoop(HLoopInformation_X86* loop, TLEVisitor& visitor) {
  const unsigned int loop_iterations = loop->GetNumIterations(loop->GetHeader());

  // For each iteration of the loop, execute its sequence of instructions.
  uint64_t current_iter = 0;
  HBasicBlock* header = loop->GetHeader();
  HBasicBlock* current_block = header;

  while (loop->Contains(*current_block)) {
    visitor.VisitBasicBlock(current_block);

    if (visitor.IsError()) {
      return false;
    }

    HBasicBlock* bb = visitor.GetNextBasicBlock();

    // For iteration count checker.
    if (current_block == header) {
      current_iter++;
      DCHECK_LE(current_iter, loop_iterations);
    }

    // Set predecessor index to handle Phi nodes in the next bb.
    visitor.setPredecessorIndex(bb->GetPredecessorIndexOf(current_block));

    current_block = bb;
  }
  // Paranoid check: we must have evaluated all our iterations. Otherwise, it means either bound
  // computation is broken, or we did not evaluate the loop properly.
  DCHECK_EQ(current_iter, loop_iterations);

  return true;
}

void TrivialLoopEvaluator::UpdateRegisters(HLoopInformation_X86* loop,
                                           TLEVisitor& visitor) {
  DCHECK(loop != nullptr);
  UNUSED(visitor);

  // We want to find all the users of the values we need to write back.
  // Then, we replace the corresponding input by the HConstant.
  for (auto value : visitor.GetConstants()) {
    HInstruction* insn = value.first;
    TLEVisitor::Value constant = value.second;
    HConstant* constant_node = nullptr;

    // Go through each of the instruction's users.
    const HUseList<HInstruction*>& uses = insn->GetUses();
    for (auto it = uses.begin(), end2 = uses.end(); it != end2; ) {
      HInstruction* user = it->GetUser();
      size_t input_index = it->GetIndex();
      it++;
      // We do not care about users in the loop (we will kill them anyway).
      if (loop->Contains(*user->GetBlock())) {
        continue;
      }

      if (constant_node == nullptr) {
        constant_node = visitor.GetConstant(graph_, insn, constant);
        CHECK(constant_node);
      }
      // Neeraj - fix dex2oat crash
      DCHECK(constant_node->GetBlock() != nullptr);
      user->ReplaceInput(constant_node, input_index);
    }

    // Go through each of the environment's users.
    const HUseList<HEnvironment*>& uses1 = insn->GetEnvUses();
    for (auto it = uses1.begin(), end2 = uses1.end(); it != end2; ) {
      HEnvironment* user = it->GetUser();
      size_t input_index = it->GetIndex();
      it++;
      // We do not care about users in the loop (we will kill then anyway).
      if (loop->Contains(*user->GetHolder()->GetBlock())) {
        continue;
      }

      if (constant_node == nullptr) {
        constant_node = visitor.GetConstant(graph_, insn, constant);
        CHECK(constant_node);
      }
      //neeraj - fix dex2oat crash
      DCHECK(constant_node == nullptr || constant_node->GetBlock() != nullptr);
      user->RemoveAsUserOfInput(input_index);

      user->SetRawEnvAt(input_index, constant_node);
      //atul.b Fix Klocwork NULL dereferenced issue 112780
      DCHECK(constant_node != nullptr);
      constant_node->AddEnvUseAt(user, input_index);
    }
  }
}

bool TrivialLoopEvaluator::LoopGate(HLoopInformation_X86* loop) {
  DCHECK(loop != nullptr);

  unsigned int num_blocks = loop->NumberOfBlocks();
  // Loops are not normalized. Therefore, we want to skip the first BB.
  if (num_blocks > 2) {
    PRINT_PASS_OSTREAM_MESSAGE(this, "Cannot apply TLE: loop has " << num_blocks << " blocks.");
    return false;
  }

  // We currently support only innermost loops.
  if (!loop->IsInner()) {
    PRINT_PASS_OSTREAM_MESSAGE(this, "Loop is not innermost.");
    return false;
  }

  // The loop should not side exit because it would make the evaluation harder.
  if (!loop->HasOneExitEdge()) {
    PRINT_PASS_OSTREAM_MESSAGE(this, "Loop has more than one exit block.");
    return false;
  }

  // The loop must have a known number of iterations in order to evaluate it.
  if (!loop->HasKnownNumIterations()) {
    PRINT_PASS_OSTREAM_MESSAGE(this, "Loop does not have a known number of iterations.");
    return false;
  }

  // The iteration count must be smaller than the threshold we fixed in order to
  // prevent increasing compile time too much.
  if (loop->GetNumIterations(loop->GetHeader()) > kLoopEvalMaxIter) {
    PRINT_PASS_OSTREAM_MESSAGE(this,
        "Loop has too many iterations (max supported : " << kLoopEvalMaxIter << ").");
    return false;
  }

  return true;
}

}  // namespace art.
