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
 */

#include <unordered_set>
#include <vector>

#include "ext_utility.h"
#include "graph_x86.h"
#include "loop_iterators.h"
#include "osr_graph_rebuilder.h"

namespace art {

void HOsrGraphRebuilder::InjectionPhiFixup(HLoopInformation_X86* loop,
                                           HBasicBlock* osr_path,
                                           HBasicBlock* new_pre_header) {
  DCHECK(loop != nullptr);
  HBasicBlock* header = loop->GetHeader();
  auto arena = graph_->GetArena();
  // Traverse over loop phis and substitute them with
  // new phis in preheader.
  for (HInstructionIterator phi_it(header->GetPhis());
       !phi_it.Done();
       phi_it.Advance()) {
    // Create a new phi merging OSR and non-osr inputs.
    HPhi* phi = phi_it.Current()->AsPhi();

    // Paranoid: is this a loop Phi from the only back edge?
    DCHECK_EQ(phi->InputCount(), 2u);
    DCHECK(loop->Contains(*phi->InputAt(1)->GetBlock()));

    bool degenerate = (phi->InputAt(1) == phi);

    PRINT_PASS_OSTREAM_MESSAGE(this, "Making OSR entry for"
                                      << (degenerate? " degenerate" : "")
                                      << " " << phi);
    HInstruction* input = phi->InputAt(0);

    HPhi* new_phi = new (arena) HPhi(arena,
                                     phi->GetRegNumber(),
                                     2u,
                                     HPhi::ToPhiType(phi->GetType()));

    // Create a fictive input in OSR path and set phi inputs.
    HOsrFictiveValue* osr_value = new (arena) HOsrFictiveValue(input->GetType());
    if (input->GetType() == Primitive::Type::kPrimNot) {
      osr_value->SetReferenceTypeInfo(input->GetReferenceTypeInfo());
      new_phi->SetReferenceTypeInfo(input->GetReferenceTypeInfo());
    }
    osr_path->InsertInstructionBefore(osr_value, osr_path->GetLastInstruction());
    new_phi->SetRawInputAt(0, input);
    new_phi->SetRawInputAt(1, osr_value);
    new_pre_header->AddPhi(new_phi);

    phi->ReplaceInput(new_phi, 0);

    if (degenerate) {
      PRINT_PASS_OSTREAM_MESSAGE(this, "Removing degenerate " << phi);
      phi->ReplaceWith(phi->InputAt(0));
      header->RemovePhi(phi);
    }
  }
}

void HOsrGraphRebuilder::InjectOsrIntoLoops(HBasicBlock* osr_path) {
  DCHECK(osr_path != nullptr);
  DCHECK(osr_path->GetLoopInformation() == nullptr);
  HGraph_X86* graph = GRAPH_TO_GRAPH_X86(graph_);

  for (auto loop : fixable_loops_) {
    HBasicBlock* header = loop->GetHeader();
    HBasicBlock* old_pre_header = loop->GetPreHeader();
    PRINT_PASS_OSTREAM_MESSAGE(this, "Inserting OSR for loop #"
                               << header->GetBlockId()
                               << " (pre-header #" << old_pre_header->GetBlockId()
                               << ")");

    HBasicBlock* new_pre_header = graph->CreateNewBasicBlock();
    PRINT_PASS_OSTREAM_MESSAGE(this, "Inserting new pre-header #"
                                     << new_pre_header->GetBlockId());
    new_pre_header->InsertBetween(old_pre_header, header);
    // Insert this to avoid appearance of critical edge.
    HBasicBlock* split = graph->CreateNewBasicBlock();
    split->AddInstruction(new (graph->GetArena()) HGoto());
    osr_path->AddSuccessor(split);
    split->AddSuccessor(new_pre_header);

    // Create OSR fixup Phis.
    InjectionPhiFixup(loop, osr_path, new_pre_header);

    // Now replace suspend check entry point with pre-header entry point.
    auto suspend_check = loop->GetSuspendCheck();
    suspend_check->RemoveOsrEntryPoint();

    HOsrEntryPoint* osr_entry = new (graph->GetArena())
                                HOsrEntryPoint(suspend_check->GetDexPc());
    new_pre_header->AddInstruction(osr_entry);
    osr_entry->CopyEnvironmentFromWithLoopPhiAdjustment(
               suspend_check->GetEnvironment(), header);
    new_pre_header->AddInstruction(new (graph->GetArena()) HGoto());
    new_pre_header->SetLoopInformation(old_pre_header->GetLoopInformation());

    // Finally, make the loop reducible.
    loop->MakeReducible();
  }
}

void HOsrGraphRebuilder::TransformCfg(HBasicBlock*& osr_fork,
                                      HBasicBlock*& normal_path,
                                      HBasicBlock*& osr_path) {
  HGraph_X86* graph = GRAPH_TO_GRAPH_X86(graph_);

  HBasicBlock* method_entry = graph->GetEntryBlock();
  osr_fork = graph->CreateNewBasicBlock();
  normal_path = graph->CreateNewBasicBlock();
  osr_path = graph->CreateNewBasicBlock();

  // Create OSR fork block.
  PRINT_PASS_OSTREAM_MESSAGE(this, "Inserting OSR fork block #" << osr_fork->GetBlockId());
  osr_fork->InsertBetween(method_entry, method_entry->GetSingleSuccessor());
  osr_fork->AddInstruction(new (graph->GetArena()) HOsrFork());

  PRINT_PASS_OSTREAM_MESSAGE(this, "Branching: normal path #" << normal_path->GetBlockId()
                                   << ", osr path #" << osr_path->GetBlockId());
  normal_path->AddInstruction(new (graph->GetArena()) HGoto());
  normal_path->InsertBetween(osr_fork, osr_fork->GetSingleSuccessor());
  osr_fork->AddSuccessor(osr_path);
  osr_path->AddInstruction(new (graph->GetArena()) HOsrJump());
}

void HOsrGraphRebuilder::MoveParams(HBasicBlock* entry_block,
                                    HBasicBlock* normal_path) {
  for (HBackwardInstructionIterator inst_it(entry_block->GetInstructions());
     !inst_it.Done();
     inst_it.Advance()) {
    HInstruction* insn = inst_it.Current();
    if (insn->IsParameterValue() || insn->IsSuspendCheck()) {
      PRINT_PASS_OSTREAM_MESSAGE(this, "Moving " << insn
                                        << " to block #"
                                        << normal_path->GetBlockId());
      insn->MoveBefore(normal_path->GetFirstInstruction());
    }
  }
}

void HOsrGraphRebuilder::DoOsrPreparation() {
  HGraph_X86* graph = GRAPH_TO_GRAPH_X86(graph_);

  HBasicBlock* osr_fork = nullptr;
  HBasicBlock* normal_path = nullptr;
  HBasicBlock* osr_path = nullptr;

  // First, transform CFG, creating auxiliary blocks.
  TransformCfg(osr_fork, normal_path, osr_path);

  // Total paranoia.
  HBasicBlock* method_entry = graph->GetEntryBlock();
  DCHECK(method_entry != nullptr);
  DCHECK(osr_fork != nullptr);
  DCHECK(normal_path != nullptr);
  DCHECK(osr_path != nullptr);

  // Move params and suspend check to normal path.
  MoveParams(method_entry, normal_path);

  // Inject jumps from OSR path to loops headers.
  InjectOsrIntoLoops(osr_path);

  // Since we changed CFG, we need to rebuild domination.
  graph->RebuildDomination();
}

bool HOsrGraphRebuilder::Gate() {
  HGraph_X86* graph = GRAPH_TO_GRAPH_X86(graph_);
  HLoopInformation_X86* loop_info = graph->GetLoopInformation();

  // Collect all fixable loops.
  for (HOnlyInnerLoopIterator loop_iter(loop_info);
       !loop_iter.Done(); loop_iter.Advance()) {
    HLoopInformation_X86* loop = loop_iter.Current();
    if (loop->IsIrreducible() &&
        loop->HasSuspendCheck() &&
        loop->NumberOfBackEdges() == 1 &&
        !loop->IsIrreducibleSlowCheck()) {
      DCHECK(loop->IsInner());
      fixable_loops_.push_back(loop);
    }
  }

  // Have we found at least one?
  return !fixable_loops_.empty();
}

void HOsrGraphRebuilder::Run() {
  if (!graph_->IsCompilingOsr()) {
    return;
  }

  PRINT_PASS_OSTREAM_MESSAGE(this, "Start " << GetMethodName(graph_));

  if (Gate()) {
    PRINT_PASS_OSTREAM_MESSAGE(this, "Preparing " << fixable_loops_.size() << " loops");
    DoOsrPreparation();
  } else {
    PRINT_PASS_OSTREAM_MESSAGE(this, "Nothing to prepare");
  }

  PRINT_PASS_OSTREAM_MESSAGE(this, "End " << GetMethodName(graph_));
}

}  // namespace art
