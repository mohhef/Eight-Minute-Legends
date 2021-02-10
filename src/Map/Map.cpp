//
// Created by Mohamed Ashraf on 2021-02-04.
//
#include "Map.h"
#include <list>
#include <stack>
#include <iostream>
#include <unordered_map>

Map::Map() {
    regions = new vector<pair<Region *, vector<pair<Region *, bool>>>>;
    continents = new vector<pair<Continent *, vector<Region *>>>;
}

Map::~Map(){
    delete regions;
    delete continents;
}

void Map::addContinent(Continent *continent) {
    continents -> push_back(make_pair(continent,vector<Region*>()));
}

void Map::addRegion(Region *region) {
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
    // if the continent doesnt exist create the continent and add it
//    vector<Region *> oneRegion = {region};
//    continents->push_back(make_pair(region->continent, oneRegion));
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

bool Map::isValid() {
    bool valid = areContinentsAndRegionsConnected();
    bool valid2 = eachRegionBelongsToOneContinent();
    return valid && valid2;
}

bool Map::eachRegionBelongsToOneContinent() {
    bool eachCountryBelongsToOneRegion = true;
    unordered_map<string, bool> uMap;
    for (int i = 0; i < (*regions).size(); i++) {
        string regionName = *((*regions)[i].first->name);
        if(uMap.find(regionName)==uMap.end()){
            uMap[regionName] = true;
        } else{
            uMap[regionName] = false;
        }
    }
    for (auto it = uMap.begin(); it != uMap.end(); ++it){
        if (it->second == false){
         cout << it->first << " has been added twice" << endl;
        }
    }
    return eachCountryBelongsToOneRegion;
}

bool Map::areContinentsAndRegionsConnected(){
    // initialize all regions to not visited (false)
    auto visitedRegions =  new vector<pair<Region *, bool>>;
    for (auto region: *(regions)) {
        visitedRegions->push_back(make_pair(region.first,false));
    }
    auto visitedContinents = new vector<pair<Continent *, bool>>;
    for (auto continent: *(continents)) {
        visitedContinents->push_back(make_pair(continent.first,false));
    }
    //uses a stack to iterate through the graph
    stack<Region*> stack;
    stack.push((*regions)[0].first);
    (*visitedRegions)[0].second = true;

    while(!stack.empty()){
        Region *region = stack.top();
        stack.pop();
        for(int i=0; i<visitedContinents->size(); i++){
            if((*visitedContinents)[i].first == region->continent){
                (*visitedContinents)[i].second = 1;

            }
        }
        vector<pair<Region*, bool>> *neighbourList = getNeighbourList(region);
        for(pair<Region*, bool> region: *neighbourList){
            for(int i=0; i<visitedRegions->size(); i++){
                if((*visitedRegions)[i].first == region.first && !(*visitedRegions)[i].second){
                    stack.push(region.first);
                    (*visitedRegions)[i].second = 1;
                    break;
                }
            }
        }
        delete neighbourList;
    }
    //Check if all regions are visited
    bool isContinentsConnected = true;
    for(pair<Continent *, bool> visitedContinent: *visitedContinents){
        if(!visitedContinent.second){
            isContinentsConnected= false;
        }
    }
    bool isRegionsConnected = true;
    for(pair<Region *, bool> visitedRegion: *visitedRegions){
        if(!visitedRegion.second){
            isRegionsConnected= false;
        }
    }
    cout << "Continents are " << ((isContinentsConnected)?"connected":"not connected") << endl;
    cout << "Regions are " << ((isRegionsConnected)?"connected":"not connected") << endl;
    return isContinentsConnected && isRegionsConnected;
}

vector<pair<Region *, bool>>* Map::getNeighbourList(Region *region){
    auto *neighbourList = new vector<pair<Region *, bool>>;
    for (int i = 0; i < regions->size(); i++) {
        if((*regions)[i].first == region){
            *neighbourList = (*regions)[i].second;
            break;
        }
    }
    return neighbourList;
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