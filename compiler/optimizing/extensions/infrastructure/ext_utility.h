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

#ifndef COMPILER_OPTIMIZING_EXTENSIONS_INFRASTRUCTURE_EXT_UTILITY_H
#define COMPILER_OPTIMIZING_EXTENSIONS_INFRASTRUCTURE_EXT_UTILITY_H

#if 0	//neeraj - resolve build errors
#include "base/stringprintf.h"
#else
#include "android-base/stringprintf.h"
using android::base::StringPrintf;
#endif

#include "driver/dex_compilation_unit.h"
#include "nodes.h"
#include <ostream>
#include <unordered_set>

namespace art {
  // This macro can be used to print messages within passes. The possible usage is:
  // PRINT_PASS_MESSAGE(this, "Value of x = %.2f", x);
  #define PRINT_PASS_MESSAGE(pass, format, ...) \
    do { \
      if (pass->IsVerbose()) { \
        LOG(INFO) << pass->GetPassName() << ": " << StringPrintf(format, ##__VA_ARGS__); \
      } \
    } while (false)

  // PRINT_PASS_OSTREAM_MESSAGE(this, x << yy << zz);
  #define PRINT_PASS_OSTREAM_MESSAGE(pass, ...) \
    do { \
      if (pass->IsVerbose()) { \
        LOG(INFO) << pass->GetPassName() << ": " << __VA_ARGS__; \
      } \
    } while (false)

  /**
   * @brief Facility to get the name of the current method.
   * @param graph The HGgraph corresponding to the method.
   * @return The name of the current method.
   */
  std::string GetMethodName(const HGraph* graph);

  /**
   * @brief Split string by delimeter and put tokens to the set.
   * @param s The string to split.
   * @param delim The delimeter character.
   * @param tokens The output set for tokens.
   */
  void SplitStringIntoSet(const std::string& s, char delim, std::unordered_set<std::string>& tokens);
}  // namespace art
#endif  // COMPILER_OPTIMIZING_EXTENSIONS_INFRASTRUCTURE_EXT_UTILITIES_H
