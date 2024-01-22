add_library(parser parser.cpp)

target_include_directories(
    parser PUBLIC
    ./include
    ../../include
    ../scanner/include
    ../lox/include
)


# Unittest
enable_testing()

add_executable(parser_test test/parser_test.cpp)

target_link_libraries(
    parser_test
    parser
    scanner
    GTest::gtest_main
)


include(GoogleTest)
gtest_discover_tests(parser_test)