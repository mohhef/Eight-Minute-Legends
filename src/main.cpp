#include <iostream>
#include "Map/Map.h"

int main() {
    Map *map = new Map();

    auto *north = new Continent("North America");
    auto *europe = new Continent("Europe");

    map->addContinent(north);
    map->addContinent(europe);

    auto *usa = new Region("USA", north);
    auto *can = new Region("Canada", north);
//    auto *portugal = new Region("Portugal", europe);
//    auto *spain = new Region("Spain", europe);
//    auto *italy = new Region("Italy", europe);

    map->addRegion(usa);
    map->addRegion(can);
//    map->addRegion(portugal);
//    map->addRegion(spain);
//    map->addRegion(italy);

    // display with only countries
    map->displayMap();

    map->addPath(usa, can, 1);
//    map->addPath(can, portugal,1);
//    map->addPath(spain, portugal,0);
//    map->addPath(spain, italy,1);

    // display with countries and adjacencies
    map->displayMap();
    map->isValid();
}
