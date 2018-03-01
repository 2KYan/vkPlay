cmake_minimum_required(VERSION 3.0.0)

project(vkTF)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(shared)

if (DEFINED ENV{ARCH})
    set(BUILD_ARCH $ENV{ARCH})
else()
    set(BUILD_ARCH x64)
endif()

find_package(Vulkan REQUIRED)

set(EXTERN_DIR ${CMAKE_SOURCE_DIR}/extern)

include_directories(${VULKAN_INCLUDE_DIR})
include_directories(${EXTERN_DIR}/glm)
include_directories(${EXTERN_DIR}/gli)
include_directories(${EXTERN_DIR})

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/base)

link_directories(${EXTERN_DIR}/lib/${BUILD_ARCH})
link_directories(${VULKAN_LIBRARY_DIR})

set(LIBRARIES 
    shaderc_shared
)

if (WIN32)
    add_definitions(-DVK_USE_PLATFORM_WIN32_KHR -DWIN32_LEAN_AND_MEAN)
endif()

add_subdirectory(include/base)

add_library(vkTFlib SHARED
    include/vkTFlib.h
    include/vkTFInterface.h
    lib/vkTFlib.cpp
)
target_link_libraries(vkTFlib ${LIBRARIES})


add_executable(vkTF WIN32
    src/vkTF.cpp
)

add_subdirectory(test)


