
find_package(GTest REQUIRED)
find_package(GLM REQUIRED)
include(GoogleTest)
include(${CMAKE_SOURCE_DIR}/cmake_helpers/CMakeLists.txt)

add_gtest(VectorTest Vector_Test.cpp)

add_gtest(PointTest Point_Test.cpp)

add_gtest(MatrixTest Matrix_Test.cpp)

target_link_libraries(MatrixTest PRIVATE glm::glm)