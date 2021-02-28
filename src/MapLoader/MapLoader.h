//
// Created by Dellen on 2021/2/13.
//


#pragma once

#include "../Map/Map.h"

using namespace std;

class MapLoader {
 public:
  static Map *loadmap(string fileName);
  Map* chooseMap();
};
