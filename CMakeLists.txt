cmake_minimum_required(VERSION 3.5)
project(LightCompiler)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp parser.h scaner.h parser.cpp scaner.cpp)
add_executable(LightCompiler ${SOURCE_FILES})