cmake_minimum_required(VERSION 3.5)
cmake_policy(SET CMP0091 NEW)

set(CONAN_SYSTEM_INCLUDES ON)

project(RayTracer VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 26)


find_package(ZLIB REQUIRED)
# https://github.com/conan-io/cmake-conan/blob/develop2/conan_provider.cmake

add_executable(RayTracer
    RayTracer/src/main.cpp
   # RayTracer/thirdparty/imgui/imgui.cpp
   # RayTracer/thirdparty/imgui/imgui_demo.cpp
   # RayTracer/thirdparty/imgui/imgui_draw.cpp
   # RayTracer/thirdparty/imgui/imgui_internal.h
   # RayTracer/thirdparty/imgui/imgui_tables.cpp
   # RayTracer/thirdparty/imgui/imgui_widgets.cpp
   # RayTracer/thirdparty/imgui/imgui_impl_opengl3.cpp
   # RayTracer/thirdparty/imgui/imgui_impl_glfw.cpp
)
target_link_libraries(RayTracer ZLIB::ZLIB)

set_property(TARGET RayTracer PROPERTY CXX_STANDARD 23)


# include(CTest)
# enable_testing()

