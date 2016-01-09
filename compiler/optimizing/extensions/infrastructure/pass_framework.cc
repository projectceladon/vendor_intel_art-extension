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
#include "constant_calculation_sinking.h"
#include "ext_utility.h"
#include "driver/compiler_driver.h"
#include "form_bottom_loops.h"
#include "find_ivs.h"
#include "generate_selects.h"
#include "graph_visualizer.h"
#include "gvn_after_fbl.h"
#include "loadhoist_storesink.h"
#include "loop_formation.h"
#ifndef SOFIA
#include "non_temporal_move.h"
#endif
#include "optimization.h"
#include "option_content.h"
#include "pass_framework.h"
#include "peeling.h"
#include "pure_invokes_analysis.h"
#include "remove_suspend.h"
#include "remove_unused_loops.h"
#include "scoped_thread_state_change.h"
#include "thread.h"
#include "trivial_loop_evaluator.h"
#include "value_propagation_through_heap.h"

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
  { "loop_formation", "GVN_after_form_bottom_loops", kPassInsertAfter },
  { "find_ivs", "loop_formation", kPassInsertAfter },
  { "remove_loop_suspend_checks", "find_ivs", kPassInsertAfter},
  { "remove_unused_loops", "remove_loop_suspend_checks", kPassInsertAfter },
  { "loadhoist_storesink", "remove_unused_loops", kPassInsertBefore},
  { "loop_peeling", "select_generator", kPassInsertBefore },
  { "loop_formation_before_peeling", "loop_peeling", kPassInsertBefore },
  { "constant_calculation_sinking", "find_ivs", kPassInsertAfter},
  { "load_store_elimination", "instruction_simplifier_after_bce", kPassInsertAfter },
  { "form_bottom_loops", "load_store_elimination", kPassInsertAfter },
  // FIXME: this pass is disabled and should be eliminated
  // completely because Google has implemented a similar
  // optimization, called "select_generator".
  // { "generate_selects", "boolean_simplifier", kPassInsertAfter },
  { "GVN_after_form_bottom_loops", "form_bottom_loops", kPassInsertAfter },
  // TODO: Google has implemented load/store elimination optimization.
  // We should evaluate that optimization and decide, whether we need
  // value_propagation_through_heap or not.
  // { "value_propagation_through_heap", "GVN_after_form_bottom_loops", kPassInsertAfter },
  { "loop_formation_before_bottom_loops", "form_bottom_loops", kPassInsertBefore },
  { "non_temporal_move", "trivial_loop_evaluator", kPassInsertAfter},
  { "trivial_loop_evaluator", "find_ivs", kPassInsertAfter},
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
 * @brief Remove the passes in the optimization list.
 * @param opts the optimization vector.
 * @param driver the compilation driver.
 */
static void RemoveOptimizations(ArenaVector<HOptimization*>& opts,
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

  // We replace the opts with nullptr if we find a match.
  //   This is cheaper than rearranging the vectors.
  for (size_t opts_idx = 0; opts_idx < opts_len; opts_idx++) {
    HOptimization* opt = opts[opts_idx];
    if (disabled_passes.find(opt->GetPassName()) != disabled_passes.end()) {
      opts[opts_idx] = nullptr;
    }
  }
}

void PrintPasses(ArenaVector<HOptimization*>& opts) {
  size_t opts_len = opts.size();

  // We replace the opts with nullptr if we find a match.
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
}

void FillUserPassOptions(ArenaVector<HOptimization*>& opts,
                         CompilerDriver* driver) {
  const PassManagerOptions* pass_options = driver->GetCompilerOptions().GetPassManagerOptions();
  const std::string& options = pass_options->GetOverriddenPassOptions();
  const size_t options_len = options.size();

  // Before anything, check if we care about anything right now.
  if (options_len == 0) {
    return;
  }

  for (auto optimization : opts) {
    if (optimization == nullptr) {
      continue;
    }

    const char* pass_name = optimization->GetPassName();
    const size_t pass_name_len = strlen(pass_name);
    const size_t min_option_size = 4;  // 2 delimiters, 1 option name, 1 option.
    size_t search_pos = 0;

    // If there is no room for pass options, exit early.
    if (options_len < pass_name_len + min_option_size) {
      continue;
    }

    do {
      search_pos = options.find(pass_name, search_pos);

      // Check if we found this pass name in rest of string.
      if (search_pos == std::string::npos) {
        // No more options for this pass.
        break;
      }

      // The string contains the pass name. Now check that there is
      // room for the options: at least one char for option name,
      // two chars for two delimiter, and at least one char for option.
      if (search_pos + pass_name_len + min_option_size >= options_len) {
        // No more options for this pass.
        break;
      }

      // Update the current search position to not include the pass name.
      search_pos += pass_name_len;

      // The format must be "PassName:SettingName:#" where # is the option.
      // Thus look for the first ":" which must exist.
      if (options[search_pos] != ':') {
        // Missing delimiter right after pass name.
        continue;
      } else {
        search_pos += 1;
      }

      // Now look for the actual option by finding the next ":" delimiter.
      const size_t option_name_pos = search_pos;
      size_t option_pos = options.find(':', option_name_pos);

      if (option_pos == std::string::npos) {
        // Missing a delimiter that would capture where option starts.
        continue;
      } else if (option_pos == option_name_pos) {
        // Missing option thus did not move from option name.
        continue;
      } else {
        // Skip the delimiter.
        option_pos += 1;
      }

      // Look for the terminating delimiter which must be a comma.
      size_t next_configuration_separator = options.find(',', option_pos);
      if (next_configuration_separator == std::string::npos) {
        next_configuration_separator = options_len;
      }

      // Prevent end of string errors.
      if (next_configuration_separator == option_pos) {
          continue;
      }

      // Get the actual option itself.
      std::string option_string =
          options.substr(option_pos, next_configuration_separator - option_pos);

      std::string option_name =
          options.substr(option_name_pos, option_pos - option_name_pos - 1);

      // We attempt to convert the option value to integer. Strtoll is being used to
      // convert because it is exception safe.
      char* end_ptr = nullptr;
      const char* option_ptr = option_string.c_str();
      DCHECK(option_ptr != nullptr);  // Paranoid: option_ptr must be a valid pointer.
      int64_t int_value = strtoll(option_ptr, &end_ptr, 0);
      DCHECK(end_ptr != nullptr);  // Paranoid: end_ptr must be set by the strtoll call.

      // If strtoll call succeeded, the option is now considered as integer.
      if (*option_ptr != '\0' && end_ptr != option_ptr && *end_ptr == '\0') {
        optimization->DefineOption(option_name, OptionContent(int_value));
      } else {
        // Otherwise, it is considered as a string.
        optimization->DefineOption(option_name, OptionContent(option_string.c_str()));
      }
      search_pos = next_configuration_separator;
    } while (true);
  }
}

bool PrintPassesOnlyOnce(ArenaVector<HOptimization*>& opts,
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

  PrintPasses(opts);
  return true;
}

/**
 * @brief Sets verbosity for passes.
 * @param optimizations the optimization array.
 * @param opts_len the length of optimizations array.
 * @param driver the compilation driver.
 */
void FillVerbose(HOptimization_X86* optimizations[],
                 size_t opts_len,
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
}

void RunOptimizationsX86(HGraph* graph,
                         CompilerDriver* driver,
                         OptimizingCompilerStats* stats,
                         ArenaVector<HOptimization*>& opt_list,
                         PassObserver* pass_observer) {
  // Create the array for the opts.
  ArenaAllocator* arena = graph->GetArena();
  HLoopFormation* loop_formation = new (arena) HLoopFormation(graph);
  HFindInductionVariables* find_ivs = new (arena) HFindInductionVariables(graph, stats);
  HRemoveLoopSuspendChecks* remove_suspends = new (arena) HRemoveLoopSuspendChecks(graph, stats);
  HRemoveUnusedLoops* remove_unused_loops = new (arena) HRemoveUnusedLoops(graph, stats);
  TrivialLoopEvaluator* tle = new (arena) TrivialLoopEvaluator(graph, stats);
  HConstantCalculationSinking* ccs = new (arena) HConstantCalculationSinking(graph, stats);
#ifndef SOFIA
  HNonTemporalMove* non_temporal_move = new (arena) HNonTemporalMove(graph, stats);
#endif
  LoadHoistStoreSink* lhss = new (arena) LoadHoistStoreSink(graph, stats);
  // HValuePropagationThroughHeap value_propagation_through_heap(graph, stats);
  HLoopFormation* formation_before_peeling =
      new (arena) HLoopFormation(graph, "loop_formation_before_peeling");
  HLoopPeeling* peeling = new (arena) HLoopPeeling(graph, stats);
  HPureInvokesAnalysis* pure_invokes_analysis = new (arena) HPureInvokesAnalysis(graph, stats);
  HLoopFormation* formation_before_bottom_loops =
      new (arena) HLoopFormation(graph, "loop_formation_before_bottom_loops");
  HFormBottomLoops* form_bottom_loops = new (arena) HFormBottomLoops(graph, stats);
  GVNAfterFormBottomLoops* gvn_after_fbl = new (arena) GVNAfterFormBottomLoops(graph);
  // HGenerateSelects* generate_selects = new (arena) HGenerateSelects(graph, stats);

  HOptimization_X86* opt_array[] = {
    form_bottom_loops,
    gvn_after_fbl,
    formation_before_bottom_loops,
    loop_formation,
    find_ivs,
    remove_suspends,
    ccs,
    remove_unused_loops,
    lhss,
    peeling,
    pure_invokes_analysis,
    formation_before_peeling,
    tle,
#ifndef SOFIA
    non_temporal_move,
#endif
    // &generate_selects
    // &value_propagation_through_heapvalue_propagation_through_heap,
  };

  // Fill verbose flags where we need it.
  FillVerbose(opt_array, arraysize(opt_array),
              driver);

  // Create the vector for the optimizations.
  FillOptimizationList(graph, opt_list, opt_array, arraysize(opt_array));

  // Finish by removing the ones we do not want.
  RemoveOptimizations(opt_list, driver);

  // Print the pass list, if needed.
  PrintPassesOnlyOnce(opt_list, driver);

  FillUserPassOptions(opt_list, driver);

  // Now execute the optimizations.
  for (auto optimization : opt_list) {
    if (optimization != nullptr) {
      const char* name = optimization->GetPassName();
      VLOG(compiler) << "Applying " << name;
      RunOptWithPassScope scope(optimization, pass_observer);
      scope.Run();
    }
  }
}
}  // namespace art
