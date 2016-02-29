/*
 * Copyright (C) 2015 The Android Open Source Project
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

#ifndef COMPILER_OPTIMIZING_EXTENSIONS_INFRASTRUCTURE_EXT_PROFILING_H
#define COMPILER_OPTIMIZING_EXTENSIONS_INFRASTRUCTURE_EXT_PROFILING_H

#include "base/mutex.h"
#include "oat_file.h"
#include "safe_map.h"

namespace art {

class DexFile;
class ExactProfileSaver;
class HGraph;
class HBasicBlock;

// These are defined outside of ExactProfiler, to be able to be used as a forward declaration.
struct OneDexFile {
  uint32_t offset_to_dex_file_name;    // Offset to UTF-8 filename in 0xB00DDADB section.
  uint32_t dex_checksum;               // Checksum of Dex file.
  uint32_t num_methods;                // Number of methods defined in the Dex file.
  uint32_t base_of_methods;            // Offset to first OneMethod for this dex file.
  uint32_t method_index_offsets;       // Offset to uint32_t[num_methods] of offsets.
  uint32_t dummy;                      // Ensure multiple of 64 bits.
};

struct OneMethod {
  typedef uint64_t CountType;

  uint16_t num_blocks;                   // number of blocks being counted in the method.
  uint32_t offset_to_per_method_data;    // Offset to data in 0xB00DDADC section.
  CountType counts[0 /* num_blocks */];  // Execution counts for the method.
                                         // Merging will cap at max(CountType)-1.
};

// Format for fixed part of prof file:
struct ExactProfileFile {
  uint32_t    magic1;                    // 0xB00DDADA.
  uint32_t    version;                   // Currently 0x1.
  uint32_t    oat_checksum;              // Checksum of oat file.
  uint32_t    num_dex_files;             // methods compiled by optimizing compiler.
  uint32_t    total_num_counters;        // Total number of counters.
  uint32_t    total_num_methods;         // Total number of methods.
  uint64_t    runtime_temp;              // Used by runtime to hold filename.
  OneDexFile  dex_info[0 /* num_dex_files */];  // Information about each Dex file.
};

class ExactProfiler {
 public:
  /*
   * @brief Construct the data structures for profiling counts.
   * @param oat_file_name Name of the OAT file, to be used to generate the
   * '.prof' filename.
   */
  explicit ExactProfiler(const std::string& oat_file_name);

  /*
   * Destructor
   */
  ~ExactProfiler() {
    for (auto it : existing_profiles_) {
      delete it;
    }
  }

  /*
   * @brief Register a method with the profiler.
   * @param dex_file Dex file which contains the method.
   * @param method_idx Method index within the dex file.
   * @param num_blocks Maximum block number profiled for this method.
   */
  void RegisterMethod(const DexFile& dex_file, int method_idx, int num_blocks)
      REQUIRES(!method_lock_);

  /*
   * @brief Populate and write the profiling data file at prof_file_name_.
   * @param update_checksum_if_needed 'true' if only the checksum needs to be updated.
   * @returns 'true' if the creation was successful.
   */
  bool WriteProfileFile(bool update_checksum_if_needed)
      REQUIRES(!method_lock_);

  /*
   * @brief Update the profiling data file after an execution.
   * @param profiles Map from OatFile to ExactProfileFile with new counts.
   * @param zero_counters 'true' if the counters need to be zero'ed after writing.
   */
  static void UpdateProfileFiles(SafeMap<const OatFile*, ExactProfileFile*>& profiles,
                                 bool zero_counters = false);

  /*
   * @brief Return the file name for the profiling data file.
   * @returns the file name for the profiling data file.
   */
  const std::string& GetProfileFileName() const { return prof_file_name_; }

  /*
   * @brief Return the file name for the profiling data file.
   * @returns pathname of the profile file
   * @note this may be relative to the dirname of the OAT file if not compiling a package.
   */
  std::string GetRelativeProfileFileName() const;

  static constexpr uint32_t Magic1_ = 0xB00DDADA;
  static constexpr uint32_t Magic2_ = 0xB00DDADB;
  static constexpr uint32_t Magic3_ = 0xB00DDADC;
  static constexpr uint32_t CurrentVersion_ = 0x00000001;
  static constexpr uint64_t MaxCount = std::numeric_limits<OneMethod::CountType>::max();

  /*
   * @brief Examine the OatFile and return the address of a matching profile
   * file.
   * @param oat_file The OAT file that may point to a profile file.
   * @param is_image 'true' if this is core.oat or boot.oat image.
   * @param zero_counts 'true' if the returned counts should be zeroed.
   * @returns An allocated buffer containing the profile file or nullptr if not available.
   */
  static ExactProfileFile* GetProfileFile(const OatFile& oat_file,
                                          bool is_image,
                                          bool zero_counts);

  /*
   * @brief Allocate Profiling counters for the given method in loaded OAT file.
   * @param method ArtMethod* pointer for this method.
   * @param hdr Matching Exact Profile file mapped into memory.
   * @param dex_file Internal version of the dex file.
   * @param method_index Index in dex file for this method.
   */
  static void AllocateProfileCounters(ArtMethod* method,
                                      ExactProfileFile* hdr,
                                      const DexFile* dex_file,
                                      uint32_t dex_file_index,
                                      uint32_t method_index)
      SHARED_REQUIRES(Locks::mutator_lock_) REQUIRES(Locks::profiler_lock_);

  /*
   * @brief Allocate Profiling counters for the given methods in boot.oat.
   * @param hdr Matching Exact Profile file mapped into memory.
   * @param dex_file_index The number of the matching dex file in the Profile file.
   * @param dex_file_location The name of the matching dex file.
   * @param dex_file Internal version of the dex file.
   * @param art_methods Array of ArtMethod* for boot.oat.
   * @param num_methods number of ArtMethod* in art_methods.
   */
  static void AllocateProfileCounters(ExactProfileFile* hdr,
                                      int32_t dex_file_index,
                                      const std::string& dex_file_location,
                                      const DexFile* dex_file,
                                      ArtMethod** art_methods,
                                      size_t num_methods)
      SHARED_REQUIRES(Locks::mutator_lock_) REQUIRES(Locks::profiler_lock_);

  /*
   * @brief Start background saver task.
   */
  static void StartProfileSaver();

  /*
   * @brief Start background saver task.
   */
  static void StopProfileSaver();

  /*
   * @brief Are there any existing profiles?
   * @return 'true' if there is at least one profile.
   */
  bool HasExistingProfiles() const { return existing_profiles_.size() != 0; }

  /*
   * @brief Look for an existing profile file and add it to the known profiles if present.
   * @param oat_location The OAT file that being compiled (or boot/core ART file).
   * @param is_image 'true' if this is core.oat or boot.oat image.
   * @param is_boot_image_option 'true' if this is oat_location is
   * really -Ximage:boot_image_art string.
   * @param isa Instruction set being compiled.
   * @returns An allocated buffer containing the profile file or nullptr if not available.
   * @note If the buffer is allocated, it is also added to exiting_profiles_.
   */
  ExactProfileFile* AddProfileFile(const std::string& oat_location,
                                   bool is_image,
                                   bool is_boot_image_option,
                                   InstructionSet isa = kNone);

  /*
   * @brief Find the matching OneMethod for a method being compiled.
   * @param graph HGraph for the method being compiled.
   * @returns the matching OneMethod, or nullptr if not found.
   */
  OneMethod* FindMethodCounts(HGraph* graph)
      REQUIRES(!method_lock_);

 private:
  mutable Mutex method_lock_ DEFAULT_MUTEX_ACQUIRED_AFTER;

  // Map from method index in dex file to #blocks used.
  typedef SafeMap<uint32_t, uint32_t> BlocksInMethod;

  // Each Dex file maintains an independent map of methods in the dex file.
  typedef SafeMap<const DexFile*, BlocksInMethod> Info;

  Info info_ GUARDED_BY(method_lock_);

  std::string oat_file_name_;
  std::string prof_file_name_;
  std::string prof_dir_name_;

  // Existing profiles to be used when optimizing.
  std::vector<ExactProfileFile*> existing_profiles_;

  // Faster lookup into existing_profiles_;
  typedef std::pair<ExactProfileFile*, const OneDexFile*> ProfileDexFile;
  typedef SafeMap<const DexFile*, ProfileDexFile> DexFileToOneDexFile;
  DexFileToOneDexFile profile_map_;

  static ExactProfileSaver *saver_ GUARDED_BY(Locks::profiler_lock_);

  // Return the file descriptor of the open file, or -1 on failure.
  static int OpenProfileFile(const OatFile& oat_file,
                             bool read_write,
                             bool is_image,
                             std::string& prof_name);
};

}  // namespace art

#endif  // COMPILER_OPTIMIZING_EXTENSIONS_INFRASTRUCTURE_EXT_PROFILING_H
