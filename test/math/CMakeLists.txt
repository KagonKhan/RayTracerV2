
find_package(GTest REQUIRED)
include(GoogleTest)

function(add_gtest targetName)
    add_executable(${targetName} ${ARGN})

    target_link_libraries(${targetName} PRIVATE gtest::gtest)
    target_include_directories(${targetName} PUBLIC ${PROJECT_SOURCE_DIR}/external/include)

    gtest_discover_tests(${targetName})
endfunction()

add_gtest(VectorTest Vector_Test.cpp)

add_gtest(PointTest Point_Test.cpp)

add_gtest(MatrixTest Matrix_Test.cpp)
