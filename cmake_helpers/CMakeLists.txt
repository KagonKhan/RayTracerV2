function(print_header header)
    message("\n\n=======================   ${header}   =======================\n\n")
endfunction()

function(add_gtest targetName)
    add_executable(${targetName} ${ARGN})

    target_link_libraries(${targetName} PRIVATE gtest::gtest)
    target_include_directories(${targetName} PUBLIC ${PROJECT_SOURCE_DIR}/external/include)

    gtest_discover_tests(${targetName})
endfunction()

function(install_conan)
    print_header("INSTALLING CONAN DEPS")
    execute_process(COMMAND conan install . --output-folder=out --build=missing --settings=build_type=Debug --format=json
        RESULT_VARIABLE return_code
        OUTPUT_VARIABLE conan_stdout
        ERROR_VARIABLE conan_stderr
        ECHO_ERROR_VARIABLE # show the text output regardless
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/ "
    )
    print_header("CONAN DEPS INSTALLED")
endfunction()