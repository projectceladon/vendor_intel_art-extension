/*
 * INTEL CONFIDENTIAL
 * Copyright (c) 2016, Intel Corporation All Rights Reserved.
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

#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "base/mutex.h"
#include "base/stringprintf.h"
#include "dex_file-inl.h"
#include "thread-inl.h"
#include "ext_profiling.h"
#include "os.h"
#include "runtime.h"
#include "cmdline.h"

namespace art {

struct ProfDumpArgs : public CmdlineArgs {
 protected:
  using Base = CmdlineArgs;

  virtual ParseStatus ParseCustom(const StringPiece& option, std::string*) OVERRIDE {
    if (option.starts_with("--oat-file=")) {
      oat_filename_ = option.substr(strlen("--oat-file=")).data();
    } else if (option.starts_with("--prof-file=")) {
      prof_filename_ = option.substr(strlen("--prof-file=")).data();
    } else if (option == "-z") {
      show_zero_counts_ = true;
    } else if (option == "-m") {
      method_counts_only_ = true;
    } else if (option == "--add-class") {
      add_class_names_ = true;
    } else if (option == "--dump-and-zero") {
      dump_and_zero_ = true;
    } else {
      return kParseUnknownArgument;
    }

    return kParseOk;
  }

  virtual ParseStatus ParseChecks(std::string* error_msg) OVERRIDE {
    // Perform the parent checks.
    ParseStatus parent_checks = Base::ParseChecks(error_msg);
    if (parent_checks != kParseOk) {
      return parent_checks;
    }

    if (oat_filename_ != nullptr && prof_filename_ != nullptr) {
      *error_msg = "Only one of --oat-file or --prof-file must be specified";
      return kParseError;
    }

    // Perform our own checks.
    if (oat_filename_ == nullptr && prof_filename_ == nullptr) {
      *error_msg = "One of --oat-file or --prof-file must be specified";
      return kParseError;
    }

    return kParseOk;
  }

  virtual std::string GetUsage() const {
    std::string usage;

    usage += "Usage: profdump [-z] [-m [--add-class]] [--dump-and-zero]"
             "--oat-file=oat_file_name|--prof-file=prof_file_name\n";

    return usage;
  }

 public:
  bool show_zero_counts_ = false;
  bool method_counts_only_ = false;
  bool add_class_names_ = false;
  bool dump_and_zero_ = false;
  const char* oat_filename_ = nullptr;
  const char* prof_filename_ = nullptr;
};

static void ComputeClassNames(ExactProfileFile* hdr,
                              std::map<PersistentClassIndex, std::string>& map) {
  char* base = reinterpret_cast<char*>(hdr);
  if (hdr->offset_to_oat_table == 0) {
    // No Oat Table -> no invokes present in the file.
    return;
  }
  DCHECK_NE(hdr->offset_to_oat_table, 0U);
  DCHECK_NE(hdr->offset_to_oat_string_table, 0U);
  OatTable* oat_table = reinterpret_cast<OatTable*>(base + hdr->offset_to_oat_table);
  OatLocationStringTable* string_table =
      reinterpret_cast<OatLocationStringTable*>(base + hdr->offset_to_oat_string_table);
  // Collect all the OAT files we need.
  std::vector<OatFile*> oat_table_files;
  std::string error_msg;
  for (uint32_t i = 0; i < oat_table->num_oat_locations; i++) {
    OneOatIndex& oat_index = oat_table->oat_locations[i];
    char *oat_name = &string_table->strings[oat_index.offset_of_oat_location_string];
    OatFile* oat =
        OatFile::Open(oat_name, oat_name, nullptr, nullptr, false, nullptr, &error_msg);
    oat_table_files.push_back(oat);
    if (oat == nullptr) {
      std::cerr << "Unable to open OAT file: " << oat_name << ": " << error_msg << '\n';
    }
  }

  // Now walk the invokes and do the mapping.
  typedef std::pair<uint8_t, uint8_t> OatDexIndex;
  std::map<OatDexIndex, std::unique_ptr<const DexFile>> oat_dex_map;
  OneDexFile* dex_files = hdr->dex_info;
  for (uint32_t i = 0; i < hdr->num_dex_files; i++) {
    OneDexFile& df = dex_files[i];
    OneMethod* method_info = reinterpret_cast<OneMethod*>(base + df.base_of_methods);

    for (uint32_t j = 0; j < df.num_methods; j++) {
      for (uint32_t k = 0; k < method_info->num_method_invokes; k++) {
        OneCallSite* call_site = method_info->CallSiteAt(k);
        for (int l = 0; l < OneCallSite::kNumInvokeTargets; l++) {
          PersistentClassIndex& invoke = call_site->targets[l].class_index;
          if (invoke.IsNull()) {
            break;
          }
          if (map.find(invoke) != map.end()) {
            continue;
          }
          OatDexIndex odi(invoke.OatIndex(), invoke.DexIndex());
          auto it = oat_dex_map.find(odi);
          const DexFile* dex_file = nullptr;
          OatFile* oat_file = oat_table_files.at(invoke.OatIndex());
          if (it != oat_dex_map.end()) {
            dex_file = it->second.get();
          } else if (oat_file != nullptr) {
            // Need to look it up.
            const OatDexFile* oat_dex_file = oat_file->GetOatDexFiles().at(invoke.DexIndex());
            if (oat_dex_file != nullptr) {
              std::unique_ptr<const DexFile> dex_file_ptr =
                  oat_dex_file->OpenDexFile(&error_msg);
              dex_file = dex_file_ptr.get();
              oat_dex_map.insert(std::make_pair(odi,std::move(dex_file_ptr)));
            }
          }
          if (dex_file == nullptr) {
            std::ostringstream ss;
            ss << '<' << static_cast<int>(invoke.OatIndex())
               << ':' << static_cast<int>(invoke.DexIndex())
               << ':' << static_cast<int>(invoke.DexIndex()) << '>';
            map.insert(std::make_pair(invoke, ss.str()));
          } else {
            std::string class_name =
                std::string(dex_file->StringByTypeIdx(invoke.ClassDefIndex()));
            class_name = PrettyDescriptor(class_name.c_str());
            map.insert(std::make_pair(invoke, class_name));
          }
        }
      }

      // Bump to next method.
      method_info = method_info->Next();
    }
  }
  for (auto oat_file : oat_table_files) {
    delete oat_file;
  }
}

static int DumpProfile(const ProfDumpArgs* args) {
  ExactProfileFile* hdr;
  const char* fname;
  bool no_oat_file = false;
  std::string error_msg;
  OatFile* oat_file = nullptr;

  std::map<PersistentClassIndex, std::string> class_name_map;
  if (args->prof_filename_ != nullptr) {
    DCHECK(args->oat_filename_ == nullptr);
    fname = args->prof_filename_;
    int fd = ::open(fname, O_RDONLY);
    if (fd < 0) {
      std::cerr << "Unable to open profile file: " << fname << ": " << strerror(errno);
      return EXIT_FAILURE;
    }
    // Read in the file
    struct stat file_info;
    ::fstat(fd, &file_info);
    char* buffer = new char[file_info.st_size];
    if (buffer == nullptr) {
      std::cerr << "Unable to allocate memory for profile file: " << fname;
      return EXIT_FAILURE;
    }

    hdr = reinterpret_cast<ExactProfileFile*>(buffer);

    size_t bytes_read = read(fd, buffer, file_info.st_size);
    close(fd);
    if (bytes_read != static_cast<size_t>(file_info.st_size)) {
      std::cerr << "Problem reading profile file: " << fname;
      return EXIT_FAILURE;
    }
    no_oat_file = true;
    if (!args->method_counts_only_) {
      ComputeClassNames(hdr, class_name_map);
    }
  } else {
    fname = args->oat_filename_;
    oat_file = OatFile::Open(fname, fname, nullptr, nullptr, false, nullptr, &error_msg);
    if (oat_file == nullptr) {
      std::cerr << "Unable to open OAT file: " << fname << ": " << error_msg;
      return EXIT_FAILURE;
    }

    hdr = ExactProfiler::GetProfileFile(*oat_file, false, false);
    if (hdr == nullptr) {
      std::cerr << "Unable to open matching profile file for OAT file: " << fname << '\n';
      return EXIT_FAILURE;
    }
    if (!args->method_counts_only_ && hdr->offset_to_oat_table != 0U) {
      // Unfortunately, we have a problem with multiple open OAT files.
      delete oat_file;
      ComputeClassNames(hdr, class_name_map);
      oat_file = OatFile::Open(fname, fname, nullptr, nullptr, false, nullptr, &error_msg);
      DCHECK(oat_file != nullptr);
    }
  }

  // Fixed header.
  char* base = reinterpret_cast<char*>(hdr);
  if (hdr->magic1 != ExactProfiler::Magic1_) {
    std::cerr << "Incorrect magic number: 0x" << std::hex << hdr->magic1
              << ", expected 0x" << ExactProfiler::Magic1_ << '\n';
    return EXIT_FAILURE;
  }

  if (hdr->version != ExactProfiler::CurrentVersion_) {
    std::cerr << "Incorrect version: " << hdr->version << ", expected "
              << ExactProfiler::CurrentVersion_ << '\n';
    return EXIT_FAILURE;
  }

  if (!args->method_counts_only_) {
    std::cout << "OAT checksum: 0x" << std::hex << hdr->oat_checksum << '\n' << std::dec;
    std::cout << "Number of dex files: " << hdr->num_dex_files << '\n';
    std::cout << "Total Number of counters: " << hdr->total_num_counters << '\n';
    std::cout << "Total Number of methods: " << hdr->total_num_methods << '\n';
    std::cout << "Offset to oat table: " << hdr->offset_to_oat_table << '\n';
    std::cout << "Offset to oat string table: " << hdr->offset_to_oat_string_table << '\n';
    std::cout << "Variable start offset: 0x" << std::hex
              << hdr->variable_start_offset << std::dec << '\n';
    if (hdr->offset_to_oat_table != 0U) {
      DCHECK_NE(hdr->offset_to_oat_string_table, 0U);
      OatTable* oat_table = reinterpret_cast<OatTable*>(base + hdr->offset_to_oat_table);
      OatLocationStringTable* string_table =
          reinterpret_cast<OatLocationStringTable*>(base + hdr->offset_to_oat_string_table);
      for (uint32_t i = 0; i < oat_table->num_oat_locations; i++) {
        OneOatIndex& oat_index = oat_table->oat_locations[i];
        char *oat_name = &string_table->strings[oat_index.offset_of_oat_location_string];
        std::cout << "Oat index(" << i << "): " << oat_name
                  << ", checksum = 0x" << std::hex << oat_index.oat_checksum
                  << std::dec << '\n';
        }
    }
  }

  // Dump each dex file.
  OneDexFile* dex_files = hdr->dex_info;
  for (uint32_t i = 0; i < hdr->num_dex_files; i++) {
    OneDexFile& df = dex_files[i];
    std::unique_ptr<const DexFile> real_dex_file;
    if (!no_oat_file) {
      const OatDexFile* oat_dex_file = oat_file->GetOatDexFiles()[i];
      real_dex_file = oat_dex_file->OpenDexFile(&error_msg);
      if (real_dex_file == nullptr) {
        std::cerr << "Error opening dex file: " << error_msg;
      }
    }
    if (!args->method_counts_only_) {
      std::cout << "Dex file " << i << " (" << (base + df.offset_to_dex_file_name)
                << "):\n";
      std::cout << "Checksum: 0x" << std::hex << df.dex_checksum << '\n' << std::dec;
      std::cout << "Number of methods: " << df.num_methods << '\n';
      std::cout << "Offset to methods: " << df.base_of_methods << '\n';
      std::cout << "Offset to method index: " << df.method_index_offsets << '\n';
    }
    OneMethod* method_info = reinterpret_cast<OneMethod*>(base + df.base_of_methods);
    uint32_t* index_table =  reinterpret_cast<uint32_t*>(base + df.method_index_offsets);

    for (uint32_t j = 0; j < df.num_methods; j++) {
      bool all_zero = true;
      bool header_printed = false;
      OneMethod* prof_method = reinterpret_cast<OneMethod*>(base + index_table[j]);
      std::string method_name;
      if (args->method_counts_only_) {
        if (real_dex_file != nullptr) {
          method_name = PrettyMethod(j, *real_dex_file, true);
        } else {
          method_name += "<index " + std::to_string(j) + '>';
        }
      } else {
        if (real_dex_file != nullptr) {
          method_name = PrettyMethod(j, *real_dex_file, true);
          method_name += ' ';
        }
        method_name += "(index " + std::to_string(j) + ')';
      }
      if (prof_method != method_info) {
          std::cerr << "Method " << method_name
                    << ": index table value = " << index_table[j]
                    << "\nMethod " << j << " address mismatch: "
                    << reinterpret_cast<char*>(prof_method) - base
                    << " and "
                    << reinterpret_cast<char*>(method_info) - base
                    << '\n';
        return EXIT_FAILURE;
      }
      if (args->method_counts_only_) {
        OneMethod::CountType count = method_info->counts[0];
        if (count != 0 || args->show_zero_counts_) {
          std::cout << count << '\t';
          if (args->add_class_names_ && real_dex_file != nullptr) {
            const DexFile::MethodId& method_id = real_dex_file->GetMethodId(j);
            std::string class_name =
                PrettyDescriptor(real_dex_file->GetMethodDeclaringClassDescriptor(method_id));
            std::cout << class_name << '\t';
          }
          std::cout << method_name << '\n';
        }
      } else {
        for (uint32_t k = 0; k < method_info->num_blocks; k++) {
          if (method_info->counts[k]) {
            if (!header_printed) {
              header_printed = true;
              std::cout << "Method " << method_name << ": num_blocks = "
                        << method_info->num_blocks << ", num_method_invokes = "
                        << method_info->num_method_invokes << '\n';
            }
            all_zero = false;
            std::cout << "  Block " << k << " count: " << method_info->counts[k] << '\n';
          }
        }
        if (all_zero && args->show_zero_counts_) {
          std::cout << "Method " << method_name << ": num_blocks = "
                    << method_info->num_blocks << ", num_method_invokes = "
                    << method_info->num_method_invokes
                    << "\n  All block counts are zero\n";
        }

        // If there are any BB counts, also dump invoke types.
        if (method_info->num_method_invokes > 0 &&
            (args->show_zero_counts_ || !all_zero)) {
          for (uint32_t l = 0; l < method_info->num_method_invokes; l++) {
            OneCallSite* call_site = method_info->CallSiteAt(l);
            bool hdr_printed = false;
            for (int m = 0; m < OneCallSite::kNumInvokeTargets; m++) {
              OneInvoke& invoke = call_site->targets[m];
              if (invoke.class_index.IsNull()) {
                break;
              }
              if (!hdr_printed) {
                hdr_printed = true;
                std::cout << "  Dex pc: 0x" << std::hex << call_site->dex_pc << std::dec;
              }
              auto it = class_name_map.find(invoke.class_index);
              DCHECK(it != class_name_map.end());
              std::cout << ' ' << it->second << " count: " << invoke.count;
            }
            if (hdr_printed) {
              std::cout << '\n';
            }
          }
        }
      }

      if (args->dump_and_zero_) {
        // Zero the counts.  We will update the copy on disk later.
        memset(method_info->counts, '\0', method_info->num_blocks * sizeof(OneMethod::CountType));
        // Zero the invoke classes/counts.
        for (uint16_t site = 0; site < method_info->num_method_invokes; site++) {
          OneCallSite* call_site = method_info->CallSiteAt(site);
          memset(call_site->targets, '\0', sizeof(call_site->targets));
        }
      }

      // Bump to next method.
      method_info = method_info->Next();
    }
  }

  if (args->dump_and_zero_) {
    // Grab the saved filename from the runtime_temp field.
    DCHECK_NE(hdr->runtime_temp, 0ULL);
    // This avoids a lint warning/error.
    const char** profile_file_name_ptr = reinterpret_cast<const char **>(&hdr->runtime_temp);
    const char* profile_file_name = *profile_file_name_ptr;
    // Reset it for zeroing.
    hdr->runtime_temp = 0;

    // We want to open the file read/write, lock it, and write it out again.
    int fd = open(profile_file_name, O_RDWR);

    if (fd < 0) {
      std::cerr << "Unable to open profile file '" << profile_file_name
                << "' for zeroing: " << strerror(errno);
      return EXIT_FAILURE;
    }
    struct stat prof_stat;
    if (fstat(fd, &prof_stat) < 0) {
      close(fd);
      std::cerr << "Unable to fstat profile file '" << profile_file_name
                << "' for zeroing: " << strerror(errno);
      return EXIT_FAILURE;
    }

    // We need exclusive access to the file.
    if (flock(fd, LOCK_EX) < 0) {
      close(fd);
      std::cerr << "Unable to flock profile file '" << profile_file_name
                << "' for zeroing: " << strerror(errno);
      return EXIT_FAILURE;
    }

    size_t bytes_written = write(fd, hdr, prof_stat.st_size);
    if (bytes_written != static_cast<size_t>(prof_stat.st_size)) {
      std::cerr << "Problem writing profile file '" << profile_file_name
                << "' for zeroing: " << strerror(errno);
      return EXIT_FAILURE;
    }

    // Lock will be released on close.
    close(fd);
  }
  return EXIT_SUCCESS;
}

struct ProfdumpMain : public CmdlineMain<ProfDumpArgs> {
  virtual bool NeedsRuntime() OVERRIDE {
    return false;
  }

  virtual bool ExecuteWithoutRuntime() OVERRIDE {
    CHECK(args_ != nullptr);

    MemMap::Init();

    return DumpProfile(args_) == EXIT_SUCCESS;
  }

  std::unique_ptr<ProfDumpArgs> oat_dumper_options_;
};

}  // namespace art

int main(int argc, char* argv[]) {
  art::ProfdumpMain main;
  return main.Main(argc, argv);
}
