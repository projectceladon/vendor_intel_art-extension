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

#include "ext_utility.h"

namespace art {

  std::string GetMethodName(const HGraph* graph) {
    DCHECK(graph != nullptr);
#if 0	//neeraj - resolve build error
    return PrettyMethod(graph->GetMethodIdx(), graph->GetDexFile());
#else
     return graph->GetDexFile().PrettyMethod(graph->GetMethodIdx());
#endif
  }

  void SplitStringIntoSet(const std::string& s, char delim, std::unordered_set<std::string>& tokens) {
    std::stringstream stream(s);
    std::string item;
    while (std::getline(stream, item, delim)) {
      if (item.size() > 1) {
        tokens.insert(item);
      }
    }
  }
}  // namespace art
