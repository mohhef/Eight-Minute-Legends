//
// Created by Mohamed Ashraf on 2021-02-27.
//

#pragma once

#include "../Map/Map.h"
#include "../MapLoader/MapLoader.h"
#include "../Player/Player.h"

class Setup {
public:
    int startGame();
    Map* map;
    vector<Player*> *players;
    Deck *deck;
};
