#pragma once
#include <string>
#include <utility>
#include <vector>

#include "../Map/Map.h"

class Player {
 private:
  Map* map;
  string* name;
  int* cubes;
  int* discs;
  int* coins;
  vector<pair<Region*, int>>* cities;
  vector<pair<Region*, int>>* armies;
  pair<Region*, int>* GetArmiesInRegion(Region* region);
  pair<Region*, int>* GetCitiesInRegion(Region* region);

 public:
  Player(Map* map, string name, int cubes_num, int discs_num, int coins_num);
  ~Player();
  bool PayCoin(int coins);
  bool PlaceNewArmies(int armies_num, Region* region);
  bool BuildCity(Region* region);
  bool DestroyArmy(Player* player, Region* region);
};