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

#include "loop_information.h"

namespace art {

/**
 * @details Add takes a new LoopInformation and determines if info is nested_ with
 *   this instance or not.
 * If it is nested_ in this instance, we fill our nested_ information with it
 * Otherwise, we are a sibling, this is due to how Add is being called,
 *  it supposes that info has less or equal number of BasicBlocks.
 */
void HLoopInformation_X86::Add(HLoopInformation_X86* other) {
  // Check if there is anything to do.
  if (other == nullptr) {
    return;
  }

  // Clear siblings and nested in case.
  other->sibling_next_ = nullptr;
  other->sibling_previous_ = nullptr;
  other->outer_ = nullptr;

  // Do we include the current loop?
  if (other->IsIn(*this) == true) {
    // We contain them, so they should not contain us.
    DCHECK(IsIn(*other) == false);

    // Search in the children if anybody includes them.
    if (inner_ == nullptr) {
      inner_ = other;
      inner_->outer_ = this;
    } else {
      // Continue down the link.
      inner_->Add(other);
    }
  } else {
    if (sibling_next_ == nullptr) {
      other->sibling_previous_ = this;
      this->sibling_next_ = other;
      other->outer_ = this->outer_;
    } else {
      sibling_next_->Add(other);
    }
  }
}

void HLoopInformation_X86::SetDepth(int depth) {
  HLoopInformation_X86* info = this;
  while (info != nullptr) {
    info->depth_ = depth;

    if (info->inner_ != nullptr) {
      info->inner_->SetDepth(depth + 1);
    }
    info = info->sibling_next_;
  }
}

void HLoopInformation_X86::Dump(int max_depth) const {
  char buffer[256];
  char* ptr = buffer;
  int i = 0;

  for (i = 0; i < depth_ && i < max_depth; i++) {
    *ptr = '\t';
    ptr++;
  }

  snprintf(ptr, sizeof(buffer) - i, "%p: Loop depth %d with header %d, # blocks %d, contains: (inner %p, sibling %p)",
      this, depth_, GetHeader()->GetBlockId(), NumberOfBlocks(), inner_, sibling_next_);
  LOG(INFO) << buffer;

  if (inner_ != nullptr) {
    inner_->Dump();
  }

  if (sibling_next_ != nullptr) {
    sibling_next_->Dump();
  }
}

bool HLoopInformation_X86::ExecutedPerIteration(HInstruction* candidate) const {
  if (candidate == nullptr) {
    return false;
  }

  HBasicBlock* bb = candidate->GetBlock();
  // First is the instruction in the loop?
  if (Contains(*bb)== false) {
    return false;
  }

  for (HBasicBlock* back_edge : GetBackEdges()) {
    if (bb->Dominates(back_edge) == false) {
      return false;
    }
  }

  return true;
}
}  // namespace art
