//
// Created by Mohamed Ashraf on 2021-02-27.
//
#include "../MainDriver.h"
#include "Setup.h"

using namespace std;

void Driver::part6() {
  Setup *setup = new Setup();
  TurnView turnView = TurnView(setup);
  StatsView statsView = StatsView(setup);

  // p1 (load map)
  setup->loadGame();
  // p1 (2 player game, each player has a bidding facility, game components)
  setup->initializePlayers();
  // p1 (deck created) //p2 (shuffle deck in deck constructor)
  setup->initializeDeck();
  // p2 (resource assignment, bidding and assigning who will start)
  setup->Startup();
  // p3 (running game loop, pay for cards, display of each player card) //p4(methods) //p5
  // happens within this main loop
  setup->mainLoop();
  // p6 (computing the score)
  setup->computeScore();
}