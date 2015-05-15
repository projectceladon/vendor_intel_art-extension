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
 */

#include "base/dumpable.h"
#include "base/timing_logger.h"
#include "code_generator.h"
#include "ext_utility.h"
#include "driver/compiler_driver.h"
#include "find_ivs.h"
#include "graph_visualizer.h"
#include "loop_formation.h"
#include "optimization.h"
#include "pass_framework.h"
#include "peeling.h"
#include "remove_unused_loops.h"
#include "scoped_thread_state_change.h"
#include "thread.h"

namespace art {

// Enumeration defining possible commands to be applied to each pass.
enum PassInstrumentation {
  kPassInsertBefore,
  kPassInsertAfter,
  kPassReplace,
};

/**
 * @brief Used to provide directive for custom pass placement.
 */
struct HCustomPassPlacement {
  const char* pass_to_place;      /**!< The pass that has a custom location. */
  const char* pass_relative_to;   /**!< The pass for which the new pass is relative to. */
  PassInstrumentation directive;  /**!< How to do the pass insert (before, after, etc). */
};

/**
 * @brief Static array holding information about custom placements.
 */
static HCustomPassPlacement kPassCustomPlacement[] = {
  { "loop_formation", "instruction_simplifier_after_bce", kPassInsertAfter },
  { "find_ivs", "loop_formation", kPassInsertAfter },
  { "remove_unused_loops", "find_ivs", kPassInsertAfter },
  { "loop_peeling", "select_generator", kPassInsertBefore },
  { "loop_formation_before_peeling", "loop_peeling", kPassInsertBefore },
};

/**
 * @brief Static array holding names of passes that need removed.
 * @details This is done in cases where common code pass ordering and
 * existing passes are not appropriate or compatible with extension.
 */
static const char* kPassRemoval[] = {
  nullptr,
};

static void AddX86Optimization(HOptimization* optimization,
                               ArenaVector<HOptimization*>& list,
                               ArenaSafeMap<const char*, HCustomPassPlacement*> &placements) {
  ArenaSafeMap<const char*, HCustomPassPlacement*>::iterator iter = placements.find(optimization->GetPassName());

  if (iter == placements.end()) {
    return;
  }

  HCustomPassPlacement* placement = iter->second;

  // Find the right pass to change now.
  size_t len = list.size();
  size_t idx;
  for (idx = 0; idx < len; idx++) {
    if (strcmp(list[idx]->GetPassName(), placement->pass_relative_to) == 0) {
      switch (placement->directive) {
        case kPassReplace:
          list[idx] = optimization;
          break;
        case kPassInsertBefore:
        case kPassInsertAfter:
          // Add an empty element.
          list.push_back(nullptr);

          // Find the start, is it idx or idx + 1?
          size_t start = idx;

          if (placement->directive == kPassInsertAfter) {
            start++;
          }

          // Push elements backwards.
          DCHECK_NE(len, list.size());
          for (size_t idx2 = len; idx2 >= start; idx2--) {
            list[idx2] = list[idx2 - 1];
          }

          // Place the new element.
          list[start] = optimization;
          break;
      }
      // Done here.
      break;
    }
  }
  // It must be the case that the custom placement was found.
  DCHECK_NE(len, idx) << "couldn't insert " << optimization->GetPassName() << " relative to " << placement->pass_relative_to;
}

static void FillCustomPlacement(ArenaSafeMap<const char*, HCustomPassPlacement*>& placements) {
  size_t len = arraysize(kPassCustomPlacement);

  for (size_t i = 0; i < len; i++) {
    placements.Overwrite(kPassCustomPlacement[i].pass_to_place, kPassCustomPlacement + i);
  }
}

static void FillOptimizationList(HGraph* graph,
                                 ArenaVector<HOptimization*>& list,
                                 HOptimization_X86* optimizations_x86[],
                                 size_t opts_x86_length) {
  // Get the custom placements for our passes.
  ArenaSafeMap<const char*, HCustomPassPlacement*> custom_placement(
      std::less<const char*>(),
      graph->GetArena()->Adapter(kArenaAllocMisc));
  FillCustomPlacement(custom_placement);

  for (size_t i = 0; i < opts_x86_length; i++) {
    HOptimization_X86* opt = optimizations_x86[i];
    if (opt != nullptr) {
      AddX86Optimization(opt, list, custom_placement);
    }
  }
}

/**
 * @brief Apply the post optimization passes.
 * @param post_opt_list post-optimization list.
 * @param pass_observer the PassObserver.
 */
static void ApplyPostOpts(const ArenaVector<HOptimization*>& post_opt_list,
                          PassObserver* pass_observer) {
  for (auto post_opt : post_opt_list) {
    if (post_opt != nullptr) {
      RunOptWithPassScope scope(post_opt, pass_observer);
      scope.Run();
    }
  }
}

/**
 * @brief Remove the passes in the optimization and post-optimization lists.
 * @param opts the optimization vector.
 * @param post_opts the post-optimization vector.
 * @param driver the compilation driver.
 */
static void RemoveOptimizations(ArenaVector<HOptimization*>& opts,
                                ArenaVector<HOptimization*>& post_opts,
                                CompilerDriver* driver) {
  std::unordered_set<std::string> disabled_passes;

  SplitStringIntoSet(driver->GetCompilerOptions().
                       GetPassManagerOptions()->GetDisablePassList(),
                     ',',
                     disabled_passes);

  // Add elements from kPassRemoval.
  for (size_t i = 0, len = arraysize(kPassRemoval); i < len; i++) {
    if (kPassRemoval[i] != nullptr) {
      disabled_passes.insert(std::string(kPassRemoval[i]));
    }
  }

  // If there are no disabled passes, bail.
  if (disabled_passes.empty()) {
    return;
  }

  size_t opts_len = opts.size();

  // We replace the opts, post_opts with nullptr if we find a match.
  //   This is cheaper than rearranging the vectors.
  for (size_t opts_idx = 0; opts_idx < opts_len; opts_idx++) {
    HOptimization* opt = opts[opts_idx];
    if (disabled_passes.find(opt->GetPassName()) != disabled_passes.end()) {
      opts[opts_idx] = nullptr;
    }
  }

  size_t post_opts_len = post_opts.size();

  for (size_t post_opts_idx = 0; post_opts_idx < post_opts_len; post_opts_idx++) {
    HOptimization* post_opt = post_opts[post_opts_idx];
    if (disabled_passes.find(post_opt->GetPassName()) != disabled_passes.end()) {
      post_opts[post_opts_idx] = nullptr;
    }
  }
}

void PrintPasses(ArenaVector<HOptimization*>& opts,
                 ArenaVector<HOptimization*>& post_opts) {
  size_t opts_len = opts.size();
  size_t post_opts_len = post_opts.size();

  // We replace the opts, post_opts with nullptr if we find a match.
  //   This is cheaper than rearranging the vectors.
  LOG(INFO) << "Pass List:";
  if (opts_len == 0) {
    LOG(INFO) << "\t<Empty>";
  }

  for (size_t opts_idx = 0; opts_idx < opts_len; opts_idx++) {
    HOptimization* opt = opts[opts_idx];
    if (opt != nullptr) {
      LOG(INFO) << "\t- " << opt->GetPassName();
    }
  }

  LOG(INFO) << "Post-Opt List:";
  if (post_opts_len == 0) {
    LOG(INFO) << "\t<Empty>";
  }

  for (size_t post_opts_idx = 0; post_opts_idx < post_opts_len; post_opts_idx++) {
    HOptimization* post_opt = post_opts[post_opts_idx];
    if (post_opt != nullptr) {
      LOG(INFO) << "\t- " << post_opt->GetPassName();
    }
  }
}

bool PrintPassesOnlyOnce(ArenaVector<HOptimization*>& opts,
                         ArenaVector<HOptimization*>& post_opts,
                         CompilerDriver* driver) {
  bool need_print = driver->GetCompilerOptions().
                            GetPassManagerOptions()->GetPrintPassNames();

  if (!need_print) {
    return false;
  }

  // Flags that we have already printed the pass name list.
  static volatile bool pass_names_already_printed_ = false;

  // Have we already printed the names?
  if (!pass_names_already_printed_) {
    // Double-check it under the lock.
    ScopedObjectAccess soa(Thread::Current());
    if (!pass_names_already_printed_) {
      pass_names_already_printed_ = true;
    } else {
      need_print = false;
    }
  } else {
    need_print = false;
  }

  if (!need_print) {
    return false;
  }

  PrintPasses(opts, post_opts);
  return true;
}

/**
 * @brief Fill the vector from an array.
 * @param array the array of HOptimization.
 * @param len the length of the array.
 * @param vector the ArenaVector to fill.
 */
static void FillPassList(HOptimization_X86** array, size_t len, ArenaVector<HOptimization*>& vector) {
  for (size_t i = 0; i < len; i++) {
    HOptimization_X86* pass = array[i];

    if (pass != nullptr) {
      vector.push_back(pass);
    }
  }
}

/**
 * @brief Sets verbosity for passes.
 * @param optimizations the optimization array.
 * @param opts_len the length of optimizations array.
 * @param post_optimizations the post-optimization array.
 * @param post_opts_len the length of post-optimizations array.
 * @param driver the compilation driver.
 */
void FillVerbose(HOptimization_X86* optimizations[],
                 size_t opts_len,
                 HOptimization_X86* post_optimizations[],
                 size_t post_opts_len,
                 CompilerDriver* driver) {
  std::unordered_set<std::string> print_passes;
  const bool print_all_passes = driver->GetCompilerOptions().
                                GetPassManagerOptions()->GetPrintAllPasses();
  if (!print_all_passes) {
    // If we don't print all passes, we need to check the list.
    SplitStringIntoSet(driver->GetCompilerOptions().
                         GetPassManagerOptions()->GetPrintPassList(),
                       ',',
                       print_passes);

    // Are there any passes to print?
    if (print_passes.empty()) {
      return;
    }
  }

  for (size_t opts_idx = 0; opts_idx < opts_len; opts_idx++) {
    HOptimization* opt = optimizations[opts_idx];
    if (opt != nullptr) {
      if (print_all_passes ||
          print_passes.find(opt->GetPassName()) != print_passes.end()) {
        optimizations[opts_idx]->SetVerbose(true);
      }
    }
  }

  for (size_t post_opts_idx = 0; post_opts_idx < post_opts_len; post_opts_idx++) {
    HOptimization* post_opt = post_optimizations[post_opts_idx];
    if (post_opt != nullptr) {
      if (print_all_passes ||
          print_passes.find(post_opt->GetPassName()) != print_passes.end()) {
        post_optimizations[post_opts_idx]->SetVerbose(true);
      }
    }
  }
}

void RunOptimizationsX86(HGraph* graph,
                         CompilerDriver* driver,
                         OptimizingCompilerStats* stats,
                         ArenaVector<HOptimization*>& opt_list,
                         PassObserver* pass_observer) {
  // We want our own list of passes with our own vector.
  ArenaVector<HOptimization*> post_opt_list(graph->GetArena()->Adapter(kArenaAllocMisc));
  ArenaSet<const char*> post_opt_request(graph->GetArena()->Adapter(kArenaAllocMisc));

  // Create the array for the opts.
  HLoopFormation loop_formation(graph);
  HFindInductionVariables find_ivs(graph, stats);
  HRemoveUnusedLoops remove_unused_loops(graph, stats);
  HLoopFormation formation_before_peeling(graph, "loop_formation_before_peeling");
  HLoopPeeling peeling(graph, stats);
  HOptimization_X86* opt_array[] = {
    &loop_formation,
    &find_ivs,
    &remove_unused_loops,
    &peeling,
    &formation_before_peeling,
  };

  // Create the array for the post-opts.
  HOptimization_X86* post_opt_array[] = {
    nullptr,
  };

  // Fill verbose flags where we need it.
  FillVerbose(opt_array, arraysize(opt_array),
              post_opt_array, arraysize(post_opt_array),
              driver);

  // Create the vector for the optimizations.
  FillOptimizationList(graph, opt_list, opt_array, arraysize(opt_array));

  // Create the vector for the post opts.
  FillPassList(post_opt_array, arraysize(post_opt_array), post_opt_list);

  // Finish by removing the ones we do not want.
  RemoveOptimizations(opt_list, post_opt_list, driver);

  // Print the pass list, if needed.
  PrintPassesOnlyOnce(opt_list, post_opt_list, driver);

  // Now execute the optimizations.
  for (auto optimization : opt_list) {
    if (optimization != nullptr) {
      {
        const char* name = optimization->GetPassName();
        VLOG(compiler) << "Applying " << name;
        RunOptWithPassScope scope(optimization, pass_observer);
        scope.Run();
      }

      // Apply post opts: for optimizing compiler, we assume the post-opts
      //   know when to run or not to limit compile time.
      ApplyPostOpts(post_opt_list, pass_observer);
    }
  }
}
}  // namespace art
