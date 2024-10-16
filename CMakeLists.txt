cmake_minimum_required(VERSION 3.5)
cmake_policy(SET CMP0091 NEW)

project(RayTracer VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
message("\n\nPRE\n\n")
execute_process(COMMAND conan install . --output-folder=out --build=missing --settings=build_type=Debug --format=json
    RESULT_VARIABLE return_code
    OUTPUT_VARIABLE conan_stdout
    ERROR_VARIABLE conan_stderr
    ECHO_ERROR_VARIABLE # show the text output regardless
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/ "
)

message("\n\nPOST\n\n")

find_package(FMT REQUIRED)

# https://github.com/conan-io/cmake-conan/blob/develop2/conan_provider.cmake
add_executable(RayTracer
    RayTracer/src/main.cpp
    RayTracer/src/MainWindow.cpp
    RayTracer/src/App.cpp
    RayTracer/src/Image.cpp
    RayTracer/src/Renderer.cpp
)

add_subdirectory(external/src/ImGui)

target_include_directories(RayTracer
    PUBLIC external/ImGui/include
    PUBLIC RayTracer/include
)
target_link_libraries(RayTracer fmt::fmt ImGui)

set_property(TARGET RayTracer PROPERTY CXX_STANDARD 23)

enable_testing()

add_subdirectory(test)