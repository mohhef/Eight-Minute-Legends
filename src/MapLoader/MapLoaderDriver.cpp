//
// Created by Mohamed Ashraf on 2021-02-14.
//

#include "../Map/Map.h"
#include "MapLoader.h"
void part2(){
  //change the file name to load a different map
  Map *map = MapLoader::loadmap("../Files/invalidMap.txt");
  map->displayMap();
}
