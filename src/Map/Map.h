//
// Created by Mohamed Ashraf on 2021-02-04.
//

#pragma once

#include <vector>
#include <string>

using namespace std;

struct Continent {
    string *name;
    Continent(string name) {
        this -> name = new string(name);
    }
    ~Continent() {
        delete name;
    }
};

struct Region {
    string *name;
    Continent *continent;
    Region(string name, Continent *c){
        this->name = new string(name);
        this->continent = c;
    }
    ~Region() {
        delete name;
        delete continent;
    }
};

class Map {
public:
    Map();
    ~Map();
    // a vector that has a pair of region and all of its adjacent regions
    vector< pair <Region*, vector<pair<Region*, bool>>>> *regions;
    // a vector that has all the continents and its regions
    vector< pair <Continent*, vector<Region*>>> *continents;
    vector<pair<Region *, bool>>* getNeighbourList(Region *region);
    Region *startingRegion;

    void addRegion(Region *region, bool setStartingRegion=false);
    void addContinent(Continent *continent);
    //use true if path is land, else false
    void addPath(Region *start, Region *destination,bool land);
    void displayMap();
    bool isValid();
    int isAdjacent(Region* start, Region* end);
    bool areRegionsConnected();
    bool areContinentsConnected();
    bool eachRegionBelongsToOneContinent();
};
