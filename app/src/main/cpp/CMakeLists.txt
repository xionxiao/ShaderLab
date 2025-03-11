cmake_minimum_required(VERSION 3.22.1)

project("native-surface")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_EXPORT_COMPILE_COMMANDS)

include_directories(include)

add_library(${CMAKE_PROJECT_NAME} SHARED
    # List C/C++ source files with relative paths to this CMakeLists.txt.
    Shader.cpp
    Renderer.cpp
    RenderThread.cpp
    native_surface.cpp
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    # List libraries link to the target library
    android
    EGL
    GLESv3
    log
)
