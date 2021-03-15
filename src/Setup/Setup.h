//
// Created by Mohamed Ashraf on 2021-02-27.
//

#pragma once

#include "../Cards/Cards.h"
#include "../Map/Map.h"
#include "../MapLoader/MapLoader.h"
#include "../Player/Player.h"

class Setup {
 public:
  void loadGame();
  void initializePlayers();
  void initializeDeck();
  void initializeBidding();
  void Startup();
  int mainLoop();
  bool checkGameOver();
  void takeTurn(Player *player, int turn);

  Player *findPlayer(string playerName);
  void addArmy(Player &player, int *count);
  void moveOverLand(Player &player, int *count);
  void moverOverLandOrWater(Player &player, int *count);
  void buildCity(Player &player, int *count);
  void destroyArmy(Player &player, int *count);

  Map *map;
  vector<Player *> *players;
  Deck *deck;
  Player *startingPlayer;
};
