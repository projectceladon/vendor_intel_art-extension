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

# This file is mostly empty because art-extension gets built by doing mm in art with
# VENDOR_ART_PATH defined. By having this file empty here, it prevents the
# Android build system from skipping this folder and going straight into the
# other folders which have makefiles. If this occurs, the build will fail because
# multiple targets with same name will be added.
