// Copyright (c) 2018 Google LLC.
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

#include <string>

#include <gmock/gmock.h>

#include "../pass_fixture.h"
#include "opt/licm_pass.h"

namespace {

using namespace spvtools;
using ::testing::UnorderedElementsAre;

using PassClassTest = PassTest<::testing::Test>;

/*
  Tests that the LICM pass will generate a preheader when one is not present

  Generated from the following GLSL fragment shader
--eliminate-local-multi-store has also been run on the spv binary
#version 440 core
void main(){
  int a = 1;
  int b = 2;
  int hoist = 0;
  for (int i = 0; i < 10; i++) {
    if (i == 5) {
      break;
    }
  }
  for (int i = 0; i < 10; i++) {
    hoist = a + b;
  }
}
*/
TEST_F(PassClassTest, HoistWithoutPreheader) {
  const std::string before_hoist = R"(OpCapability Shader
%1 = OpExtInstImport "GLSL.std.450"
OpMemoryModel Logical GLSL450
OpEntryPoint Fragment %main "main"
OpExecutionMode %main OriginUpperLeft
OpSource GLSL 440
OpName %main "main"
%void = OpTypeVoid
%4 = OpTypeFunction %void
%int = OpTypeInt 32 1
%_ptr_Function_int = OpTypePointer Function %int
%int_1 = OpConstant %int 1
%int_2 = OpConstant %int 2
%int_0 = OpConstant %int 0
%int_10 = OpConstant %int 10
%bool = OpTypeBool
%int_5 = OpConstant %int 5
%main = OpFunction %void None %4
%13 = OpLabel
OpBranch %14
%14 = OpLabel
%15 = OpPhi %int %int_0 %13 %16 %17
OpLoopMerge %25 %17 None
OpBranch %19
%19 = OpLabel
%20 = OpSLessThan %bool %15 %int_10
OpBranchConditional %20 %21 %25
%21 = OpLabel
%22 = OpIEqual %bool %15 %int_5
OpSelectionMerge %23 None
OpBranchConditional %22 %24 %23
%24 = OpLabel
OpBranch %25
%23 = OpLabel
OpBranch %17
%17 = OpLabel
%16 = OpIAdd %int %15 %int_1
OpBranch %14
%25 = OpLabel
%26 = OpPhi %int %int_0 %24 %int_0 %19 %27 %28
%29 = OpPhi %int %int_0 %24 %int_0 %19 %30 %28
OpLoopMerge %31 %28 None
OpBranch %32
%32 = OpLabel
%33 = OpSLessThan %bool %29 %int_10
OpBranchConditional %33 %34 %31
%34 = OpLabel
%27 = OpIAdd %int %int_1 %int_2
OpBranch %28
%28 = OpLabel
%30 = OpIAdd %int %29 %int_1
OpBranch %25
%31 = OpLabel
OpReturn
OpFunctionEnd
)";

  const std::string after_hoist = R"(OpCapability Shader
%1 = OpExtInstImport "GLSL.std.450"
OpMemoryModel Logical GLSL450
OpEntryPoint Fragment %main "main"
OpExecutionMode %main OriginUpperLeft
OpSource GLSL 440
OpName %main "main"
%void = OpTypeVoid
%4 = OpTypeFunction %void
%int = OpTypeInt 32 1
%_ptr_Function_int = OpTypePointer Function %int
%int_1 = OpConstant %int 1
%int_2 = OpConstant %int 2
%int_0 = OpConstant %int 0
%int_10 = OpConstant %int 10
%bool = OpTypeBool
%int_5 = OpConstant %int 5
%main = OpFunction %void None %4
%13 = OpLabel
OpBranch %14
%14 = OpLabel
%15 = OpPhi %int %int_0 %13 %16 %17
OpLoopMerge %18 %17 None
OpBranch %19
%19 = OpLabel
%20 = OpSLessThan %bool %15 %int_10
OpBranchConditional %20 %21 %34
%21 = OpLabel
%22 = OpIEqual %bool %15 %int_5
OpSelectionMerge %23 None
OpBranchConditional %22 %24 %23
%24 = OpLabel
OpBranch %34
%23 = OpLabel
OpBranch %17
%17 = OpLabel
%16 = OpIAdd %int %15 %int_1
OpBranch %14
%34 = OpLabel
%35 = OpPhi %int %int_0 %24 %int_0 %19
%36 = OpPhi %int %int_0 %24 %int_0 %19
%26 = OpIAdd %int %int_1 %int_2
OpBranch %18
%18 = OpLabel
%25 = OpPhi %int %26 %27 %35 %34
%28 = OpPhi %int %29 %27 %36 %34
OpLoopMerge %30 %27 None
OpBranch %31
%31 = OpLabel
%32 = OpSLessThan %bool %28 %int_10
OpBranchConditional %32 %33 %30
%33 = OpLabel
OpBranch %27
%27 = OpLabel
%29 = OpIAdd %int %28 %int_1
OpBranch %18
%30 = OpLabel
OpReturn
OpFunctionEnd
)";

  SinglePassRunAndCheck<opt::LICMPass>(before_hoist, after_hoist, true);
}

}  // namespace
