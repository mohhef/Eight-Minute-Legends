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
  map->addRegion(germany, true);
  map->addRegion(france);
  map->addPath(germany, france, 0);

  // Creating player
  Player *p1 = new Player(map, "Maxim", 18, 3, 9);
  Player *p2 = new Player(map, "Gleb", 18, 3, 9);
  p1->PlaceNewArmies(5, germany);
  p1->MoveArmies(5, germany, france);
}