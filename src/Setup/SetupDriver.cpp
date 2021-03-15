//
// Created by Mohamed Ashraf on 2021-02-27.
//
#include "../MainDriver.h"
#include "Setup.h"

using namespace std;

void Driver::part6() {
  Setup *setup = new Setup();
  setup->loadGame();
  setup->initializePlayers();
  setup->initializeDeck();
  setup->initializeBidding();
  setup->mainLoop();
}

// void Driver::part8() {
//
//    Setup *setup = new Setup();
//
//    setup->startGame();
//
//    setup->mainLoop();
//}