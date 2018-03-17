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

#ifndef _VKTF_INTERFACE_H_
#define _VKTF_INTERFACE_H_
#pragma once

#include <unordered_map>
#include <vector>

#include "vkPlaylib.h"

class vkTFInterface {
public:
    class TestArgs
    {
    public:
        std::unordered_map<std::string, std::string> libNames;
        std::vector<std::string> testNames;
        uint32_t height = 512;
        uint32_t width = 512;
        int verbose_flag = 0;
        int dump_flag = 0;
        int window_flag = 0;
        int start_frame = 0;
        int total_frame = 1;
        int validation = 0;
        int vsync = 0;

        bool fullscreen = false;
        bool overlay = false;
        
        class BenchmarkArgs {
        public:
            int active = false;
            uint32_t warmup = 0;
            uint32_t duration = 0;
            std::string filename;
            bool outputFrameTimes = false;
        };
        BenchmarkArgs benchmark;
    };
    TestArgs settings;
public:
    virtual int vkTFInit(void*) = 0;
    virtual int vkTFRender(int) = 0;
    virtual int vkTFExit(void) = 0;
};


#endif//_VKTF_INTERFACE_H_
