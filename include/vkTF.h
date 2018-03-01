//=======================================================================
//
// Copyright 2011 Yan, All Rights Reserved
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//=======================================================================
/*
    Descriptions:

    Notes:

    Authors:
        2KYan, 2KYan@outlook.com

*/
//=======================================================================
//

#ifndef _VKTF_H_
#define _VKTF_H_

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <gli/gli.hpp>

#include <vulkan/vulkan.h>
#include "vulkanexamplebase.h"
#include "VulkanDevice.hpp"
#include "VulkanBuffer.hpp"

#define VERTEX_BUFFER_BIND_ID 0
#define ENABLE_VALIDATION false

#ifdef USE_VKTF_DLL
    #define VKTFEXPORT __declspec(dllimport)
#else
    #define VKTFEXPORT __declspec(dllexport)
#endif

#define PASTOR(x, y) x ## _ ## y
#define EXPORTTESTCLASS(BNAME, TNAME)\
extern "C" VKTFEXPORT vktfInterface* PASTOR(BNAME, TNAME)(void* ptr)\
{\
    return new BNAME::TNAME(ptr);\
}\

#endif//_VKTF_H_
