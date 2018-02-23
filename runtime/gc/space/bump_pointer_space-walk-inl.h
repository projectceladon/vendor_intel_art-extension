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

#ifndef ART_RUNTIME_GC_SPACE_BUMP_POINTER_SPACE_WALK_INL_H_
#define ART_RUNTIME_GC_SPACE_BUMP_POINTER_SPACE_WALK_INL_H_

#include "bump_pointer_space.h"
#include "base/bit_utils.h"
#include "mirror/object-inl.h"
#include "thread-current-inl.h"
#include "thread_list.h"
#include "scoped_thread_state_change.h"

namespace art {
namespace gc {
namespace space {

template <typename Visitor>
void BumpPointerSpace::WalkThreadUnsafe(std::vector<TlabPtrs>* active_tlabs, Visitor&& visitor) {
  uint8_t* pos = Begin();
  uint8_t* end = End();

  size_t tlab_pos = 0;

  // Internal indirection w/ NO_THREAD_SAFETY_ANALYSIS. Optimally, we'd like to have an annotation
  // like
  //   REQUIRES_AS(visitor.operator(mirror::Object*))
  // on Walk to expose the interprocedural nature of locks here without having to duplicate the
  // function.
  //
  // NO_THREAD_SAFETY_ANALYSIS is a workaround. The problem with the workaround of course is that
  // it doesn't complain at the callsite. However, that is strictly not worse than the
  // ObjectCallback version it replaces.
  auto no_thread_safety_analysis_visit = [&](mirror::Object* obj) NO_THREAD_SAFETY_ANALYSIS {
    visitor(obj);
  };

  while(pos < end) {
    mirror::Object* obj = reinterpret_cast<mirror::Object*>(pos);
    // No read barrier because obj may not be a valid object.
    if (obj->GetClass<kDefaultVerifyFlags, kWithoutReadBarrier>() == nullptr) {
      // We are in live TLAB or last object is not initialized yet.
      while (tlab_pos < active_tlabs->size() && pos >= (*active_tlabs)[tlab_pos].end_) {
        tlab_pos++;
      }

      if (tlab_pos < active_tlabs->size()) {
        if(pos >= (*active_tlabs)[tlab_pos].start_) {
          // We are in live TLAB. Move to end of TLAB.
          pos = (*active_tlabs)[tlab_pos].end_;
        } else {
          // There is a race condition where a thread has just allocated an object but not set the
          // class. Move to next TLAB.
          pos = (*active_tlabs)[tlab_pos].start_;
        }
      } else {
        // There is a race condition where a thread has just allocated an object but not set the
        // class. We can't know the size of this object, so we don't visit it and exit the function
        // since there is guaranteed to be not other blocks.
        return;
      }
    } else {
      no_thread_safety_analysis_visit(obj);
      pos = reinterpret_cast<uint8_t*>(GetNextObject(obj));
    }
  }
}

template <typename Visitor>
void BumpPointerSpace::Walk(Visitor&& visitor) NO_THREAD_SAFETY_ANALYSIS {
  std::vector<TlabPtrs> active_tlabs;
  // Collecting TLAB info requires stop the world.
  // That's not allowed when Walk is called by SS/GSS heap for verification.
  // Check the number of active tlabs
  if(tlabs_alive_.LoadSequentiallyConsistent() != 0) {
    Thread* self = Thread::Current();
    if (Locks::mutator_lock_->IsExclusiveHeld(self)) {
      // The mutators are already suspended.
      CollectActiveTlabsUnsafe(&active_tlabs);
    } else if (Locks::mutator_lock_->IsSharedHeld(self)) {
      // The mutators are not suspended yet and we have a shared access
      // to the mutator lock. Temporarily release the shared access by
      // transitioning to the suspend state, and suspend the mutators.
      ScopedThreadSuspension sts(self, kSuspended);
      CollectActiveTlabsWithSuspendAll(self, &active_tlabs);
    } else {
      // The mutators are not suspended yet. Suspend the mutators.
      CollectActiveTlabsWithSuspendAll(self, &active_tlabs);
    }
  }
  WalkThreadUnsafe(&active_tlabs, visitor);
}

}  // namespace space
}  // namespace gc
}  // namespace art

#endif  // ART_RUNTIME_GC_SPACE_BUMP_POINTER_SPACE_WALK_INL_H_
