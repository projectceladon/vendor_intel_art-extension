/*
 * Copyright (C) 2014 The Android Open Source Project
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

#include <vector>
#include <algorithm>

#include "vtune_support.h"
#include "mapping_table.h"
#include "oat_file-inl.h"
#include "dex/dex_file-inl.h"
#include "dex/dex_instruction.h"
#include "vtune/jitprofiling.h"
#include "dex/code_item_accessors-inl.h"

namespace art {

typedef std::vector<LineNumberInfo> LineInfoTable;

struct SortLineNumberInfoByOffset {
  bool operator()(LineNumberInfo const& lhs, LineNumberInfo const& rhs) {
    return lhs.Offset < rhs.Offset;
  }

  static void sort(LineInfoTable &table) {
    std::sort(table.begin(), table.end(), SortLineNumberInfoByOffset());
  }
};

struct SortLineNumberInfoByLine {
  static const unsigned NOT_FOUND = 0 - 1;

  bool operator()(LineNumberInfo const& lhs, LineNumberInfo const& rhs) {
    return lhs.LineNumber < rhs.LineNumber;
  }

  static void sort(LineInfoTable &table) {
    std::sort(table.begin(), table.end(), SortLineNumberInfoByLine());
  }

  static int find(LineInfoTable &table, unsigned line, bool strict = false) {
    LineInfoTable::iterator lower =
           std::lower_bound(table.begin(),
                            table.end(),
                            LineNumberInfo({0, line}),
                            SortLineNumberInfoByLine());
    return lower == table.end() || (strict && lower->LineNumber != line)
           ? NOT_FOUND
           : lower->Offset;
  }
};

// Reads debug information from dex file to get BC to Java line mapping
static void getLineInfoForJava(const uint8_t *dbgstream,
                               LineInfoTable& table,
                               LineInfoTable& pc2dex) {
  SortLineNumberInfoByLine::sort(pc2dex);

  int adjopcode;
  uint32_t address = 0;
  uint32_t line = DecodeUnsignedLeb128(&dbgstream);

  // skip parameters
  for(uint32_t param_count = DecodeUnsignedLeb128(&dbgstream); param_count != 0; --param_count) {
    DecodeUnsignedLeb128(&dbgstream);
  }

  for(bool is_end = false; is_end == false; ) {
    uint8_t opcode = *dbgstream++;
    switch (opcode) {
    case DexFile::DBG_END_SEQUENCE:
      is_end = true;
      break;

    case DexFile::DBG_ADVANCE_PC:
      address += DecodeUnsignedLeb128(&dbgstream);
      break;

    case DexFile::DBG_ADVANCE_LINE:
      line += DecodeSignedLeb128(&dbgstream);
      break;

    case DexFile::DBG_START_LOCAL:
    case DexFile::DBG_START_LOCAL_EXTENDED:
      DecodeUnsignedLeb128(&dbgstream);
      DecodeUnsignedLeb128(&dbgstream);
      DecodeUnsignedLeb128(&dbgstream);

      if (opcode == DexFile::DBG_START_LOCAL_EXTENDED) {
        DecodeUnsignedLeb128(&dbgstream);
      }
      break;

    case DexFile::DBG_END_LOCAL:
    case DexFile::DBG_RESTART_LOCAL:
      DecodeUnsignedLeb128(&dbgstream);
      break;

    case DexFile::DBG_SET_PROLOGUE_END:
    case DexFile::DBG_SET_EPILOGUE_BEGIN:
    case DexFile::DBG_SET_FILE:
      break;

    default:
      adjopcode = opcode - DexFile::DBG_FIRST_SPECIAL;
      address += adjopcode / DexFile::DBG_LINE_RANGE;
      line += DexFile::DBG_LINE_BASE + (adjopcode % DexFile::DBG_LINE_RANGE);

      unsigned offset;
      offset = SortLineNumberInfoByLine::find(pc2dex, address);
      if (offset != SortLineNumberInfoByLine::NOT_FOUND) {
        table.push_back({offset, line});
      }
      break;
    }
  }
}

// convert dex offsets to dex disassembly line numbers in pc2dex
static void getLineInfoForDex(const CodeItemDataAccessor& accessor, LineInfoTable& pc2dex) {
  SortLineNumberInfoByLine::sort(pc2dex);

  size_t offset = 0; // in code units
  size_t end = accessor.InsnsSizeInCodeUnits();
  int line_no = 1;
  auto li = pc2dex.begin();
  for ( ; offset < end && li != pc2dex.end(); ++line_no) {
    const uint16_t* insns = accessor.Insns() + offset;
    const Instruction* instr = Instruction::At(insns);
    for ( ; li != pc2dex.end() && li->LineNumber <= offset; li++) {
      // REVIEW: to be strict we have to remove li if li->lineNumber != offset
      li->LineNumber = line_no;
    }
    offset += instr->SizeInCodeUnits();
  }
}

// TODO: come up with another way to get options:
// 1. filter by app package (process nice name)
// 2. filter by dex/oat/class/method
// 3. output map type: none/dex/java
// 4. vtune jit api library path (set INTEL_JIT_PROFILER32 or
//    INTEL_JIT_PROFILER64 environment variable)
// 5. set any other environment vars for the profiler lib (output path, ...)
static const char * ART_VTUNE_JIT_API_SRC = getenv("ART_VTUNE_JIT_API_SRC");

void SendMethodToVTune(const char* method_name,
                       const void* code,
                       size_t code_size,
                       const char* class_file_name,
                       const char* source_file_name,
                       const DexFile* dex_file,
		       const uint32_t method_idx,
                       const DexFile::CodeItem* code_item) {
  if (code == nullptr) {
    return;
  }

  // Older versions of VTune treated IDs < 1000 as reserved.
  static unsigned next_method_id = 1000;
  next_method_id++;

  const char *method_name_c = strchr(method_name, ' '); // skip return type

  iJIT_Method_Load jit_method;
  memset(&jit_method, 0, sizeof(jit_method));
  jit_method.class_file_name = const_cast<char*>(class_file_name);
  jit_method.source_file_name = const_cast<char*>(source_file_name);
  jit_method.method_name = const_cast<char*>(method_name_c == nullptr ?
      method_name : (method_name_c + 1));
  jit_method.method_id = next_method_id;
  jit_method.method_load_address = const_cast<void*>(code);
  jit_method.method_size = code_size;

  // dump positions
  // TODO: Get the table from StackMaps
  MappingTable table(nullptr);

  // prepare pc2src to point to either pc2java or pc2dex
  LineInfoTable *pc2src = nullptr;
  LineInfoTable pc2dex;
  LineInfoTable pc2java;
  CodeItemDebugInfoAccessor accessor(*dex_file, code_item, method_idx);
  if (dex_file != nullptr && code_item != nullptr &&
      (ART_VTUNE_JIT_API_SRC == nullptr || strcmp(ART_VTUNE_JIT_API_SRC, "none") != 0) &&
      table.TotalSize() != 0 && table.PcToDexSize() != 0) {
    for (MappingTable::PcToDexIterator cur = table.PcToDexBegin(),
                                       end = table.PcToDexEnd(); cur != end; ++cur) {
      pc2dex.push_back({cur.NativePcOffset(), cur.DexPc()});
    }

    if (ART_VTUNE_JIT_API_SRC != nullptr && strcmp(ART_VTUNE_JIT_API_SRC, "dex") == 0) {
      pc2src = &pc2dex;
      getLineInfoForDex(accessor, pc2dex);
      // TODO: set dexdump file name for this method
    } else { // default is pc -> java
      pc2src = &pc2java;

      //CodeItemDebugInfoAccessor accessor(dex_file, code_item, method_idx);
      const uint8_t* dbgstream = dex_file->GetDebugInfoStream(accessor.DebugInfoOffset());
      getLineInfoForJava(dbgstream, pc2java, pc2dex);
    }
  }

  if (pc2src == nullptr || pc2src->size() == 0) {
    jit_method.line_number_size = 0;
    jit_method.line_number_table = nullptr;
  } else {
    // shift offsets
    SortLineNumberInfoByOffset::sort(*pc2src);
    for (unsigned i = 1; i < pc2src->size(); ++i) {
      (*pc2src)[i - 1].Offset = (*pc2src)[i].Offset;
    }
    (*pc2src)[pc2src->size() - 1].Offset = code_size;

    jit_method.line_number_size = pc2src->size();
    jit_method.line_number_table = pc2src->size() == 0 ? nullptr : &((*pc2src)[0]);
  }

  int is_notified = iJIT_NotifyEvent(iJVM_EVENT_TYPE_METHOD_LOAD_FINISHED, (void*)&jit_method);
  if (is_notified) {
    LOG(DEBUG) << "JIT API: method '" << jit_method.method_name
               << "' is written successfully: id=" << jit_method.method_id
               << ", address=" << jit_method.method_load_address
               << ", size=" << jit_method.method_size;
  } else if (true) {
    LOG(WARNING) << "JIT API: failed to write method '" << jit_method.method_name
                 << "': id=" << jit_method.method_id
                 << ", address=" << jit_method.method_load_address
                 << ", size=" << jit_method.method_size;
  }
}

}  // namespace art
