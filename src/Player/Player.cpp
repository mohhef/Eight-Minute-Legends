#include "Player.h"
using namespace std;

/*
Player constructor to initialize player attributes
*/
Player::Player(Map *map, string name, int cubes_num, int discs_num, int coins_num) {
  this->map = map;
  this->name = new string(name);
  cubes = new int(cubes_num);
  discs = new int(discs_num);
  coins = new int(coins_num);
  cities = new vector<pair<Region *, int>>;
  armies = new vector<pair<Region *, int>>;
  bidding_facility = new BiddingFacility(coins_num, name);
  hand = new Hand();
  for (auto region : *(this->map->regions)) {
    cities->push_back(make_pair(region.first, 0));
    armies->push_back(make_pair(region.first, 0));
  }
}

Player::Player(int coins, string name) {
  this->map = new Map();
  this->cubes = new int(0);
  this->discs = new int(0);
  this->name = new string(name);
  this->coins = new int(coins);
  cities = new vector<pair<Region *, int>>;
  armies = new vector<pair<Region *, int>>;
  bidding_facility = new BiddingFacility(coins, name);
  this->hand = new Hand();
}
/*
Copy constructor to copy player
*/
Player::Player(const Player &player) {
  map = player.GetMap();
  name = new string(player.GetName());
  cubes = new int(player.GetCubes());
  discs = new int(player.GetDiscs());
  coins = new int(player.GetCoins());
  cities = new vector<pair<Region *, int>>;
  armies = new vector<pair<Region *, int>>;
  for (auto region : *player.GetCities()) {
    cities->push_back(make_pair(region.first, region.second));
  }
  for (auto region : *player.GetArmies()) {
    armies->push_back(make_pair(region.first, region.second));
  }
  bidding_facility = new BiddingFacility(*player.GetBiddingFacility());
  hand = player.GetHand();
}

/*
Deconstructor to deallocate heap variables
Here the pointers to region in cities and armies are not deallocated because,
 that would destroy our map
*/
Player::~Player() {
  delete name;
  delete cubes;
  delete discs;
  delete coins;
  delete cities;
  delete armies;
  delete bidding_facility;
}

Map *Player::GetMap() const { return map; }

string Player::GetName() const { return *name; }

int Player::GetCubes() const { return *cubes; }

int Player::GetDiscs() const { return *discs; }

int Player::GetCoins() const { return *coins; }

vector<pair<Region *, int>> *Player::GetCities() const { return cities; }

vector<pair<Region *, int>> *Player::GetArmies() const { return armies; }

BiddingFacility *Player::GetBiddingFacility() const { return bidding_facility; }

Hand *Player::GetHand() const { return hand; }

/*
Player assignment operator
Only deep copy player specific arguments
Avoid memory leak by only delete non-deeply copied arguments
*/
Player &Player::operator=(const Player &player) {
  if (this != &player) {
    map = player.GetMap();
    *name = player.GetName();
    *cubes = player.GetCubes();
    *discs = player.GetDiscs();
    *coins = player.GetCoins();
    delete cities;
    cities = new vector<pair<Region *, int>>;
    delete armies;
    armies = new vector<pair<Region *, int>>;
    for (auto region : *player.cities) {
      cities->push_back(make_pair(region.first, region.second));
    }
    for (auto region : *player.armies) {
      armies->push_back(make_pair(region.first, region.second));
    }
    delete bidding_facility;
    bidding_facility = new BiddingFacility(*player.GetBiddingFacility());
    hand = player.GetHand();
  }
  return *this;
}

/*
Stream insertion operator
*/
ostream &operator<<(ostream &os, const Player &player) {
  os << "### " << player.GetName() << " ###" << endl;
  os << "Cubes: " << player.GetCubes() << endl;
  os << "Discs: " << player.GetDiscs() << endl;
  os << "Coins: " << player.GetCoins() << endl;
  os << "Cities: ";
  for (auto region : *(player.GetCities())) {
    os << *((region.first)->name) << "->" << region.second << " ";
  }
  os << endl;
  os << "Armies: ";
  for (auto region : *(player.GetArmies())) {
    os << *((region.first)->name) << "->" << region.second << " ";
  }
  os << endl;
  os << endl;
  return os;
}

/*
Lets a player pay an amount of coins
*/
bool Player::PayCoin(int coins, bool won_bid) {
  if (*this->coins < coins) {
    cout << "Player::PayCoin(): Not enough coins." << endl;
    return false;
  } else {
    *this->coins -= coins;
    if (won_bid) {
      this->bidding_facility->subtractBid();
    }
    cout << *name << " has payed " << coins << " coins." << endl;
    return true;
  }
}

/*
Returns a region with the number of armies in it
*/
pair<Region *, int> *Player::GetArmiesInRegion(Region *region) {
  vector<pair<Region *, int>>::iterator i;
  for (i = armies->begin(); i != armies->end(); ++i) {
    if (i->first == region) {
      return &(*i);
    }
  }
  return nullptr;
}

/*
Returns a region with the number of cities in it
*/
pair<Region *, int> *Player::GetCitiesInRegion(Region *region) {
  vector<pair<Region *, int>>::iterator i;
  for (i = cities->begin(); i != cities->end(); ++i) {
    if (i->first == region) {
      return &(*i);
    }
  }
  return nullptr;
}

/*
Places an amount of armies in a region
*/
bool Player::PlaceNewArmies(int armies_num, Region *region, bool force) {
  if (*cubes < armies_num) {
    cout << "Player::PlaceNewArmies(): Not enough armies." << endl;
    return false;
  }
  pair<Region *, int> *cities_in_region = GetCitiesInRegion(region);
  if (cities_in_region->second > 0 || region == map->startingRegion || force) {
    pair<Region *, int> *armies_in_region = GetArmiesInRegion(region);
    armies_in_region->second += armies_num;
    *cubes -= armies_num;
    cout << *name << " has placed " << armies_num << " new armies in " << *region->name << "." << endl;
    return true;
  } else {
    cout << "Player::PlaceNewArmies(): Cannot place armies in a region in which the "
            "player has no city."
         << endl;
    return false;
  }
}

/*
Builds city in a specific region
*/
bool Player::BuildCity(Region *region) {
  if (*discs < 1) {
    cout << "Player::BuildCity(): Not enough discs." << endl;
    return false;
  }
  pair<Region *, int> *armies_in_region = GetArmiesInRegion(region);
  if (armies_in_region->second > 0) {
    *discs -= 1;
    pair<Region *, int> *cities_in_region = GetCitiesInRegion(region);
    cities_in_region->second += 1;
    cout << *name << " has built a city in " << *region->name << "." << endl;
    return true;
  } else {
    cout << "Player::BuildCity(): Cannot build a city in a region in which the player "
            "has no armies."
         << endl;
    return false;
  }
}

/*
Destroy army in a region
*/
bool Player::DestroyArmy(Player *player, Region *region) {
  pair<Region *, int> *armies_in_region = GetArmiesInRegion(region);
  if (armies_in_region->second > 0) {
    pair<Region *, int> *enemy_armies_in_region = player->GetArmiesInRegion(region);
    if (enemy_armies_in_region->second > 0) {
      enemy_armies_in_region->second -= 1;
      cout << *name << " has destroyed " << *player->name << "'s army in "
           << *region->name << "." << endl;
      return true;
    } else {
      cout << "Player::DestroyArmy(): " << *player->name << " has no armies in "
           << *region->name << "." << endl;
      return false;
    }
  } else {
    cout << "Player::DestroyArmy(): Not enough armies to destroy enemy's army." << endl;
    return false;
  }
}

/*
Move number of armies from one region to another, by land
*/
bool Player::MoveOverLand(int armies_num, Region *origin, Region *destination) {
  pair<Region *, int> *armies_in_origin = GetArmiesInRegion(origin);
  pair<Region *, int> *armies_in_destination = GetArmiesInRegion(destination);
  armies_in_origin->second -= armies_num;
  armies_in_destination->second += armies_num;
  cout << *name << " has moved " << armies_num << " armies from " << *origin->name
       << " to " << *destination->name << " by land." << endl;
  return true;
}

/*
Move number of armies from one region to another, by water
*/
bool Player::MoveOverWater(int armies_num, Region *origin, Region *destination) {
  pair<Region *, int> *armies_in_origin = GetArmiesInRegion(origin);
  pair<Region *, int> *armies_in_destination = GetArmiesInRegion(destination);
  armies_in_origin->second -= armies_num;
  armies_in_destination->second += armies_num;
  cout << *name << " has moved " << armies_num << " armies from " << *origin->name
       << " to " << *destination->name << " by water." << endl;
  return true;
}

/*
Move number of armies from one region to another and determine if its by land or water
*/
bool Player::MoveArmies(int armies_num, Region *origin, Region *destination) {
  pair<Region *, int> *armies_in_origin = GetArmiesInRegion(origin);
  if (armies_in_origin->second >= armies_num) {
    int adjacency = map->isAdjacent(origin, destination);
    if (adjacency == 1) {
      return MoveOverLand(armies_num, origin, destination);
    } else if (adjacency == 0) {
      return MoveOverWater(armies_num, origin, destination);
    } else {
      cout << "Player::MovieArmies(): Origin and destination regions are not adjacent." << endl;
      return false;
    }
  } else {
    cout << "Player::MovieArmies(): Not enough armies in the origin region." << endl;
    return false;
  }
}