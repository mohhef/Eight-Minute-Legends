#pragma once
#include <string>
#include <utility>
#include <vector>

#include "../BiddingFacility/BiddingFacility.h"
#include "../Cards/Cards.h"
#include "../Map/Map.h"
#include "PlayerStrategy.h"

class Player {
 private:
  Map *map;
  string *name;
  int *cubes;
  int *discs;
  int *coins;
  int* score;
  vector<pair<Region *, int>> *cities;
  vector<pair<Region *, int>> *armies;
  BiddingFacility *bidding_facility;
  Hand *hand;
  PlayerStrategy *strategy;

 public:
  Player(Map *map, string name, int cubes_num, int discs_num, int coins_num, PlayerStrategy* strategy, int maxHandSize);
  Player(int coins, string name);
  Player(const Player &player);
  ~Player();
  Map *GetMap() const;
  string GetName() const;
  int GetCubes() const;
  int GetDiscs() const;
  int GetCoins() const;
  int GetScore() const;
  void SetScore(int);
  pair<Region *, int> *GetArmiesInRegion(Region *region);
  pair<Region *, int> *GetCitiesInRegion(Region *region);
  vector<pair<Region *, int>> *GetCities() const;
  vector<pair<Region *, int>> *GetArmies() const;
  int countHandCardNameStartsWith(string) const;
  int countHandCardAbilityEquals(string) const;
  BiddingFacility *GetBiddingFacility() const;
  Hand *GetHand() const;
  PlayerStrategy *GetStrategy() const;
  Player &operator=(const Player &player);
  friend ostream &operator<<(ostream &os, const Player &player);
  bool PayCoin(int coins, bool won_bid = false);
  bool PlaceNewArmies(int armies_num, Region *region, bool force = false);
  bool BuildCity(Region *region);
  bool DestroyArmy(Player *player, Region *region);
  bool MoveArmies(int &armies_num, Region *origin, Region *destination);
  bool MoveOverLand(int armies_num, Region *origin, Region *destination);
  bool MoveOverWater(int armies_num, Region *origin, Region *destination);
  void setStrategy(PlayerStrategy *playerStrategy);
  int pickCard(Deck *deck, vector<Player *> *players);
  int pickAction(string cardAction);
  Region* pickRegion(Map *map, string type, vector<Player *> *players);
  string pickPlayer();
  bool skipTurn();
  int pickArmies(string type, int count);
  Player* pickPlayer(vector<Player *> *players);
  int getArmyCount() const;
  Region* getRegionWithMostArmies() const;
  Region* getRegionWithLeastArmies() const;
  Region* getRegionWithLeastCities() const;
  vector<Region *> getRegionsWithArmies();
  vector<Region *> getRegionsWithCities();
};