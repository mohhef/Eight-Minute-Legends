//
// Created by Mohamed Ashraf on 2021-02-04.
//

#ifndef SRC_MAP_H
#define SRC_MAP_H

#include <vector>
#include <string>

using namespace std;

struct Continent {
private:
    string *name;
public:
    Continent(string name) {
        this -> name = new string(name);
    }
};

struct Region {
private:
    string *name;
    Continent *continent;
public:
    Region(string name, Continent *c){
        this->name = new string(name);
        this->continent = c;
    }
};

class Map {
    void addRegion(Region *region);
    //use true if path is land, else false
    void addPath(Region *start, Region *destination,bool land);
};


#endif //SRC_MAP_H
