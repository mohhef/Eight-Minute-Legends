//
// Created by Mohamed Ashraf on 2021-02-04.
//
#include "Map.h"
#include <list>
#include <stack>
#include <iostream>
#include <unordered_map>
#include <algorithm>

Map::Map() {
    regions = new vector<pair<Region *, vector<pair<Region *, bool>>>>;
    continents = new vector<pair<Continent *, vector<Region *>>>;
    startingRegion = nullptr;
}

Map::~Map() {
    delete regions;
    delete continents;
    delete startingRegion;
}

void Map::addContinent(Continent *continent) {
    continents->push_back(make_pair(continent, vector<Region *>()));
}

//starting region is false by default, if want to set starting set true
void Map::addRegion(Region *region, bool setStartingRegion) {
    if (setStartingRegion) {
        startingRegion = region;
    }
    vector<pair<Region *, bool>> intiAdjacency;
    regions->push_back(make_pair(region, intiAdjacency));
    string regionContinent = *(region->continent->name);

    //if the continent exits then add the country to that continent
    for (int i = 0; i < continents->size(); i++) {
        string continentName = *((*continents)[i].first->name);
        if (continentName == regionContinent) {
            (*continents)[i].second.push_back(region);
            return;
        }
    }
}

void Map::addPath(Region *start, Region *destination, bool land) {
    for (int i = 0; i < (*regions).size(); i++) {
        if ((*regions)[i].first == start) {
            (*regions)[i].second.push_back(make_pair(destination, land));
        } else if ((*regions)[i].first == destination) {
            (*regions)[i].second.push_back(make_pair(start, land));
        }
    }
}

// checks if two regions are adjacent: 1 = land, 0 = water, -1 not adjacent
int Map::isAdjacent(Region* start, Region* end){
    for(int i=0; i<regions->size(); i++){
        if((*regions)[i].first == start){
            vector<pair<Region*, bool>> adjacentRegions = (*regions)[i].second;
            for(i=0; i<adjacentRegions.size(); i++){
                if(adjacentRegions[i].first == end && adjacentRegions[i].second == 1){
                    return 1;
                }
                if (adjacentRegions[i].first == end && adjacentRegions[i].second == 0){
                    return 0;
                }
            }
        }
    }
    return -1;
}

bool Map::isValid() {
    bool valid1 = areRegionsConnected();
    bool valid2 = areContinentsConnected();
    bool valid3 = eachRegionBelongsToOneContinent();
    return valid1 && valid2 && valid3;
}

bool Map::eachRegionBelongsToOneContinent() {
    bool eachCountryBelongsToOneRegion = true;
    unordered_map<string, bool> uMap;
    for (int i = 0; i < (*regions).size(); i++) {
        string regionName = *((*regions)[i].first->name);
        if (uMap.find(regionName) == uMap.end()) {
            uMap[regionName] = true;
        } else {
            uMap[regionName] = false;
        }
    }
    for (auto it = uMap.begin(); it != uMap.end(); ++it) {
        if (it->second == false) {
            cout << it->first << " has been added more than once" << endl;
        }
    }
    return eachCountryBelongsToOneRegion;
}

bool Map::areContinentsConnected() {

    auto visitedContinents = new vector<pair<Continent *, bool>>;
    for (auto continent: *(continents)) {
        visitedContinents->push_back(make_pair(continent.first, false));
    }

    // initialize all regions to not visited (false)
    auto visitedRegions = new vector<pair<Region *, bool>>;
    for (auto region: *(regions)) {
        visitedRegions->push_back(make_pair(region.first, false));
    }

    stack<Region *> stack;
    vector<Region *> continentRegions = (*continents)[0].second;
    (*visitedContinents)[0].second = true;
    for (int i = 0; i < continentRegions.size(); i++) {
        stack.push(continentRegions[i]);
        for (int i = 0; i < visitedRegions->size(); i++) {
            if ((*visitedRegions)[i].first == continentRegions[i] && !(*visitedRegions)[i].second) {
                stack.push(continentRegions[i]);
                (*visitedRegions)[i].second = true;
                break;
            }
        }
    }

    while (!stack.empty()) {
        Region *region = stack.top();
        stack.pop();
        for (int i = 0; i < visitedRegions->size(); i++) {
            if ((*visitedRegions)[i].first == region && !(*visitedRegions)[i].second) {
                stack.push(region);
                (*visitedRegions)[i].second = true;
                break;
            }
        }
        vector<pair<Region *, bool>> *neighbourList = getNeighbourList(region);
        for (pair<Region *, bool> region: *neighbourList) {
            for (int i = 0; i < visitedRegions->size(); i++) {
                if ((*visitedRegions)[i].first == region.first && !(*visitedRegions)[i].second) {
                    for (int i = 0; i < visitedContinents->size(); i++) {
                        if ((*visitedContinents)[i].first == region.first->continent &&
                            !(*visitedContinents)[i].second) {
                            (*visitedContinents)[i].second = true;
                            break;
                        }
                    }
                    stack.push(region.first);
                    (*visitedRegions)[i].second = true;
                    break;
                }
            }
        }
    }
    //Check if all continents are visited
    bool isContinentsConnected = true;
    for (pair<Continent *, bool> visitedContinent: *visitedContinents) {
        if (!visitedContinent.second) {
            isContinentsConnected = false;
        }
    }
    cout << "Continents are " << ((isContinentsConnected) ? "connected" : "not connected") << endl;
    delete visitedContinents;
    delete visitedRegions;
    return isContinentsConnected;
}

bool Map::areRegionsConnected() {
    // initialize all regions to not visited (false)
    auto visitedRegions = new vector<pair<Region *, bool>>;
    for (auto region: *(regions)) {
        visitedRegions->push_back(make_pair(region.first, false));
    }

    //uses a stack to iterate through the graph
    stack<Region *> stack;
    stack.push((*regions)[0].first);
    (*visitedRegions)[0].second = true;

    while (!stack.empty()) {
        Region *region = stack.top();
        stack.pop();

        vector<pair<Region *, bool>> *neighbourList = getNeighbourList(region);
        for (pair<Region *, bool> region: *neighbourList) {
            for (int i = 0; i < visitedRegions->size(); i++) {
                if ((*visitedRegions)[i].first == region.first && !(*visitedRegions)[i].second) {
                    stack.push(region.first);
                    (*visitedRegions)[i].second = true;
                    break;
                }
            }
        }
        delete neighbourList;
    }

    bool isRegionsConnected = true;
    for (pair<Region *, bool> visitedRegion: *visitedRegions) {
        if (!visitedRegion.second) {
            isRegionsConnected = false;
        }
    }
    delete visitedRegions;
    cout << "Regions are " << ((isRegionsConnected) ? "connected" : "not connected") << endl;
    return isRegionsConnected;
}

vector<pair<Region *, bool>> *Map::getNeighbourList(Region *region) {
    auto *neighbourList = new vector<pair<Region *, bool>>;
    for (int i = 0; i < regions->size(); i++) {
        if ((*regions)[i].first == region) {
            *neighbourList = (*regions)[i].second;
            break;
        }
    }
    return neighbourList;
}

Continent *Map::findContinent(string continent){
  vector<Continent>::iterator i;
  for (auto it = continents->begin(); it != continents->end(); it++) {
    if (it->first->name->compare(continent) == 0) {
        return it->first;
    }
  }
  return nullptr;
}

void Map::displayMap() {
    printf("\n%20s %s  \n\n", "", "********* Map Details *********");
    for (int i = 0; i < regions->size(); i++) {
        string regionName = *((*regions)[i].first->name);
        string continentName = *((*regions)[i].first->continent->name);
        vector<pair<Region *, bool>> adjacentRegions = (*regions)[i].second;
        printf("Region Name: %s %10s || Continent Name: %s %15s || Adjacent regions: %5s",
               "", regionName.c_str(), "", continentName.c_str(), "");
        for (int i = 0; i < adjacentRegions.size(); i++) {
            string connectionType = (adjacentRegions[i].second == 1) ? "Land" : "Water";
            cout << "{" << *(adjacentRegions[i].first->name) << ": " << connectionType << "}";
        }
        cout << endl;
    }
};