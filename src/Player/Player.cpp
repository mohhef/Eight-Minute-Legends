#include "Player.h"

#include <iostream>

#include "../Map/Map.h"
using namespace std;

Player::Player(Map* map, string name, int cubes_num, int discs_num, int coins_num) {
  this->map = map;
  this->name = new string(name);
  cubes = new int(cubes_num);
  discs = new int(discs_num);
  coins = new int(coins_num);
  cities = new vector<pair<Region*, int>>;
  armies = new vector<pair<Region*, int>>;
  for (auto region : *(this->map->regions)) {
    cities->push_back(make_pair(region.first, 0));
    armies->push_back(make_pair(region.first, 0));
  }
}

Player::~Player() {
  delete name;
  delete cubes;
  delete discs;
  delete coins;
  delete cities;
  delete armies;
}

bool Player::PayCoin(int coins) {
  if (*this->coins < coins) {
    cout << "Not enough coins." << endl;
    return false;
  } else {
    *this->coins -= coins;
    cout << "Player payed." << endl;
    return true;
  }
}

pair<Region*, int>* Player::GetArmiesInRegion(Region* region) {
  vector<pair<Region*, int>>::iterator i;
  for (i = armies->begin(); i != armies->end(); ++i) {
    if (i->first == region) {
      return &(*i);
    }
  }
  return NULL;
}

pair<Region*, int>* Player::GetCitiesInRegion(Region* region) {
  vector<pair<Region*, int>>::iterator i;
  for (i = cities->begin(); i != cities->end(); ++i) {
    if (i->first == region) {
      return &(*i);
    }
  }
  return NULL;
}

bool Player::PlaceNewArmies(int armies_num, Region* region) {
  if (*cubes < armies_num) {
    cout << "Not enough armies." << endl;
    return false;
  }
  pair<Region*, int>* cities_in_region = GetCitiesInRegion(region);
  if (cities_in_region->second > 0 || region == map->startingRegion) {
    pair<Region*, int>* armies_in_region = GetArmiesInRegion(region);
    armies_in_region->second += armies_num;
    cout << *name << " has placed " << armies_num << " new armies in " << *region->name
         << "." << endl;
    return true;
  } else {
    cout << "Unable to place armies in a region where player has no city." << endl;
    return false;
  }
}

bool Player::BuildCity(Region* region) {
  if (*discs < 1) {
    cout << "Not enough discs." << endl;
    return false;
  }
  pair<Region*, int>* armies_in_region = GetArmiesInRegion(region);
  if (armies_in_region->second > 0) {
    *discs -= 1;
    pair<Region*, int>* cities_in_region = GetCitiesInRegion(region);
    cities_in_region->second += 1;
    cout << *name << " has built a city in " << *region->name << "." << endl;
    return true;
  } else {
    cout << "Unable to build a city in a region where player has no armies." << endl;
    return false;
  }
}

bool Player::DestroyArmy(Player* player, Region* region) {
  pair<Region*, int>* armies_in_region = GetArmiesInRegion(region);
  if (armies_in_region->second > 0) {
    pair<Region*, int>* enemy_armies_in_region = player->GetArmiesInRegion(region);
    if (enemy_armies_in_region->second > 0) {
      enemy_armies_in_region->second -= 1;
      cout << *name << " has destroyed " << *player->name << "'s army in "
           << *region->name << "." << endl;
      return true;
    } else {
      cout << *player->name << " has no armies in " << *region->name << "." << endl;
      return false;
    }
  } else {
    cout << "Not enough armies to destroy enemy's army." << endl;
    return false;
  }
}

bool Player::MoveOverLand(int armies_num, Region* origin, Region* destination) {
  pair<Region*, int>* armies_in_origin = GetArmiesInRegion(origin);
  pair<Region*, int>* armies_in_destination = GetArmiesInRegion(destination);
  armies_in_origin->second -= armies_num;
  armies_in_destination->second += armies_num;
  cout << *name << " has moved " << armies_num << " armies from " << *origin->name
       << " to " << *destination->name << " by land." << endl;
  return true;
}

bool Player::MoveOverWater(int armies_num, Region* origin, Region* destination) {
  pair<Region*, int>* armies_in_origin = GetArmiesInRegion(origin);
  pair<Region*, int>* armies_in_destination = GetArmiesInRegion(destination);
  armies_in_origin->second -= armies_num;
  armies_in_destination->second += armies_num;
  cout << *name << " has moved " << armies_num << " armies from " << *origin->name
       << " to " << *destination->name << " by water." << endl;
  return true;
}

bool Player::MoveArmies(int armies_num, Region* origin, Region* destination) {
  pair<Region*, int>* armies_in_origin = GetArmiesInRegion(origin);
  if (armies_in_origin->second >= armies_num) {
    int adjacency = map->isAdjacent(origin, destination);
    if (adjacency == 1) {
      return MoveOverLand(armies_num, origin, destination);
    } else if (adjacency == 0) {
      return MoveOverWater(armies_num, origin, destination);
    } else {
      cout << "Player::MoviesArmies(): Origin and destination regions are not adjacent."
           << endl;
      return false;
    }
  } else {
    cout << "Player::MoviesArmies(): Not enough armies in the origin region." << endl;
    return false;
  }
}