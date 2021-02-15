#include <iostream>
#include "Map.h"
#include "../MapLoader/MapLoader.h"

int main() {
  Map *map = new Map();

  Map *map1 = new Map();


  auto *na = new Continent("North America");
  auto *europe = new Continent("Europe");

  map->addContinent(na);
  map->addContinent(europe);

  auto *usa = new Region("USA", na);
  auto *can = new Region("Canada", na);
  auto *germany = new Region("Germany", europe);
  auto *usa2 = new Region("USA", europe);
  auto *france = new Region("France", europe);

  map->addRegion(usa);
  map->addRegion(can, true);
  map->addRegion(germany);
  map->addRegion(france);

//  map->displayMap();
  map->addPath(usa, can, 1);
  map->addPath(can, germany, 0);
  map->addPath(germany, france, 1);

  Map map2 = *map;
  map2.displayMap();
  map2.isValid();
  map->displayMap();
  map->isValid();
}
