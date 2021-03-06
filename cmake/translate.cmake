#===============================================================================
# Copyright 2020 FUJITSU LIMITED
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#===============================================================================

# Manage different library options
#===============================================================================

if(translate_cmake_included)
    return()
endif()
set(translate_cmake_included true)

# =============================
# JIT translationi function
# =============================
if(NOT DEFINED CMAKE_SYSTEM_PROCESSOR OR CMAKE_SYSTEM_PROCESSOR STREQUAL "")
    message(FATAL_ERROR "CMAKE_SYSTEM_PROCESSOR is not defined. Perhaps CMake toolchain is broken")
else()
    message(STATUS "CMAKE_SYSTEM_PROCESSOR=${CMAKE_SYSTEM_PROCESSOR}")
endif()

if(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
    option(DNNL_INDIRECT_JIT_AARCH64
        "enables indirect JIT code from x86_64 to AArch64 architecture."
        ON) # disabled by default on AArch64 CPU
else()
    option(DNNL_INDIRECT_JIT_AARCH64
        "enables indirect JIT code from x86_64 to AArch64 architecture."
        OFF) # disabled by default on x86_64 CPU
endif()

message(STATUS "DNNL_INDIRECT_JIT_AARCH64=${DNNL_INDIRECT_JIT_AARCH64}")
