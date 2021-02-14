//
// Created by Mohamed Ashraf on 2021-02-14.
//

#include "../Map/Map.h"
#include "MapLoader.h"
int main(){
  Map* map = MapLoader::loadmap("../valid.txt");
  map->displayMap();

}
