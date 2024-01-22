add_library(scanner scanner.cpp token.cpp)

target_include_directories(
    scanner PUBLIC
    ./include
    ../../include
    ../lox/include
)


# Unittest
enable_testing()

add_executable(scanner_test test/scanner_test.cpp)

target_link_libraries(
    scanner_test
    scanner
    GTest::gtest_main
)


include(GoogleTest)
gtest_discover_tests(scanner_test)