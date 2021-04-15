//
// Created by Mohamed Ashraf on 2021-02-27.
//

#pragma once

#include <map>

#include "../Cards/Cards.h"
#include "../Map/Map.h"
#include "../MapLoader/MapLoader.h"
#include "../Player/Player.h"
#include "GameObservers.h"

enum State { pickCard, showBoard };

class Setup : public Subject {
 public:
  Setup();
  ~Setup();
  void deepCopy(const Setup &obj);
  Setup(const Setup &setup);
  friend ostream &operator<<(ostream &os, const Setup &setup);
  Setup &operator=(const Setup setup);

  void changeState(State state);
  void loadGame();
  void initializePlayers();
  void initializeDeck();
  void Startup();
  int mainLoop();
  int computeScore();
  bool checkGameOver();
  void takeTurn(Player *player, int turn);
  void UpdateConquerings();
  void ConquerRegion(Player &player, Region &region);
  void ConquerContinent(Player &player, Continent &continent);

  Player *findPlayer(string playerName);
  bool andOrAction(Player &player, Cards &card);
  void addArmy(Player &player, int *count);
  void moverOverLandOrWater(Player &player, int *count);
  void buildCity(Player &player, int *count);
  void destroyArmy(Player &player, int *count);

  Map *map;
  vector<Player *> *players;
  Player *non_player;
  Deck *deck;
  Player *starting_player;
  State state;

  Player *current_player;
  Cards *selected_card;
  int *current_cost;
  vector<pair<Region *, Player *>> *conquered_regions;
  vector<pair<Continent *, Player *>> *conquered_continents;
};

class TurnView : public Observer {
 public:
  TurnView();
  TurnView(Setup *s);
  void update();
  void display();

 private:
  Setup *subject;
};