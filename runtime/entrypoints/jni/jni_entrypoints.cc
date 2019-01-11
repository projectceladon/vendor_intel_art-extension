/*
 * Copyright (C) 2012 The Android Open Source Project
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

#include <android-base/logging.h>

#include "art_method-inl.h"
#include "entrypoints/entrypoint_utils.h"
#include "java_vm_ext.h"
#include "mirror/object-inl.h"
#include "scoped_thread_state_change-inl.h"
#include "thread.h"

namespace art {

// Used by the JNI dlsym stub to find the native method to invoke if none is registered.
#if defined(__arm__) || defined(__aarch64__)
extern "C" void* artFindNativeMethod() NO_THREAD_SAFETY_ANALYSIS {
  Thread* self = Thread::Current();
#else
extern "C" const void* artFindNativeMethod(Thread* self) NO_THREAD_SAFETY_ANALYSIS {
  DCHECK_EQ(self, Thread::Current());
#endif
  bool was_slow = false;
  bool is_fast = false;
  const void* return_val = nullptr;
  {
    Locks::mutator_lock_->AssertNotHeld(self);  // We come here as Native.
    ScopedObjectAccess soa(self);

    ArtMethod* method = self->GetCurrentMethod(nullptr);
    DCHECK(method != nullptr);

    // Lookup symbol address for method, on failure we'll return null with an exception set,
    // otherwise we return the address of the method we found.
    void* native_code = soa.Vm()->FindCodeForNativeMethod(method);
    if (native_code == nullptr) {
      self->AssertPendingException();
      return nullptr;
    } else {
      // Register so that future calls don't come here
      was_slow = !method->IsFastNative();
      const void* final_function_ptr = method->RegisterNative(native_code);
      is_fast = method->IsFastNative();
      return_val = final_function_ptr;
    }
  }
  if (was_slow && is_fast) {
    self->TransitionFromSuspendedToRunnable();
  }
  return return_val;
}

}  // namespace art
