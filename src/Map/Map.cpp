//
// Created by Mohamed Ashraf on 2021-02-04.
//

#include "Map.h"
#include <list>
#include <iomanip>
#include <iostream>

Map::Map(){
    regions = new vector< pair <Region*, vector<pair<Region*, bool>>>>;
    continents = new vector< pair <Continent*, vector<Region*>>>;
}

void Map::addRegion(Region *region){

    vector<pair<Region*, bool>> intiAdjacency;
    regions -> push_back(make_pair(region,intiAdjacency));
    string regionContinent = *(region->continent->name);

    //if the continent exits then add the country to that continent
    for(int i = 0; i < (*continents).size(); i++ ){
        string continentName = *((*continents)[i].first->name);
        if(continentName == regionContinent){
            (*continents)[i].second.push_back(region);
            return;
        }
    }

    // if the continent doesnt exist create the continent and add it
    vector<Region*> oneRegion = {region};
    continents->push_back(make_pair(region->continent,oneRegion));

}


void Map::addPath(Region *start, Region *destination, bool land) {

    for(int i = 0; i < (*regions).size(); i++){

        if((*regions)[i].first == start){
            (*regions)[i].second.push_back(make_pair(destination,land));
        }
        else if((*regions)[i].first == destination){
            (*regions)[i].second.push_back(make_pair(start,land));
        }
    }

}

void Map::displayMap(){
    printf ("\n%20s %s  \n\n","","********* Map Details *********");
    for(int i = 0 ; i < (*regions).size(); i++){
        string regionName = *((*regions)[i].first->name);
        string continentName = *((*regions)[i].first->continent->name);
        vector<pair<Region*, bool>> adjacentRegions = (*regions)[i].second;
//        vector<string> adjacentRegionsNames;
        printf("Region Name: %s %10s || Continent Name: %s %15s || Adjacent regions: %5s",
               "",regionName.c_str(),"", continentName.c_str(),"");
        for(int i=0 ; i<adjacentRegions.size(); i++){
            string connectionType = (adjacentRegions[i].second ==1)? "Land":"Water";
            cout << "{" <<*(adjacentRegions[i].first->name) << ": " << connectionType << "}" ;
//            adjacentRegionsNames.push_back(*(adjacentRegions[i].first->name));
        }
        cout << endl;
    }
};