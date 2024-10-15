
include(GoogleTest)

add_executable(VectorTest Vector_Test.cpp)

find_package(GTest REQUIRED)
target_link_libraries(VectorTest PRIVATE gtest::gtest)
target_include_directories(VectorTest PUBLIC ${PROJECT_SOURCE_DIR}/RayTracer/include)

gtest_discover_tests(VectorTest)