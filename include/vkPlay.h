/*=======================================================================
This file is part of vkPlay

vkPlay is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

vkPlay is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/
//=======================================================================
/*
Descriptions:

Notes:

Authors:
2KYan, 2KYan@outlook.com

*/
//=======================================================================
//

#ifndef _VKPLAY_H_
#define _VKPLAY_H_

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <gli/gli.hpp>

#include <vulkan/vulkan.h>
#include "vkTFInterface.h"
#include "vulkanexamplebase.h"
#include "VulkanDevice.hpp"
#include "VulkanBuffer.hpp"

#define VERTEX_BUFFER_BIND_ID 0
// Set to "true" to enable Vulkan's validation layers (see vulkandebug.cpp for details)
#define ENABLE_VALIDATION false
// Set to "true" to use staging buffers for uploading vertex and index data to device local memory
// See "prepareVertices" for details on what's staging and on why to use it
#define USE_STAGING true

#ifdef USE_VKPLAY_DLL
    #define VKPLAYEXPORT __declspec(dllimport)
#else
    #define VKPLAYEXPORT __declspec(dllexport)
#endif

#define PASTOR(x, y) x ## _ ## y
#define EXPORTTESTCLASS(BNAME, TNAME)\
extern "C" VKPLAYEXPORT vkTFInterface* PASTOR(BNAME, TNAME)(void* ptr)\
{\
    return new BNAME::TNAME(ptr);\
}\

#endif//_VKPLAY_H_
