//
// Created by Mohamed Ashraf on 2021-02-27.
//

#pragma once

#include "../Cards/Cards.h"
#include "../Map/Map.h"
#include "../MapLoader/MapLoader.h"
#include "../Player/Player.h"
#include "../Cards/Cards.h"
#include <map>

class Setup {
 public:
  Setup();
  ~Setup();
  void deepCopy(const Setup &obj);
  Setup(const Setup &setup);
  friend ostream &operator<<(ostream& os, const Setup& setup);
  Setup& operator=(const Setup setup);

  void loadGame();
  void initializePlayers();
  void initializeDeck();
  void Startup();
  int mainLoop();
  int computeScore();
  bool checkGameOver();
  void takeTurn(Player *player, int turn);

  Player *findPlayer(string playerName);
  bool andOrAction(Player &player, Cards &card);
  void addArmy(Player &player, int *count);
  void moverOverLandOrWater(Player &player, int *count);
  void buildCity(Player &player, int *count);
  void destroyArmy(Player &player, int *count);
  PlayerStrategy *chooseStrategy();

  Map *map;
  vector<Player *> *players;
  Player *non_player;
  Deck *deck;
  Player *starting_player;
};
