#=======================================================================
#
#  Copyright 2011 Yan, All Rights Reserved
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# http://www.apache.org/licenses/LICENSE-2.0
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
#=======================================================================
#
#    Authors:
#        2KYan, 2KYan@outlook.com

cmake_minimum_required(VERSION 3.1)

add_definitions(-D"BLOCK=${VKTF_BLOCK}")
set(LIBNAME vktests_${VKTF_BLOCK})

add_library(${LIBNAME} SHARED ${TESTS})
target_link_libraries(${LIBNAME} ${LIBRARIES})