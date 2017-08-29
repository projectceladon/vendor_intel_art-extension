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

#include "base/arena_allocator.h"
#include "builder.h"
#include "dex_file.h"
#include "dex_instruction.h"
#include "graph_x86.h"
#include "nodes.h"
#include "loop_information.h"
#include "loop_iterators.h"
#include "loop_formation.h"
#include "optimizing_unit_test.h"

#include "gtest/gtest.h"

namespace art {

static void TestPeeling(const uint16_t* data, ArenaAllocator* allocator, size_t num_peeled_blocks) {
  // Build the graph.
  HGraph_X86* graph = CreateGraph_X86_for_test(allocator);
  HGraphBuilder builder(graph);
  const DexFile::CodeItem* item = reinterpret_cast<const DexFile::CodeItem*>(data);
  builder.BuildGraph(*item);
  graph->TryBuildingSsa();

  // Loop formation is needed to get loop hierarchy in place for peeling.
  HLoopFormation formation(graph);
  formation.Run();

  HOnlyInnerLoopIterator inner_iter(GRAPH_TO_GRAPH_X86(graph)->GetLoopInformation());
  while (!inner_iter.Done()) {
    HLoopInformation_X86* inner_loop = inner_iter.Current();
    if (inner_loop->IsPeelable(&formation)) {
      inner_loop->PeelHead(&formation);
    }
    ASSERT_EQ(inner_loop->GetPeeledBlockIds().Size(), num_peeled_blocks);
    inner_iter.Advance();
  }
}

TEST(LoopPeelingTest, ForLoop) {
  /*
   *  Simple for loop:
   *  int result = 0;
   *  for (int i = 0; i < 10; i++) {
   *    result += i;
   *  }
   *  return result;
   */

  const uint16_t data[] = THREE_REGISTERS_CODE_ITEM(
    Instruction::CONST_4 | 0,
    Instruction::MOVE | 1 << 8,
    Instruction::CONST_16 | 2 << 8, 0xa,
    Instruction::IF_GE | 0x20 << 8, 0x6,
    Instruction::ADD_INT_2ADDR | 1 << 8,
    Instruction::ADD_INT_LIT8 | 0, 0x100,
    Instruction::GOTO | 0xF9 << 8,
    Instruction::RETURN | 1 << 8);

  ArenaPool arena;
  ArenaAllocator allocator(&arena);
  // The loop above will generate two blocks - thus peel must also be two.
  TestPeeling(data, &allocator, 2u);
}
}  // namespace art
