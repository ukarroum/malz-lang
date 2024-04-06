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
        malz-lang/ir.hpp
        malz-lang/vm.cpp
        malz-lang/vm.hpp
        malz-lang/optimizer.cpp
        malz-lang/optimizer.hpp)
