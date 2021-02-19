#include <iostream>
#include "Map.h"
#include "../MainDriver.h"

void Driver::part1() {
  //Valid Map
  Map *map = new Map();

  auto *na = new Continent("North America");
  auto *europe = new Continent("Europe");
  auto *asia = new Continent("Asia");

  map->addContinent(na);
  map->addContinent(europe);
  map->addContinent(asia);

  auto *usa = new Region("USA", na);
  auto *can = new Region("Canada", na);

  auto *germany = new Region("Germany", europe);
  auto *france = new Region("France", europe);

  auto *india = new Region("India", asia);
  auto *pakistan = new Region("Pakistan", asia);

  map->addRegion(usa);
  map->addRegion(can, true);
  map->addRegion(germany);
  map->addRegion(france);
  map->addRegion(india);
  map->addRegion(pakistan);

  map->addPath(usa, can, 1);
  map->addPath(can, germany, 0);
  map->addPath(germany, france, 1);
  map->addPath(france, india, 0);
  map->addPath(india, pakistan, 1);

  cout<<*map;
  map->isValid();

  //Invalid map, Copy constructor, Stream insertion operator
  Map map2;
  map2 = *map;

  auto *africa = new Continent("Africa");
  auto *egypt = new Region("Egypt", africa);
  map2.addContinent(africa);
  map2.addRegion(egypt);
  cout<<map2;
  map2.isValid();

  delete map;
}

