//
// Created by Mohamed Ashraf on 2021-02-27.
//

#pragma once

#include "../Map/Map.h"
#include "../MapLoader/MapLoader.h"
#include "../Player/Player.h"
#include "../Cards/Cards.h"

class Setup {
 public:
  int startGame();
  Map *map;
  vector<Player *> *players;
  Deck *deck;
  Player *startingPlayer;

  Player *findPlayer(string playerName);
  void addArmy(Player &player, int *count);
  void moveOverLand(Player &player, int *count);
  void moverOverLandOrWater(Player &player, int *count);
  void buildCity(Player &player, int *count);
  void destroyArmy(Player &player, int *count);
};
