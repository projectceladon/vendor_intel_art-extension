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
 *
 */

#include "bound_information.h"
#include "ext_utility.h"
#include "induction_variable.h"

namespace art {

void HLoopBoundInformation::Dump(std::ostream& os) {
  const char* prefix = HLoopInformation_X86::kLoopDumpPrefix;
  if (GetLoopBIV() == nullptr) {
    PRINT_OSTREAM_MESSAGE(os, prefix, "The loop does not have a basic IV");
  } else {
    GetLoopBIV()->Dump(os);
    if (IsSimpleCountUp()) {
      PRINT_OSTREAM_MESSAGE(os, prefix,
        "The loop is a simple count up loop:");
    } else if (is_simple_count_down_) {
      PRINT_OSTREAM_MESSAGE(os, prefix,
        "The loop is a simple count down loop");
    } else {
      PRINT_OSTREAM_MESSAGE(os, prefix,
        "The loop is neither a simple count up nor count down loop");
    }

    std::string cond;
    switch (GetComparisonCondition()) {
      case kCondLT:
        cond = "<";
        break;
      case kCondLE:
        cond = "<=";
        break;
      case kCondGE:
        cond = ">=";
        break;
      case kCondGT:
        cond = ">";
        break;
      default:
        DCHECK(false) << "Unexpected condition " << GetComparisonCondition();
        cond = "???";
    }

    if (IsFP()) {
      PRINT_OSTREAM_MESSAGE(os, prefix,
         "\tfor (i = " << GetFPBIVStartValue() << "; i " << cond
         << " " << GetFPBIVEndValue()  << "; i += "
         << GetLoopBIV()->GetFPIncrement() << ")");
    } else {
      PRINT_OSTREAM_MESSAGE(os, prefix,
         "\tfor (i = " << GetIntegralBIVStartValue() << "; i " << cond
         << " " << GetIntegralBIVEndValue()  << "; i += "
         << GetLoopBIV()->GetIncrement() << ")");
    }
  }

  if (num_iterations_ == -1) {
    PRINT_OSTREAM_MESSAGE(os, prefix, "The loop has unknown number of iterations");
  } else {
    DCHECK_GE(num_iterations_, 0);
    PRINT_OSTREAM_MESSAGE(os, prefix, "The loop has " << num_iterations_ << " iterations");
  }
}

bool HLoopBoundInformation::IsFP() const {
  auto biv = GetLoopBIV();
  return biv != nullptr && biv->IsFP();
}

bool HLoopBoundInformation::IsInteger() const {
  auto biv = GetLoopBIV();
  return biv != nullptr && biv->IsInteger();
}

bool HLoopBoundInformation::IsLong() const {
  auto biv = GetLoopBIV();
  return biv != nullptr && biv->IsLong();
}

bool HLoopBoundInformation::IsFloat() const {
  auto biv = GetLoopBIV();
  return biv != nullptr && biv->IsFloat();
}

bool HLoopBoundInformation::IsDouble() const {
  auto biv = GetLoopBIV();
  return biv != nullptr && biv->IsDouble();
}

bool HLoopBoundInformation::IsWide() const {
  auto biv = GetLoopBIV();
  return biv != nullptr && biv->IsWide();
}

int64_t HLoopBoundInformation::GetIntegralBIVStartValue() const {
  DCHECK(GetLoopBIV() != nullptr);
  DCHECK(!IsFP());
  return biv_start_value_.integral;
}

void HLoopBoundInformation::SetIntegralBIVStartValue(int64_t start_value) {
  DCHECK(GetLoopBIV() != nullptr);
  DCHECK(!IsFP());
  biv_start_value_.integral = start_value;
}

int64_t HLoopBoundInformation::GetIntegralBIVEndValue() const {
  DCHECK(GetLoopBIV() != nullptr);
  DCHECK(!IsFP());
  return biv_end_value_.integral;
}

void HLoopBoundInformation::SetIntegralBIVEndValue(int64_t end_value) {
  DCHECK(GetLoopBIV() != nullptr);
  DCHECK(!IsFP());
  biv_end_value_.integral = end_value;
}

double HLoopBoundInformation::GetFPBIVStartValue() const {
  DCHECK(GetLoopBIV() != nullptr);
  DCHECK(IsFP());
  return biv_start_value_.fp;
}

void HLoopBoundInformation::SetFPBIVStartValue(double start_value) {
  DCHECK(GetLoopBIV() != nullptr);
  DCHECK(IsFP());
  biv_start_value_.fp = start_value;
}

double HLoopBoundInformation::GetFPBIVEndValue() const {
  DCHECK(GetLoopBIV() != nullptr);
  DCHECK(IsFP());
  return biv_end_value_.fp;
}

void HLoopBoundInformation::SetFPBIVEndValue(double end_value) {
  DCHECK(GetLoopBIV() != nullptr);
  DCHECK(IsFP());
  biv_end_value_.fp = end_value;
}

}  // namespace art
