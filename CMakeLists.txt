cmake_minimum_required(VERSION 3.27)
project(malz_lang)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

add_executable(malz_lang
        main.cpp
        chunk.cpp
        debug.cpp
        value.cpp
        value.hpp
        ir.cpp
        ir.hpp
        vm.cpp
        vm.hpp)
