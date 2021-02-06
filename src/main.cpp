#include <iostream>
#include "Map/Map.h"

int main() {
    Map *map = new Map();

    auto *na = new Continent("North America");
    auto *europe = new Continent("Europe");

    map->addContinent(na);
//    map->addContinent(europe);

    auto *usa = new Region("USA", na);
    auto *can = new Region("Canada", na);
//    auto *germany = new Region("Germany", europe);
//    auto *france = new Region("France", europe);

    map->addRegion(usa);
    map->addRegion(can);
//    map->addRegion(germany);
//    map->addRegion(spain);
//    map->addRegion(italy);

    // display with only countries
    map->displayMap();

    map->addPath(usa, can, 1);
//    map->addPath(can, germany,0);
//    map->addPath(germany, france,1);

    // display with countries and adjacencies
    map->displayMap();
    map->isValid();
}
