cmake_minimum_required(VERSION 3.20)

project(cpplox CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG v1.14.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_subdirectory(src/scanner)
add_subdirectory(src/parser)
add_subdirectory(src/interpreter)
add_subdirectory(src/ast)
add_subdirectory(src/lox)

add_executable(cpplox src/main.cpp)
add_executable(astgen src/tools/astgen.cpp)

target_link_libraries(cpplox
    PUBLIC scanner
    PUBLIC parser
    PUBLIC interpreter
    lox)

target_include_directories(
    cpplox PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/src/scanner/include"
    "${PROJECT_SOURCE_DIR}/src/parser/include"
    "${PROJECT_SOURCE_DIR}/src/lox/include"
    "${PROJECT_SOURCE_DIR}/src/ast/include"
)

# enable_testing()

# add_executable(
#     cpplox_test
#     # test/main.cpp
#     src/scanner/test/scanner_test.cpp
# )

# target_link_libraries(
#     cpplox_test
#     scanner
#     GTest::gtest_main
# )


# target_include_directories(
#     cpplox_test PUBLIC
#     "${PROJECT_BINARY_DIR}"
#     "${PROJECT_SOURCE_DIR}/include"
#     "${PROJECT_SOURCE_DIR}/src/scanner/include"
#     "${PROJECT_SOURCE_DIR}/src/parser/include"
#     "${PROJECT_SOURCE_DIR}/src/lox/include"
#     "${PROJECT_SOURCE_DIR}/src/ast/include"
# )

# include(GoogleTest)
# gtest_discover_tests(cpplox_test)