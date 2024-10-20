cmake_minimum_required(VERSION 3.5)
cmake_policy(SET CMP0091 NEW)

project(RayTracer VERSION 0.1.0 LANGUAGES CXX)

include(cmake_helpers/CMakeLists.txt)

set(CMAKE_CXX_STANDARD 23)
install_conan()

find_package(FMT REQUIRED)
find_package(glm REQUIRED)

add_subdirectory(external/src/ImGui)
add_subdirectory(external/include/RayMath)
add_subdirectory(RayTracer)

# https://github.com/conan-io/cmake-conan/blob/develop2/conan_provider.cmake
enable_testing()
add_subdirectory(test)