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

endif
