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

#ifndef _VKTF_LIB_H_
#define _VKTF_LIB_H_
#pragma once

class vkTFlib {
    enum class ResourceType {
        SHADER = 0,
        TEXTURE,
        NUM_RESOURCES,
    };
public:
    virtual ~vkTFlib();
    static vkTFlib* instance();

    std::string getShaderFileName(const char* fileName);
    std::string getTextureFileName(const char* fileName);
    int execCmd(std::string & cmd);
    void* glslRead(const char* fileName, size_t& size);
    std::vector<uint32_t> glslCompile(const char* fileName, size_t& size, int shader_type);
    std::stringstream glslCompile(const char* fileName, int shader_type);

protected:
    int initPaths();
    std::string getResourceFileName(const std::string& fileName, ResourceType resType);
    std::string getFileNameWoExt(const std::string& fileName);
    std::string getSpvFileName(const std::string& fileName);
    int glsl2spv(const std::string& glslFileName, const std::string& spvFileName);

private:
    vkTFlib();
    vkTFlib(vkTFlib const&) = delete;
    vkTFlib& operator=(vkTFlib const &) = delete;
    vkTFlib(vkTFlib &&) = delete;
    vkTFlib& operator=(vkTFlib &&) = delete;
    
private:
    bool m_enableSPVDump;
    std::vector<std::string> resPaths[static_cast<int>(ResourceType::NUM_RESOURCES)];
};

#endif//_VKTF_LIB_H_
