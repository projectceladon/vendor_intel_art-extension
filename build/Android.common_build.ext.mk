# INTEL CONFIDENTIAL
# Copyright (c) 2015, Intel Corporation All Rights Reserved.
#
# The source code contained or described herein and all documents related to the
# source code ("Material") are owned by Intel Corporation or its suppliers or
# licensors. Title to the Material remains with Intel Corporation or its suppliers
# and licensors. The Material contains trade secrets and proprietary and
# confidential information of Intel or its suppliers and licensors. The Material
# is protected by worldwide copyright and trade secret laws and treaty provisions.
# No part of the Material may be used, copied, reproduced, modified, published,
# uploaded, posted, transmitted, distributed, or disclosed in any way without
# Intel's prior express written permission.
#
# No license under any patent, copyright, trade secret or other intellectual
# property right is granted to or conferred upon you by disclosure or delivery of
# the Materials, either expressly, by implication, inducement, estoppel or
# otherwise. Any license under such intellectual property rights must be express
# and approved by Intel in writing.

ifndef ART_ANDROID_COMMON_BUILD_VENDOR_MK
ART_ANDROID_COMMON_BUILD_VENDOR_MK = true

VENDOR_COMPILER_INCLUDES := \
  $(VENDOR_ART_PATH)/compiler \
  $(VENDOR_ART_PATH)/compiler/optimizing \
  $(VENDOR_ART_PATH)/compiler/optimizing/extensions \
  $(VENDOR_ART_PATH)/compiler/optimizing/extensions/infrastructure \
  $(VENDOR_ART_PATH)/compiler/optimizing/extensions/passes

# It is a bit of a sin to polute general includes with compiler includes
# but ART uses ART_C_INCLUDES everywhere. For sake of reducing conflicts,
# simply add all vendor compiler includes in the general includes.
ART_C_INCLUDES += $(VENDOR_COMPILER_INCLUDES)

TIP_ID = $(shell cd vendor/intel/art-extension && git log | grep '^\s\+Change-Id:' \
  | head -1 | sed -r 's/^\s+Change-Id:\s*//' | head -c 6)

# To understand what weekly build is base for current build, we get all
# unique weekly build ids and number of their entries which corresponds to
# actual number of patches from weekly.
WWs = $(shell cd vendor/intel/art-extension && git log | grep 'Extension-Weekly:' \
  | sed -r 's/^\s+Extension-Weekly:\s*//' | sort -r | uniq -c | sed -r 's/^\s*//' \
  | sed -r 's/\s/-/')

# Here we compare the actual number of patches from weekly builds current
# build contains with total number of patches in weekly. If these
# numbers are equal we know which weekly build is the base for current build.
WW=$(shell for val in ${WWs}; \
  do real_ww_patches="`sed -r 's/^([[:digit:]]+).*/\1/' <<< $$val`"; \
  supposed_ww_patches="`sed -r 's/[^.]*\.([[:digit:]]+)/\1/' <<< $$val`"; \
  get_week="`sed -r 's/[^-]*-([[:digit:]]+[a-z]*)\..*/\1/' <<< $$val`"; \
  if [[ $$real_ww_patches = $$supposed_ww_patches ]]; then echo $$get_week; break; fi done)

# Handle unexpected case.
ifeq (${WW},)
ART_EXTENSION_VERSION = UNKNOWN.${TIP_ID}
else
# Calculate number of patches were added to weekly build with number WW.
NUMBER_OF_PATCHES_ABOVE_WEEKLY = $(shell cd vendor/intel/art-extension && git log \
  | sed -r '/^\s+Extension-Weekly:\s*${WW}/q' | grep '^\s\+Change-Id:' | head -n -1 | wc -l)

ART_EXTENSION_VERSION = \
  ${WW}.${NUMBER_OF_PATCHES_ABOVE_WEEKLY}.${TIP_ID}
endif

ART_HOST_CFLAGS += -DART_EXTENSION_VERSION=$(ART_EXTENSION_VERSION)
ART_TARGET_CFLAGS += -DART_EXTENSION_VERSION=$(ART_EXTENSION_VERSION)

endif
