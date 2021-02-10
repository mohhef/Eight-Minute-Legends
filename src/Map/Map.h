//
// Created by Mohamed Ashraf on 2021-02-04.
//

#ifndef SRC_MAP_H
#define SRC_MAP_H

#include <vector>
#include <string>

using namespace std;

struct Continent {
    string *name;
    Continent(string name) {
        this -> name = new string(name);

    }
};

struct Region {
    string *name;
    Continent *continent;
    Region(string name, Continent *c){
        this->name = new string(name);
        this->continent = c;
    }
};

class Map {
public:
    Map();
    // a vector that has a pair of region and all of its adjacent regions
    vector< pair <Region*, vector<pair<Region*, bool>>>> *regions;

    // a vector that has all the continents and its regions
    vector< pair <Continent*, vector<Region*>>> *continents;

    vector<pair<Region *, bool>>* getNeighbourList(Region *region);

    void addRegion(Region *region);
    void addContinent(Continent *continent);
    //use true if path is land, else false
    void addPath(Region *start, Region *destination,bool land);
    void displayMap();
    bool isValid();
    bool areContinentsAndRegionsConnected();
    bool eachRegionBelongsToOneContinent();
};


#endif //SRC_MAP_H
