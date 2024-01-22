add_library(ast_printer ast_printer.cpp)

include(FetchContent)
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)

FetchContent_MakeAvailable(json)

target_include_directories(
    ast_printer PUBLIC
    ${nlohmann_json_SOURCE_DIR}/include
    ./include
    ../../include
    ../scanner/include
    ../lox/include
)


# Unittest
enable_testing()

add_executable(ast_printer_test test/ast_printer_test.cpp)

target_link_libraries(
    ast_printer_test
    ast_printer
    parser
    scanner
    GTest::gtest_main
    nlohmann_json::nlohmann_json
)


include(GoogleTest)
gtest_discover_tests(ast_printer_test)