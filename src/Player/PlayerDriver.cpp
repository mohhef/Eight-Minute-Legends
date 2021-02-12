#include <iostream>

#include "../Map/Map.h"
#include "Player.h"
using namespace std;

int main() {
  // Creating a map
  Map *map = new Map();
  auto *europe = new Continent("Europe");
  map->addContinent(europe);
  auto *germany = new Region("Germany", europe);
  auto *france = new Region("France", europe);
  map->addRegion(germany);
  map->addRegion(france);

  // Creating player
  Player *p1 = new Player(map, "Maxim", 18, 3, 9);
  Player *p2 = new Player(map, "Gleb", 18, 3, 9);
  p1->DestroyArmy(p2, germany);
}