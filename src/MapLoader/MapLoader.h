//
// Created by Dellen on 2021/2/13.
//


#ifndef EIGHT_MINUTE_EMPIRE_MAPLOADER_H
#define EIGHT_MINUTE_EMPIRE_MAPLOADER_H

#include "../Map/Map.h"

using namespace std;

class MapLoader {
public:
    static Map* loadmap(string fileName);
};

#endif //EIGHT_MINUTE_EMPIRE_MAPLOADER_H