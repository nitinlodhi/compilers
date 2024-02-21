add_library(interpreter interpreter.cpp environment.cpp)

target_include_directories(
    interpreter PUBLIC
    ./include
    ../../include
    ../scanner/include
    ../lox/include
)


# Unittest
enable_testing()

add_executable(interpreter_test test/interpreter_test.cpp)

target_link_libraries(
    interpreter_test
    interpreter
    scanner
    parser
    GTest::gtest_main
)


include(GoogleTest)
gtest_discover_tests(interpreter_test)