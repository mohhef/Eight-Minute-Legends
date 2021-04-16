#pragma once

#include "../Cards/Cards.h"
#include "../Map/Map.h"

class Player;

class PlayerStrategy {
public:
  virtual void placeBid(string selfName, vector<Player *> *players) = 0;
  virtual int pickCard(Deck* deck, int playerCoin, string selfName, vector<Player *> *players) = 0;
  virtual int pickAction(string cardAction) = 0;
  virtual Region* pickRegion(Map *map, string type, string selfName, vector<Player *> *players) = 0;
  virtual void moveRegion(Map *map, Region **from, Region **to, string selfName, vector<Player *> *players, int count) = 0;
  virtual bool skipTurn() = 0;
  virtual Player* pickPlayer(string selfName, vector<Player *> *players) = 0;
  virtual int pickArmies(string type, int count) = 0;
};

class HumanStrategy : public PlayerStrategy {
public:
  virtual void placeBid(string selfName, vector<Player *> *players);
  int pickCard(Deck* deck, int playerCoin, string selfName, vector<Player *> *players);
  int pickAction(string cardAction);
  Region* pickRegion(Map *map, string type, string selfName, vector<Player *> *players);
  void moveRegion(Map *map, Region **from, Region **to, string selfName, vector<Player *> *players, int count);
  bool skipTurn();
  Player* pickPlayer(string selfName, vector<Player *> *players);
  int pickArmies(string type, int count);
};

class GreedyAIStrategy : public PlayerStrategy {
public:
  virtual void placeBid(string selfName, vector<Player *> *players);
  int pickCard(Deck* deck, int playerCoin, string selfName, vector<Player *> *players);
  int pickAction(string cardAction);
  Region* pickRegion(Map *map, string type, string selfName, vector<Player *> *players);
  void moveRegion(Map *map, Region **from, Region **to, string selfName, vector<Player *> *players, int count);
  bool skipTurn();
  Player* pickPlayer(string selfName, vector<Player *> *players);
  int pickArmies(string type, int count);
};

class ModerateAIStrategy : public PlayerStrategy {
public:
  virtual void placeBid(string selfName, vector<Player *> *players);
  int pickCard(Deck* deck, int playerCoin, string selfName, vector<Player *> *players);
  int pickAction(string cardAction);
  Region* pickRegion(Map *map, string type, string selfName, vector<Player *> *players);
  void moveRegion(Map *map, Region **from, Region **to, string selfName, vector<Player *> *players, int count);
  bool skipTurn();
  Player* pickPlayer(string selfName, vector<Player *> *players);
  int pickArmies(string type, int count);
};