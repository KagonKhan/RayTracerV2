cmake_minimum_required(VERSION 3.5.0)
project(RayTracer VERSION 0.1.0 LANGUAGES C CXX)


set(CMAKE_CXX_STANDARD 26)


add_executable(RayTracer main.cpp)

set_property(TARGET RayTracer PROPERTY CXX_STANDARD 23)


# include(CTest)
# enable_testing()

