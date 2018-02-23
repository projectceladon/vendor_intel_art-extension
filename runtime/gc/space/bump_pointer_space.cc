/*
 * Copyright (C) 2013 The Android Open Source Project
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

#include "bump_pointer_space.h"
#include "bump_pointer_space-inl.h"
#include "mirror/object-inl.h"
#include "mirror/class-inl.h"
#include "thread_list.h"

namespace art {
namespace gc {
namespace space {

BumpPointerSpace* BumpPointerSpace::Create(const std::string& name, size_t capacity,
                                           uint8_t* requested_begin) {
  capacity = RoundUp(capacity, kPageSize);
  std::string error_msg;
  std::unique_ptr<MemMap> mem_map(MemMap::MapAnonymous(name.c_str(), requested_begin, capacity,
                                                       PROT_READ | PROT_WRITE, true, false,
                                                       &error_msg));
  if (mem_map.get() == nullptr) {
    LOG(ERROR) << "Failed to allocate pages for alloc space (" << name << ") of size "
        << PrettySize(capacity) << " with message " << error_msg;
    return nullptr;
  }
  return new BumpPointerSpace(name, mem_map.release());
}

BumpPointerSpace* BumpPointerSpace::CreateFromMemMap(const std::string& name, MemMap* mem_map) {
  return new BumpPointerSpace(name, mem_map);
}

BumpPointerSpace::BumpPointerSpace(const std::string& name, uint8_t* begin, uint8_t* limit)
    : ContinuousMemMapAllocSpace(name, nullptr, begin, begin, limit,
                                 kGcRetentionPolicyAlwaysCollect),
      growth_end_(limit),
      objects_allocated_(0),
      bytes_allocated_(0),
      tlabs_alive_(0) {
}

BumpPointerSpace::BumpPointerSpace(const std::string& name, MemMap* mem_map)
    : ContinuousMemMapAllocSpace(name, mem_map, mem_map->Begin(), mem_map->Begin(), mem_map->End(),
                                 kGcRetentionPolicyAlwaysCollect),
      growth_end_(mem_map->End()),
      objects_allocated_(0),
      bytes_allocated_(0),
      tlabs_alive_(0) {
  aging_table_.reset(accounting::AgingTable::Create(
        android::base::StringPrintf("bump pointer space %s aging-table", name.c_str()),
        NonGrowthLimitCapacity(), Begin(), mem_map->End()));
  CHECK(aging_table_.get() != nullptr) << "could not create bump pointer space aging table";
}

void BumpPointerSpace::Clear() {
  // Release the pages back to the operating system.
  if (!kMadviseZeroes) {
    memset(Begin(), 0, Limit() - Begin());
  }
  CHECK_NE(madvise(Begin(), Limit() - Begin(), MADV_DONTNEED), -1) << "madvise failed";
  // Reset the end of the space back to the beginning, we move the end forward as we allocate
  // objects.
  SetEnd(Begin());
  objects_allocated_.StoreRelaxed(0);
  bytes_allocated_.StoreRelaxed(0);
  growth_end_ = Limit();
}

void BumpPointerSpace::Dump(std::ostream& os) const {
  os << GetName() << " "
      << reinterpret_cast<void*>(Begin()) << "-" << reinterpret_cast<void*>(End()) << " - "
      << reinterpret_cast<void*>(Limit());
}

mirror::Object* BumpPointerSpace::GetNextObject(mirror::Object* obj) {
  const uintptr_t position = reinterpret_cast<uintptr_t>(obj) + obj->SizeOf();
  return reinterpret_cast<mirror::Object*>(RoundUp(position, kAlignment));
}

size_t BumpPointerSpace::RevokeThreadLocalBuffers(Thread* thread) {
  AssertTlabOperationSafety(thread);
  if (HasAddress(reinterpret_cast<mirror::Object*>(thread->GetTlabStart()))) {
    objects_allocated_.FetchAndAddSequentiallyConsistent(thread->GetThreadLocalObjectsAllocated());
    bytes_allocated_.FetchAndAddSequentiallyConsistent(thread->GetThreadLocalBytesAllocated());
    tlabs_alive_.FetchAndSubSequentiallyConsistent(1);

    size_t remains = thread->TlabSize();
    if (remains > 0u) {
      FillWithDummyObject(reinterpret_cast<mirror::Object*>(thread->GetTlabPos()), remains);
    }
    thread->SetTlab(nullptr, nullptr, nullptr);
  }
  return 0U;
}

size_t BumpPointerSpace::RevokeAllThreadLocalBuffers() {
  Thread* self = Thread::Current();
  MutexLock mu(self, *Locks::runtime_shutdown_lock_);
  MutexLock mu2(self, *Locks::thread_list_lock_);
  std::list<Thread*> thread_list = Runtime::Current()->GetThreadList()->GetList();
  for (Thread* thread : thread_list) {
    RevokeThreadLocalBuffers(thread);
  }
  return 0U;
}

void BumpPointerSpace::AssertThreadLocalBuffersAreRevoked(Thread* thread) {
  if (kIsDebugBuild) {
    DCHECK(!thread->HasTlab() ||
           !HasAddress(reinterpret_cast<mirror::Object*>(thread->GetTlabStart())));
  }
}

void BumpPointerSpace::AssertAllThreadLocalBuffersAreRevoked() {
  if (kIsDebugBuild) {
    Thread* self = Thread::Current();
    MutexLock mu(self, *Locks::runtime_shutdown_lock_);
    MutexLock mu2(self, *Locks::thread_list_lock_);
    std::list<Thread*> thread_list = Runtime::Current()->GetThreadList()->GetList();
    for (Thread* thread : thread_list) {
      AssertThreadLocalBuffersAreRevoked(thread);
    }
  }
}

accounting::ContinuousSpaceBitmap::SweepCallback* BumpPointerSpace::GetSweepCallback() {
  UNIMPLEMENTED(FATAL);
  UNREACHABLE();
}

void BumpPointerSpace::AssertTlabOperationSafety(Thread* thread) {
  if (kIsDebugBuild) {
    Thread* self = Thread::Current();
    if(self != thread) {
      uint64_t owner = Locks::mutator_lock_->GetExclusiveOwnerTid();
      uint64_t threadId = static_cast<uint64_t>(thread->GetTid());
      DCHECK(owner != threadId);
      DCHECK(owner != 0ULL);
    }
  }
}

uint64_t BumpPointerSpace::GetBytesAllocated() {
  // Start out pre-determined amount (blocks which are not being allocated into).
  uint64_t total = static_cast<uint64_t>(bytes_allocated_.LoadRelaxed());
  Thread* self = Thread::Current();
  MutexLock mu(self, *Locks::runtime_shutdown_lock_);
  MutexLock mu2(self, *Locks::thread_list_lock_);
  std::list<Thread*> thread_list = Runtime::Current()->GetThreadList()->GetList();
  for (Thread* thread : thread_list) {
    if (HasAddress(reinterpret_cast<mirror::Object*>(thread->GetTlabStart()))) {
      total += thread->GetThreadLocalBytesAllocated();
    }
  }
  return total;
}

uint64_t BumpPointerSpace::GetObjectsAllocated() {
  // Start out pre-determined amount (blocks which are not being allocated into).
  uint64_t total = static_cast<uint64_t>(objects_allocated_.LoadRelaxed());
  Thread* self = Thread::Current();
  MutexLock mu(self, *Locks::runtime_shutdown_lock_);
  MutexLock mu2(self, *Locks::thread_list_lock_);
  std::list<Thread*> thread_list = Runtime::Current()->GetThreadList()->GetList();
  for (Thread* thread : thread_list) {
    if (HasAddress(reinterpret_cast<mirror::Object*>(thread->GetTlabStart()))) {
      total += thread->GetThreadLocalObjectsAllocated();
    }
  }
  return total;
}

bool BumpPointerSpace::AllocNewTlab(Thread* self, size_t bytes) {
  DCHECK(Thread::Current() == self) << "Cannot allocate TLAB from another thread";
  DCHECK(HasAddress(reinterpret_cast<mirror::Object*>(self->GetTlabStart())) ||
         self->GetTlabStart() == nullptr);
  tlabs_alive_.FetchAndAddSequentiallyConsistent(1);
  RevokeThreadLocalBuffers(self);
  uint8_t* start = reinterpret_cast<uint8_t*>(AllocNonvirtualWithoutAccounting(bytes));
  if (start == nullptr) {
    return false;
  }
  self->SetTlab(start, start + bytes, start + bytes);
  return true;
}

void BumpPointerSpace::LogFragmentationAllocFailure(std::ostream& os,
                                                    size_t /* failed_alloc_bytes */) {
  size_t max_contiguous_allocation = Limit() - End();
  os << "; failed due to fragmentation (largest possible contiguous allocation "
     <<  max_contiguous_allocation << " bytes)";
  // Caller's job to print failed_alloc_bytes.
}

size_t BumpPointerSpace::AllocationSizeNonvirtual(mirror::Object* obj, size_t* usable_size) {
  size_t num_bytes = obj->SizeOf();
  if (usable_size != nullptr) {
    *usable_size = RoundUp(num_bytes, kAlignment);
  }
  return num_bytes;
}

void BumpPointerSpace::CollectActiveTlabsWithSuspendAll(Thread* self,
                                                        std::vector<TlabPtrs>* active_tlabs) {
  ScopedSuspendAll ssa(__FUNCTION__);
  MutexLock mu(self, *Locks::runtime_shutdown_lock_);
  MutexLock mu2(self, *Locks::thread_list_lock_);
  CollectActiveTlabsUnsafe(active_tlabs);
}

void BumpPointerSpace::CollectActiveTlabsUnsafe(std::vector<TlabPtrs>* active_tlabs) {
  for (Thread* thread : Runtime::Current()->GetThreadList()->GetList()) {
    if (HasAddress(reinterpret_cast<mirror::Object*>(thread->GetTlabStart()))) {
      active_tlabs->emplace_back(thread->GetTlabStart(), thread->GetTlabEnd());
    }
  }
  if (active_tlabs->size() > 0) {
    std::sort(active_tlabs->begin(), active_tlabs->end(), TlabPtrsSorter);
  }
}


}  // namespace space
}  // namespace gc
}  // namespace art
