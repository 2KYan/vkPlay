file(GLOB BASE_SRC *.cpp)
file(GLOB BASE_HEADERS *.hpp *.h)

file(GLOB IMGUI_SRC ../../extern/imgui/*.cpp)
source_group("imgui" FILES ${IMGUI_SRC})

if(WIN32)
    add_library(base STATIC ${BASE_SRC} ${BASE_HEADERS} ${IMGUI_SRC})
    target_link_libraries(base ${Vulkan_LIBRARY} ${ASSIMP_LIBRARIES} ${WINLIBS})
 else(WIN32)
    add_library(base STATIC ${BASE_SRC} ${IMGUI_SRC})
    target_link_libraries(base ${Vulkan_LIBRARY} ${ASSIMP_LIBRARIES} ${XCB_LIBRARIES} ${WAYLAND_CLIENT_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
endif(WIN32)