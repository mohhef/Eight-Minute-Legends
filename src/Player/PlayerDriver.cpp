#include <iostream>

#include "../Map/Map.h"
#include "Player.h"
#include "../MainDriver.h"
using namespace std;

void Driver::part3(){
  // Creating a map
  Map *map = new Map();
  auto *europe = new Continent("Europe");
  map->addContinent(europe);
  auto *germany = new Region("Germany", europe);
  auto *france = new Region("France", europe);
  map->addRegion(germany, true);
  map->addRegion(france);
  map->addPath(germany, france, 1);

  // Creating players
  Player *p1 = new Player(map, "Maxim", 18, 3, 9);
  Player *p2 = new Player(map, "Mohamed", 18, 3, 9);

  // Making different kinds of moves
  p1->PayCoin(3);
  p2->PayCoin(3);
  p1->PlaceNewArmies(2, germany);
  p2->PlaceNewArmies(1, germany);
  p1->MoveArmies(1, germany, france);
  p1->BuildCity(france);
  p1->DestroyArmy(p2, germany);

  // Visualize the state of each player
  cout << *p1;
  cout << *p2;
}