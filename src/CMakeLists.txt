cmake_minimum_required(VERSION 3.17)
project(src)

set(CMAKE_CXX_STANDARD 17)

add_executable(src Map/MapDriver.cpp Map/Map.cpp Map/Map.h)