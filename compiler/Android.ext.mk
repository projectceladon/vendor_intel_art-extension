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
ifeq ($(findstring graph_x86,$(LIBART_COMPILER_SRC_FILES)),)

  VENDOR_EXTENSIONS_FOLDER := optimizing/extensions

  COMPILER_EXTENSION_SRC_FILES := \
    $(VENDOR_EXTENSIONS_FOLDER)/infrastructure/cloning.cc \
    $(VENDOR_EXTENSIONS_FOLDER)/infrastructure/ext_alias.cc \
    $(VENDOR_EXTENSIONS_FOLDER)/infrastructure/ext_utility.cc \
    $(VENDOR_EXTENSIONS_FOLDER)/infrastructure/graph_x86.cc \
    $(VENDOR_EXTENSIONS_FOLDER)/infrastructure/loop_information.cc \
    $(VENDOR_EXTENSIONS_FOLDER)/infrastructure/loop_unrolling.cc \
    $(VENDOR_EXTENSIONS_FOLDER)/infrastructure/pass_framework.cc \
    $(VENDOR_EXTENSIONS_FOLDER)/passes/aur.cc \
    $(VENDOR_EXTENSIONS_FOLDER)/passes/constant_calculation_sinking.cc \
    $(VENDOR_EXTENSIONS_FOLDER)/passes/find_ivs.cc \
    $(VENDOR_EXTENSIONS_FOLDER)/passes/form_bottom_loops.cc \
    $(VENDOR_EXTENSIONS_FOLDER)/passes/generate_selects.cc \
    $(VENDOR_EXTENSIONS_FOLDER)/passes/gvn_after_fbl.cc \
    $(VENDOR_EXTENSIONS_FOLDER)/passes/insert_profiling.cc \
    $(VENDOR_EXTENSIONS_FOLDER)/passes/loadhoist_storesink.cc \
    $(VENDOR_EXTENSIONS_FOLDER)/passes/loop_formation.cc \
    $(VENDOR_EXTENSIONS_FOLDER)/passes/loop_full_unrolling.cc \
    $(VENDOR_EXTENSIONS_FOLDER)/passes/non_temporal_move.cc \
    $(VENDOR_EXTENSIONS_FOLDER)/passes/peeling.cc \
    $(VENDOR_EXTENSIONS_FOLDER)/passes/phi_cleanup.cc \
    $(VENDOR_EXTENSIONS_FOLDER)/passes/pure_invokes_analysis.cc \
    $(VENDOR_EXTENSIONS_FOLDER)/passes/remove_unused_loops.cc \
    $(VENDOR_EXTENSIONS_FOLDER)/passes/remove_suspend.cc \
    $(VENDOR_EXTENSIONS_FOLDER)/passes/value_propagation_through_heap.cc \
    $(VENDOR_EXTENSIONS_FOLDER)/passes/trivial_loop_evaluator.cc

  COMPILER_EXTENSION_ENUM_OPERATOR_OUT_HEADER_FILES = \
    base/stringprintf.h \
    dex/pass_manager.h

  LIBART_COMPILER_SRC_FILES += \
    $(COMPILER_EXTENSION_SRC_FILES) \
    $(COMPILER_EXTENSION_ENUM_OPERATOR_OUT_HEADER_FILES)

endif
