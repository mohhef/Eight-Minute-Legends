//
// Created by Mohamed Ashraf on 2021-02-04.
//

#pragma once

#include <vector>
#include <string>
#include <iostream>

using namespace std;

struct Continent {
    string *name;

    Continent(string name) {
        this->name = new string(name);
    }

    Continent(const Continent &obj) {
        name = new string;
        *name = *obj.name;
    }

    Continent &operator=(const Continent &rhs) {
        if (this != &rhs)
            *name = *rhs.name;
        return *this;
    }

    ~Continent() {
        delete name;
    }
};

struct Region {
    string *name;
    Continent *continent;

    Region(string name, Continent *c) {
        this->name = new string(name);
        this->continent = c;
    }

    Region(const Region &obj) {
        name = new string;
        continent = new Continent(*(obj.continent));
        *name = *obj.name;
    }

    Region &operator=(const Region &rhs) {
        if (this != &rhs) {
            *continent = *(rhs.continent);
            *name = *rhs.name;
        }
        return *this;
    }

    ~Region() {
        delete name;
        delete continent;
    }
};

class Map {
public:

    // a vector that has a pair of region and all of its adjacent regions
    vector<pair<Region *, vector<pair<Region *, bool>>>> *regions;
    // a vector that has all the continents and its regions
    vector<pair<Continent *, vector<Region *>>> *continents;
    Region *startingRegion;

    Map();

    ~Map();

    Map &operator = (const Map &rhs);

    Map(const Map &obj);

    vector<pair<Region *, bool>> *getNeighbourList(Region *region);

    void addRegion(Region *region, bool setStartingRegion = false);

    void addContinent(Continent *continent);

    //use true if path is land, else false
    void addPath(Region *start, Region *destination, bool land);

    void displayMap();

    bool isValid();

    int isAdjacent(Region *start, Region *end);

    bool areRegionsConnected();

    bool areContinentsConnected();

    bool eachRegionBelongsToOneContinent();
};
