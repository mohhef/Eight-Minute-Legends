//
// Created by Mohamed Ashraf on 2021-02-04.
//

#include "Map.h"
#include <list>
#include <iomanip>

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
