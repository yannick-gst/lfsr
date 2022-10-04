cmake_minimum_required(VERSION 3.5)
project(lfsr)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES maxlfsr.h maxlfsr.cpp test.cpp)
add_executable(lfsr ${SOURCE_FILES})