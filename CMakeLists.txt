cmake_minimum_required(VERSION 3.27)
project(malz_lang)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

add_executable(malz_lang
        malz-lang/main.cpp
        malz-lang/chunk.cpp
        malz-lang/debug.cpp
        malz-lang/value.cpp
        malz-lang/value.hpp
        malz-lang/ir.cpp
        malz-lang/vm.cpp
        malz-lang/optimizer.cpp
)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/f8d7d77c06936315286eb55f8de22cd23c188571.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(malz_test
        tests/ir_test.cpp
        malz-lang/chunk.cpp
        malz-lang/debug.cpp
        malz-lang/value.cpp
        malz-lang/value.hpp
        malz-lang/ir.cpp
        malz-lang/vm.cpp
        malz-lang/optimizer.cpp
        tests/optimizer_test.cpp
)
target_link_libraries(
        malz_test
        GTest::gtest_main
        GTest::gtest
)

include(GoogleTest)
gtest_discover_tests(malz_test)