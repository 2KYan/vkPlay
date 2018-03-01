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
#if defined(WIN32)
#include <windows.h>
#elif defined (LINUX)
#include <sys/wait.h>
#include <unistd.h>
#endif

#include <sys/stat.h>
#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <mutex>

#include "vkTFlib.h"
#include "shaderc/shaderc.hpp"

vkTFlib* vkTFlib::instance()
{
    std::mutex inst_m;
    std::lock_guard<std::mutex> l(inst_m);

    static std::unique_ptr<vkTFlib> thisPtr(new vkTFlib);
    static bool isInit = false;
    if (isInit == false) {
        isInit = true;
        thisPtr->initPaths();
    }

    return thisPtr.get();
}

vkTFlib::vkTFlib() 
{
    m_enableSPVDump = false;
}

vkTFlib::~vkTFlib()
{
}

int vkTFlib::initPaths()
{
char* envPath = nullptr;
std::vector<std::string> paths = { "./" };
std::vector<std::string> resourceTypeStrings = { "shader/", "texture/" };

std::unordered_map<std::string, std::string> envPaths = {
    { "DEV_HOME", "/data/"},
};

for (const auto& envVar : envPaths) {
    if ((envPath = getenv(envVar.first.c_str())) != nullptr) {
        paths.push_back(std::string(envPath) + envVar.second);
    }
}

std::string resPath;
struct stat buffer;
for (const auto& path : paths) {
    for (int i = 0; i < static_cast<int>(ResourceType::NUM_RESOURCES); ++i) {
        resPath = path;
        if (stat(resPath.c_str(), &buffer) == 0) {
            resPaths[i].push_back(resPath);
        }
        resPath = path + resourceTypeStrings[i];
        if (stat(resPath.c_str(), &buffer) == 0) {
            resPaths[i].push_back(resPath);
        }
    }
}

return 0;
}

std::string vkTFlib::getShaderFileName(const char* fileName) {
    return getResourceFileName(fileName, ResourceType::SHADER);
}

std::string vkTFlib::getTextureFileName(const char* fileName) {
    return getResourceFileName(fileName, ResourceType::TEXTURE);
}

std::string vkTFlib::getResourceFileName(const std::string& fileName, ResourceType resType)
{
    struct stat buffer;
    for (const auto& resPath : resPaths[static_cast<int>(resType)]) {
        if (stat((resPath + fileName).c_str(), &buffer) == 0) {
            return resPath + fileName;
        }
    }

    return std::string();
}

std::string vkTFlib::getFileNameWoExt(const std::string& fileName)
{
    std::string fileNameStr(fileName);
    std::string sep("\\");
    size_t offset = 0;
    while ((offset = fileNameStr.find(sep, offset)) != std::string::npos) {
        fileNameStr.replace(offset, sep.length(), "/");
    }
    if ((offset = fileNameStr.rfind("/", fileNameStr.length())) != std::string::npos) {
        fileNameStr.erase(0, offset + 1);
    }

    return fileNameStr;
}

std::string vkTFlib::getSpvFileName(const std::string& fileName)
{
    std::string fileNameStr(getFileNameWoExt(fileName));
    auto offset = 0;
    while ((offset = fileNameStr.find('.', offset)) != std::string::npos) {
        fileNameStr.replace(offset, 1, 1, '-');
    }
    return fileNameStr + ".spv";
}

std::stringstream vkTFlib::glslCompile(const char* fileName, int shader_type)
{
    std::stringstream stream;
    size_t size = 0;
    std::vector<uint32_t> spvBinary = glslCompile(fileName, size, shader_type);

    if (spvBinary.size() != 0) {
        stream.rdbuf()->sputn((char*)spvBinary.data(), size);
    }

    return stream;
}

std::vector<uint32_t> vkTFlib::glslCompile(const char* fileName, size_t& size, int shader_type)
{
    std::vector<uint32_t> spvBinary;

    shaderc::Compiler compiler;
    shaderc::CompileOptions options;

    std::string fName;

    if ((fName = getShaderFileName(fileName)).empty() == false) {
        std::ifstream t(fName);
        std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

        //options.AddMacroDefinition("MY_DEFINE", "1");
        shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(str, static_cast<shaderc_shader_kind>(shader_type), "shader_src", options);

        if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
            std::string msg = module.GetErrorMessage();
        } else {
            spvBinary.assign(module.cbegin(), module.cend()); 
            size = (module.cend() - module.cbegin()) * sizeof(uint32_t);

            static int count = 0;
            if (m_enableSPVDump) {
                std::string fileName("shader");
                fileName += std::to_string(count++) + ".spv";
                FILE* fptr = fopen(fileName.c_str(), "wb");
                fwrite(spvBinary.data(), size, 1, fptr);
                fclose(fptr);
            }
        }
    }

    return spvBinary;

}

void* vkTFlib::glslRead(const char* fileName, size_t& size) 
{
    std::string glslFileName;
    std::string outSpvFileName;

    if ((glslFileName = getShaderFileName(fileName)).empty() == false) {
        outSpvFileName = getSpvFileName(glslFileName);
        if (glsl2spv(glslFileName, outSpvFileName) == 0) {
            FILE* fp = fopen(outSpvFileName.c_str(), "rb");
            if (!fp) {
                return nullptr;
            }

            fseek(fp, 0L, SEEK_END);
            size = ftell(fp);

            fseek(fp, 0L, SEEK_SET);

            void* shader_code = malloc(size);
            size_t retval = fread(shader_code, size, 1, fp);
            fclose(fp);
            
            return shader_code;
        }
    }
    return nullptr;
}

int vkTFlib::glsl2spv(const std::string& glslFileName, const std::string& outSpvFileName)
{
    std::string cmdLine = std::string("glslangValidator -s -V -o") + outSpvFileName + " " + glslFileName;
    return execCmd(cmdLine);
}

int vkTFlib::execCmd(std::string& cmd) 
{
    int rtnVal = 0;
#if defined(WIN32)
    PROCESS_INFORMATION p_info;
    STARTUPINFO s_info;

    memset(&p_info, 0, sizeof(p_info));
    memset(&s_info, 0, sizeof(s_info));
    s_info.cb = sizeof(s_info);

    char* cmdline = _strdup(cmd.c_str());
    if (CreateProcess(NULL, cmdline, NULL, NULL, 0, CREATE_NO_WINDOW, NULL, NULL, &s_info, &p_info)) {
        WaitForSingleObject(p_info.hProcess, INFINITE);
        CloseHandle(p_info.hProcess);
        CloseHandle(p_info.hThread);
    } else {
        rtnVal = -1;
    }
    free(cmdline);
#elif defined(LINUX)
    pid_t pid = fork();
    int status;
    switch(pid) {
    case -1: //error
        rtnVal = -1;
        break;
    case 0: //chil process
        execl(cmd.c_str(), 0, 0); //run the command
        exit(1);
    default: //parent process, pid now contains the child pid
        while(-1 == waitpid(pid, &status, 0)); //wait for child to complete
        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            std::cerr << "process " << cmd << "(pid=" << pid << ") failed" << std::endl;
        } else {
            rtnVal = 0;
        }
        break;
    }
#endif
    return rtnVal;
}

