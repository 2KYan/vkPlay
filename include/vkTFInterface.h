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

#ifndef _VKTF_INTERFACE_H_
#define _VKTF_INTERFACE_H_
#pragma once

#include <unordered_map>
#include <vector>

class vkTFInterface {
public:
	struct TestArgs
	{
		std::unordered_map<std::string, std::string> libNames;
		std::vector<std::string> testNames;
		uint32_t height = 512;
		uint32_t width  = 512;
		int verbose_flag = 0;
		int dump_flag = 0;
		int window_flag = 0;
		int start_frame = 0;
		int total_frame = 1;
	};
public:
	virtual int vkTFInit(void*) = 0;
	virtual int vkTFRender(int) = 0;
	virtual int vkTFExit(void) = 0;
};


#endif//_VKTF_INTERFACE_H_
