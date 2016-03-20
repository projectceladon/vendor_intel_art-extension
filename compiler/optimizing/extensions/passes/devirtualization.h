/*
 * Copyright (C) 2016 Intel Corporation
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

#ifndef ART_COMPILER_OPTIMIZING_EXTENSIONS_PASSES_DEVIRTUALIZATION_H_
#define ART_COMPILER_OPTIMIZING_EXTENSIONS_PASSES_DEVIRTUALIZATION_H_

#include "dex_file.h"
#include "safe_map.h"
#include "speculation_pass.h"

namespace art {

class HDevirtualization : public HSpeculationPass {
 public:
  HDevirtualization(HGraph* graph,
                    const DexCompilationUnit& compilation_unit,
                    CompilerDriver* compiler_driver,
                    StackHandleScopeCollection* handles,
                    OptimizingCompilerStats* stats = nullptr)
      : HSpeculationPass(graph, kDevirtualizationPassName, compilation_unit,
                         compiler_driver, stats),
        handles_(handles) {
  }
  ~HDevirtualization();

 protected:
  // For explanation on each of these methods, see HSpeculationPass.
  bool IsCandidate(HInstruction* instr) OVERRIDE;
  bool HasPrediction(HInstruction* instr, bool update) OVERRIDE;
  uint64_t GetMaxCost() OVERRIDE;
  uint64_t GetCost(HInstruction* instr) OVERRIDE;
  std::pair<uint64_t, uint64_t> GetMispredictRate(HInstruction* instr) OVERRIDE;
  uint64_t GetProfit(HInstruction* instr) OVERRIDE;
  bool IsPredictionSame(HInstruction* instr, HInstruction* instr2) OVERRIDE;
  HSpeculationGuard* InsertSpeculationGuard(HInstruction* instr_guarded,
                                            HInstruction* instr_cursor) OVERRIDE;
  bool HandleSpeculation(HInstruction* instr, bool guard_inserted) OVERRIDE;
  void RecordSpeculation() OVERRIDE {
    MaybeRecordStat(kIntelDevirtualized);
  }
  void RecordFoundCandidate() OVERRIDE {
    MaybeRecordStat(kIntelDevirtualizationConsideration);
  }
  SpeculationRecoveryApproach GetRecoveryMethod(HInstruction* instr) OVERRIDE;

  /**
   * @brief Used to check whether a particular method or its class are final.
   * @param method The ArtMethod to check.
   * @return Returns true if method or its class are final.
   */
  static bool IsMethodOrDeclaringClassFinal(ArtMethod* method)
      SHARED_REQUIRES(Locks::mutator_lock_);

  /**
   * @brief Used to find the target of virtual invoke.
   * @details Uses type information from RTP to determine target.
   * @param invoke The virtual/interface invoke being considered.
   * @param resolved_method This is a resolved method from dex cache from the provided
   * dex index that was present in dex code. This might not be the final target - it will
   * depend on concrete type of object.
   * @return Returns the ArtMethod target of the invoke.
   */
  static ArtMethod* FindVirtualOrInterfaceTarget(HInvoke* invoke,
                                                 ArtMethod* resolved_method)
      SHARED_REQUIRES(Locks::mutator_lock_);

  /**
   * @brief Used to check whether this class has its type visible in the dex file.
   * @param cls The class to check.
   * @param dex_file The dex file in which to search for this class.
   * @param dex_cache The caller's dex cache.
   * @return Returns the class index in the dex file.
   */
  static uint32_t FindClassIndexIn(mirror::Class* cls, const DexFile& dex_file,
                                   Handle<mirror::DexCache> dex_cache)
      SHARED_REQUIRES(Locks::mutator_lock_);

  /**
   * @brief Used to check whether this method's declaration can be found in dex file.
   * @param method The method to check.
   * @param dex_file The dex file in which to search for this method.
   * @param referrer_index The dex index of referrer.
   * @return Returns the method index in the dex file.
   */
  static uint32_t FindMethodIndexIn(ArtMethod* method, const DexFile& dex_file,
                                    uint32_t referrer_index)
      SHARED_REQUIRES(Locks::mutator_lock_);

  typedef Handle<mirror::Class> TypeHandle;

  /**
   * @brief Used to find an ordered list of imprecise types predicted for this invoke
   *  via profile or other mechanisms.
   * @param invoke The invoke for which to find probable types.
   * @param caller_method The method that is doing the calling.
   * @returns Returns an ordered list of types provided by profile.
   */
  std::vector<TypeHandle> FindTypesFromProfile(HInvoke* invoke,
                                               ArtMethod* caller_method);

  /**
   * @brief Used to find the primary type for this invoke.
   * @details This requires that types were already loaded from profiles or other means.
   * @param invoke The call for which to get the type.
   * @return Returns the primary type prediction for this invoke.
   */
  TypeHandle GetPrimaryType(HInvoke* invoke) const;

 private:
  SafeMap<HInstruction*, TypeHandle> precise_prediction_;
  SafeMap<HInstruction*, std::vector<TypeHandle>> imprecise_predictions_;
  StackHandleScopeCollection* const handles_;

  static constexpr const char* kDevirtualizationPassName = "devirtualization";

  // Ideally, we would use a cost framework for this. But since it does not
  // exist, for now simply estimate these.
  static constexpr uint32_t kCostOfLoadClass = 5;
  static constexpr uint32_t kCostOfLoadReferrerClass = 1;
  static constexpr uint32_t kCostOfDevirtCheck = 2;
  // The benefit of prediction must include the overhead of call we eliminate.
  // For virtual invokes, we have 3 instruction path length + 2 to account
  // for loads being indirect.
  static constexpr uint32_t kCostOfVirtualInvokes = 5;

  DISALLOW_COPY_AND_ASSIGN(HDevirtualization);
};

}  // namespace art

#endif  // ART_COMPILER_OPTIMIZING_EXTENSIONS_PASSES_DEVIRTUALIZATION_H_
