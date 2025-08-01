cmake_minimum_required(VERSION 3.31)
project(Quokka)

set(CMAKE_CXX_STANDARD 23)

add_executable(Quokka sandbox/main.cpp
        src/time/datetime.h)
