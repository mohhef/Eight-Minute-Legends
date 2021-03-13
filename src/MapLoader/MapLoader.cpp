
#include "MapLoader.h"
#include <fstream>
#include <iostream>
#include <cassert>

using namespace std;

/*
loads a map from a from a file
*/
Map *MapLoader::loadmap(string fileName) {
    // read file
    bool reading_regions = false;// read region
    bool reading_paths = false; //read path
    ifstream infile;
    infile.open(fileName);
    try {
        Map *map = new Map();
        string line;
        while (getline(infile, line)) {
            if (line.empty()) {
                //skip space line
                continue;
            }
            if (!line.compare("[Region]")) {
                //read region
                reading_regions = true;
                reading_paths = false;
                continue;
            }
            if (!line.compare("[Path]")) {
                //read Path status
                reading_paths = true;
                reading_regions = false;
                continue;
            }
            string first = line.substr(0, line.find(","));// part before ;
            string second = line.substr(line.find(",") + 1, line.length());//part that after ;
            assert(first != line && "Incorrect file format, check the delimiter");
            if (reading_regions) {
                //read region info, add region to the map
                try {
                    Continent *continent = new Continent(second);
                    if (map->findContinent(*continent->name) == nullptr) {
                        map->addContinent(continent);
                    }
                    Region *region = new Region(first, continent);
                    // add region to map
                    map->addRegion(region);
                }
                catch (const exception &e) {
                    cout << "Invalid region format: " << e.what() << endl;
                }
            } else if (reading_paths) {
                //reading the info about adjacency
                string type = second.substr(second.find(":") + 1, 1);
                // "1" for water，"0" for land
                string adjacency = second.substr(0, second.find(":"));
                if (type != "1" && type != "0") {
                    cout << "Each adjacency has to have a type of water or Land." << endl;
                    cout << "1 is for land, 0 is for water";
                    return nullptr;
                }
                try {
                    Region *start = 0, *end = 0;
                    int cnt = 0;
                    // find the pointer for regions，first and adjacency
                    for (auto it = map->regions->begin(); it != map->regions->end(); it++) {
                        if (it->first->name->compare(first) == 0) {
                            start = it->first;
                            cnt++;
                        }
                        if (it->first->name->compare(adjacency) == 0) {
                            end = it->first;
                            cnt++;
                        }
                        if (cnt >= 2) {
                            // find the beginning and ending
                            break;
                        }
                    }
                    // ensure countries have path
                    assert(start && "The regions is non existent");
                    assert(end && "The region is non existent");
                    // add the vector of Neighbour
                    map->addPath(start, end, type == "1");
                }
                catch (const exception &e) {
                    cout << "Invalid path format" << e.what() << endl;
                }
            } else {
                cout << "Invalid map format" << endl;
                return nullptr;
            }
        }
        if (map->isValid()) {
            // make sure if the map is valid
            return map;
        }
        cout << "The game's map is not valid, will not add the map" << endl;
        return nullptr;
    } catch (const exception &e) {
        cout << "Invalid map format" << e.what() << endl;
        infile.close();
    }
    return nullptr;
}

Map *MapLoader::chooseMap() {
    Map *loadMap = nullptr;
    while (loadMap == nullptr) {
        // Changed since the .exe might not run within MapLoader directory, hence ../Files might fail
        cout << "Choose a map, by entering the file path" << endl;
        string fileName;
        cin >> fileName;
        loadMap = loadmap(fileName);
    }
    cout << *loadMap;
    return loadMap;
}
