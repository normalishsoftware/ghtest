// Copyright (c) 2017 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "feature_manager.h"
#include <queue>
#include <stack>

#include "enum_string_mapping.h"

namespace spvtools {
namespace opt {

void FeatureManager::Analyze(ir::Module* module) {
  AddExtensions(module);
  AddCapabilities(module);
}

void FeatureManager::AddExtensions(ir::Module* module) {
  for (auto ext : module->extensions()) {
    const std::string name =
        reinterpret_cast<const char*>(ext.GetInOperand(0u).words.data());
    libspirv::Extension extension;
    if (libspirv::GetExtensionFromString(name, &extension)) {
      extensions_.Add(extension);
    }
  }
}

void FeatureManager::AddCapability(SpvCapability cap) {
  if (capabilities_.Contains(cap)) return;

  capabilities_.Add(cap);

  spv_operand_desc desc = {};
  if (SPV_SUCCESS ==
      grammar_.lookupOperand(SPV_OPERAND_TYPE_CAPABILITY, cap, &desc)) {
    libspirv::CapabilitySet(desc->numCapabilities, desc->capabilities)
        .ForEach([this](SpvCapability c) { AddCapability(c); });
  }
}

void FeatureManager::AddCapabilities(ir::Module* module) {
  for (ir::Instruction& inst : module->capabilities()) {
    AddCapability(static_cast<SpvCapability>(inst.GetSingleWordInOperand(0)));
  }
}

}  // namespace opt
}  // namespace spvtools
