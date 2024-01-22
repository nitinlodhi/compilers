add_library(lox lox.cpp)

target_link_libraries(lox scanner parser interpreter)

target_include_directories(
    lox PUBLIC
    ./include
    ../../include
    ../scanner/include
    ../parser/include
    ../interpreter/include
    ../ast/include
)

# Unittest
enable_testing()

add_executable(lox_test test/lox_test.cpp)

target_link_libraries(
    lox_test
    lox
    GTest::gtest_main
)


include(GoogleTest)
gtest_discover_tests(lox_test)