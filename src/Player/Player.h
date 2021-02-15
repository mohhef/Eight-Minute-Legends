#pragma once
#include <string>
#include <utility>
#include <vector>

#include "../Map/Map.h"

class Player {
 private:
  Map *map;
  string *name;
  int *cubes;
  int *discs;
  int *coins;
  vector<pair<Region *, int>> *cities;
  vector<pair<Region *, int>> *armies;
  pair<Region *, int> *GetArmiesInRegion(Region *region);
  pair<Region *, int> *GetCitiesInRegion(Region *region);
  bool MoveOverLand(int armies_num, Region *origin, Region *destination);
  bool MoveOverWater(int armies_num, Region *origin, Region *destination);

 public:
  Player(Map *map, string name, int cubes_num, int discs_num, int coins_num);
  Player(const Player &player);
  ~Player();
  Map *GetMap() const;
  string GetName() const;
  int GetCubes() const;
  int GetDiscs() const;
  int GetCoins() const;
  vector<pair<Region *, int>> *GetCities() const;
  vector<pair<Region *, int>> *GetArmies() const;
  Player &operator=(const Player &player);
  friend ostream &operator<<(ostream &os, const Player &player);
  bool PayCoin(int coins);
  bool PlaceNewArmies(int armies_num, Region *region);
  bool BuildCity(Region *region);
  bool DestroyArmy(Player *player, Region *region);
  bool MoveArmies(int armies_num, Region *origin, Region *destination);
};