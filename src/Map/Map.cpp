//
// Created by Mohamed Ashraf on 2021-02-04.
//

#include "Map.h"
#include <list>
#include <iomanip>

Map::Map(){
    countries = new vector< pair <Region*, vector<pair<Region*, bool>>>>;
    continents = new vector< pair <Continent*, vector<Region*>>>;
}

void Map::addRegion(Region *region){

    vector<pair<Region*, bool>> intiAdjacency;
    countries -> push_back(make_pair(region,intiAdjacency));
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
