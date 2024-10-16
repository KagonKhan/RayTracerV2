
find_package(GTest REQUIRED)
include(GoogleTest)
include(${CMAKE_SOURCE_DIR}/cmake_helpers/CMakeLists.txt)

add_gtest(VectorTest Vector_Test.cpp)

add_gtest(PointTest Point_Test.cpp)

add_gtest(MatrixTest Matrix_Test.cpp)
