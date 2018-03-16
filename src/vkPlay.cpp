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
#include <iostream>
#include <string>

#include <filesystem>

#include "vkPlay.h"
#include "vkTFInterface.h"
#include "getopt.h"

#ifdef _WIN32
#pragma comment(linker, "/subsystem:console")
//#pragma comment(linker, "/subsystem:console /ENTRY:mainCRTstartup")
#endif

std::string& trim(std::string& s, const char* t=" \t\n\r\f\v") 
{
    s.erase(0, s.find_first_not_of(t));
    s.erase(s.find_last_not_of(t)+1);

    return s;
}

vkTFInterface::TestArgs parsingArgs(int argc, char**argv)
{
    vkTFInterface::TestArgs args;
    struct option long_options[] = 
    {
        { "verbose", no_argument      , &args.verbose_flag, 1  },
        { "window",  no_argument      , &args.window_flag , 1  },
        { "dump",    no_argument      , &args.dump_flag   , 1  },
        { "test",    required_argument, 0,                 't' },
        { "frame",   required_argument, 0,                 'f' },
        { "total",   required_argument, 0,                 'l' },
        { "height",  required_argument, 0,                 'h' },
        { "width",   required_argument, 0,                 'w' },
        { 0,         0                , 0,                  0  },

    };

    int option_index = 0;
    while(1)
    {
        int c = getopt_long_only(argc, argv, "t:f:l:h:w:", long_options, &option_index);
        if (c == -1) break;

        switch(c)
        {
            case 0:
                if (long_options[option_index].flag == 0) {
                    fprintf(stdout, "option %s with addtional args %s", long_options[option_index].name, optarg);
                    assert(false);
                }
                break;
            case 't':
            {
                std::istringstream ss(optarg);
                std::string token;
                while(std::getline(ss, token, ';')) {
                    trim(token);
                    args.testNames.push_back(token);
                }
                break;
            }
            case 'f':
                args.start_frame = std::stoi(optarg);
                break;
            case 'l':
                args.total_frame = std::stoi(optarg);
                break;
            case 'h':
                args.height = std::stoi(optarg);
                break;
            case 'w':
                args.width = std::stoi(optarg);
                break;
            case '?':
                /* getopt_long already printed an error message */
                break;
            default:
                assert(false && "Incorrect command line options");
        }
    }
    /* Old code to get dll*/
    while (optind < argc) {
        std::string blockname;
        std::string filename = std::string(argv[optind]);
        size_t pos = 0;
        size_t block_offset = 0;
        if ((pos = filename.rfind(".so")) == (filename.size() - 3)) {
        }
        if ((pos = filename.rfind(".dll")) == (filename.size() - 4)) {
            if ((block_offset = filename.rfind("_")) != std::string::npos) {
                blockname = filename.substr(block_offset+1, pos - block_offset - 1);
                args.libNames.insert(std::make_pair(filename, blockname));
            }
        }
        optind++;
    }

    return args;
}

int main(int argc, char** argv) {
    vkTFInterface::TestArgs args = parsingArgs(argc, argv);

    std::string path = ".";
    for (auto & p : std::experimental::filesystem::directory_iterator(path)) {
        std::string extension = p.path().extension().string();
        if (extension == ".dll" || extension == ".so") {
            std::string stem = p.path().stem().string();
            std::string filename = p.path().filename().string();
            auto  block_offset = stem.rfind("_");
            if (block_offset != std::string::npos)            
            {
                auto blockname = stem.substr(block_offset + 1, stem.size() - 1);
                args.libNames.insert(std::make_pair(filename, blockname));
                std::cout << filename << std::endl;
            }
        }
    }
 
    for(auto lib: args.libNames) {
        HINSTANCE hDllInst = LoadLibrary(lib.first.c_str());
        if (hDllInst != nullptr) {
            for(auto testname: args.testNames) {
                std::string entry_func = lib.second + "_" + testname;
                auto allocator = reinterpret_cast<vkTFInterface*(*)(void*)>(GetProcAddress(hDllInst, entry_func.c_str()));
                if (allocator != nullptr) {
                    vkTFInterface* testObj = allocator((void*)&args);
                    testObj->vkTFInit(nullptr);
                    testObj->vkTFRender(0);
                    testObj->vkTFExit();

                    delete testObj;
                }
            }
            FreeLibrary(hDllInst);
        }
    }
}
