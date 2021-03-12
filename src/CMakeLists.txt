cmake_minimum_required(VERSION 3.17)
project(src)

set(CMAKE_CXX_STANDARD 17)

#All parts
add_executable(
        src MainDriver.cpp
        Map/MapDriver.cpp Map/Map.cpp Map/Map.cpp
        MapLoader/MapLoaderDriver.cpp MapLoader/MapLoader.cpp MapLoader/MapLoader.h
        Player/PlayerDriver.cpp Player/Player.cpp Player/Player.h
        Cards/CardsDriver.cpp Cards/Cards.cpp Cards/Cards.h
        BiddingFacility/BiddingFacilityDriver.cpp BiddingFacility/BiddingFacility.h BiddingFacility/BiddingFacility.cpp
        Setup/SetupDriver.cpp Setup/Setup.h Setup/Setup.cpp
)


#Map Part1(Mohamed)
#add_executable(src Map/MapDriver.cpp Map/Map.cpp Map/Map.h)

#MapLoader Part2(Lang)
#add_executable(src MapLoader/MapLoaderDriver.cpp Map/Map.cpp MapLoader/MapLoader.cpp Map/Map.h MapLoader/MapLoader.h)

#Player Part3(Maxim)
#add_executable(src Player/PlayerDriver.cpp Player/Player.cpp Player/Player.h Map/Map.cpp Map/Map.h)

#Cards Part4(Kosta)
#add_executable(src Cards/CardsDriver.cpp Cards/Cards.cpp Cards/Cards.h)

#Bidding Part5(Dominic)
#add_executable(src BiddingFacility/BiddingFacility.cpp BiddingFacility/BiddingFacility.h BiddingFacility/BiddingFacilityDriver.cpp)