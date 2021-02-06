//
// Created by Mohamed Ashraf on 2021-02-04.
//
#include "Map.h"
#include <list>
#include <stack>
#include <iostream>

Map::Map() {
    regions = new vector<pair<Region *, vector<pair<Region *, bool>>>>;
    continents = new vector<pair<Continent *, vector<Region *>>>;
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
    bool valid = areRegionsConnected() && areContinentsConnected();
    return valid;
}

bool Map::areRegionsConnected() {
    bool isConnected = true;
    // initialize all regions to not visited (false)
    auto visitedRegions =  new vector<pair<Region *, bool>>;
    for (auto region: *(regions)) {
        visitedRegions->push_back(make_pair(region.first,false));
    }

    //uses a stack to iterate through the graph
    stack<Region*> stack;
    stack.push((*regions)[0].first);
    (*visitedRegions)[0].second = true;

    while(!stack.empty()){
        Region *region = stack.top();
        stack.pop();

        vector<pair<Region*, bool>> *neighbourList = getNeighbourList(region);
        for(pair<Region*, bool> region: *neighbourList){
            for(int i=0; i<visitedRegions->size(); i++){
//            for(pair<Region *, bool> visitedRegion: visitedRegions){
                if((*visitedRegions)[i].first == region.first && !(*visitedRegions)[i].second){
                    stack.push(region.first);
                    (*visitedRegions)[i].second = 1;
                    break;
                }
            }
//            }
        }
    }
    //Check if all regions are visited
    for(pair<Region *, bool> visitedRegion: *visitedRegions){
        if(!visitedRegion.second){
            isConnected= false;
        }
    }
    cout << isConnected << endl;
    return isConnected;
}

bool Map::areContinentsConnected(){
    bool isConnected = true;
    // initialize all regions to not visited (false)
    auto visitedContinents = new vector<pair<Continent *, bool>>;
    for (auto continent: *(continents)) {
        visitedContinents->push_back(make_pair(continent.first,false));
    }

    stack<Region*> stack;
    stack.push((*regions)[0].first);
    (*visitedContinents)[0].second = true;

    while(!stack.empty()){
        Region *region = stack.top();
        stack.pop();

        vector<pair<Region*, bool>> *neighbourList = getNeighbourList(region);
        for(pair<Region*, bool> region: *neighbourList){
            for(int i=0; i<visitedContinents->size(); i++){
//            for(pair<Region *, bool> visitedRegion: visitedRegions){
                if((*visitedContinents)[i].first == region.first->continent && !(*visitedContinents)[i].second){
                    stack.push(region.first);
                    (*visitedContinents)[i].second = 1;
                    break;
                }
            }
//            }
        }
    }

    for(pair<Continent *, bool> visitedContinent: *visitedContinents){
        if(!visitedContinent.second){
            isConnected= false;
        }
    }
    cout << isConnected << endl;
    return isConnected;
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
//        vector<string> adjacentRegionsNames;
        printf("Region Name: %s %10s || Continent Name: %s %15s || Adjacent regions: %5s",
               "", regionName.c_str(), "", continentName.c_str(), "");
        for (int i = 0; i < adjacentRegions.size(); i++) {
            string connectionType = (adjacentRegions[i].second == 1) ? "Land" : "Water";
            cout << "{" << *(adjacentRegions[i].first->name) << ": " << connectionType << "}";
//            adjacentRegionsNames.push_back(*(adjacentRegions[i].first->name));
        }
        cout << endl;
    }
};