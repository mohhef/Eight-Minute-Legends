//
// Created by Mohamed Ashraf on 2021-02-27.
//
#include "../MainDriver.h"
#include "Setup.h"

using namespace std;

void Driver::part6() {

    Setup *setup = new Setup();
//setup->startGame();
  Map *map = new Map();
  Player *p1 = new Player(map, "Maxim", 18, 3, 9);
  Cards *c1 = new Cards("PLACE_ARMIES_OR_MOVE_ARMIES|3|2","","",12);
  setup->andOrAction(*p1, *c1);

}