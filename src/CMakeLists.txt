cmake_minimum_required(VERSION 3.17)
project(src)

set(CMAKE_CXX_STANDARD 17)

#Map Part1
add_executable(src Map/MapDriver.cpp Map/Map.cpp Map/Map.h)

#MapLoader Part2
#add_executable(src MapLoader/MapLoaderDriver.cpp Map/Map.cpp MapLoader/MapLoader.cpp Map/Map.h MapLoader/MapLoader.h)

#Player Part3
#add_executable(src Player/PlayerDriver.cpp Player/Player.cpp Player/Player.h Map/Map.cpp Map/Map.h)
