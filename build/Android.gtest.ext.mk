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

# Let us be careful and not re-add extension files multiple times.
# To do the check we pick the name of one of the tests below.
ifeq ($(findstring loop_hierarchy_test,$(COMPILER_GTEST_COMMON_SRC_FILES)),)

  VENDOR_EXTENSIONS_FOLDER := compiler/optimizing/extensions

  COMPILER_GTEST_EXTENSION_SRC_FILES := \
    $(VENDOR_EXTENSIONS_FOLDER)/passes/loop_hierarchy_test.cc \
    $(VENDOR_EXTENSIONS_FOLDER)/infrastructure/loops_iv_bounds_test.cc

  COMPILER_GTEST_COMMON_SRC_FILES += \
    $(COMPILER_GTEST_EXTENSION_SRC_FILES)

  # Although we added to the common files, add them to target and host as well because
  # the point where extensions makefile is included is after those get initialized
  # from the common list of files.
  TMP := $(COMPILER_GTEST_TARGET_SRC_FILES)
  COMPILER_GTEST_TARGET_SRC_FILES := \
    $(TMP) \
    $(COMPILER_GTEST_EXTENSION_SRC_FILES)

  TMP := $(COMPILER_GTEST_HOST_SRC_FILES)
  COMPILER_GTEST_HOST_SRC_FILES := \
    $(TMP) \
    $(COMPILER_GTEST_EXTENSION_SRC_FILES)

endif
