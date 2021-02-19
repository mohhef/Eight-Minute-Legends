//
// Created by Mohamed Ashraf on 2021-02-04.
//
#include "Map.h"
#include <list>
#include <stack>
#include <iostream>
#include <unordered_map>
#include <algorithm>

/*
 * Default constructor that initializes regions, continents and a starting region
*/
Map::Map() {
  regions = new vector<pair<Region *, vector<pair<Region *, bool>>>>;
  continents = new vector<pair<Continent *, vector<Region *>>>;
  startingRegion = nullptr;
}

/*
 * Destructor that deallocates all the pointer
 * The starting region and the content in regions vector are not deleted here,
 * because they are all deleted while looping through the continents
*/
Map::~Map() {
  for (auto continent: *continents) {
    for (auto region: continent.second) {
      delete region->name;
    }
    delete continent.first;
  }
  delete regions;
  delete continents;
}

/*
 * An assignment operator for the map object
*/
Map &Map::operator=(const Map &rhs) {
  if (this != &rhs) {
    this->~Map();
    deepCopy(rhs);
  }
  return *this;
}

/*
 * A copy constructor for the map object
*/
Map::Map(const Map &obj) {
  deepCopy(obj);
}

/*
deep copy
*/
void Map::deepCopy(const Map &obj) {
  startingRegion = new Region(*(obj.startingRegion));
  regions = new vector<pair<Region *, vector<pair<Region *, bool>>>>;
  continents = new vector<pair<Continent *, vector<Region *>>>;

  //copy continents
  for (int i = 0; i < obj.continents->size(); i++) {
    auto *continent = new Continent(*((*obj.continents)[i].first));
    vector<Region *> continentRegion;
    continents->push_back(make_pair(continent, continentRegion));
  }

  //copy regions
  for (int i = 0; i < obj.regions->size(); i++) {
    auto *continent = findContinent(*((*obj.regions)[i].first->continent->name));
    auto *region = new Region(*((*obj.regions)[i].first->name), continent);
    vector<pair<Region *, bool>> intiAdjacency;
    regions->push_back(make_pair(region, intiAdjacency));

  }

  //copy regions adjacency
  for (int i = 0; i < regions->size(); i++) {
    for (int j = 0; j < (*obj.regions)[i].second.size(); j++) {
      auto *adjacent = findRegion(*((*obj.regions)[i].second[j].first->name));
      bool type = (*obj.regions)[i].second[j].second;
      (*regions)[i].second.push_back(make_pair(adjacent, type));
    }
  }

  //copy the regions of the continents array
  for (int i = 0; i < continents->size(); i++) {
    for (int j = 0; j < (*obj.continents)[i].second.size(); j++) {
      auto *region = findRegion(*((*obj.continents)[i].second[j])->name);
      (*continents)[i].second.push_back(region);
    }
  }
}

/*
 * Stream insertion operator
 */
std::ostream &operator<<(ostream &output, Map &map) {
  map.displayMap();
  return output;
}

/*
 * Adds a continent to the map
 */
void Map::addContinent(Continent *continent) {
  continents->push_back(make_pair(continent, vector<Region *>()));
}

/*
 * Adds a region to the map
 * Starting region is false by default, if want to set starting set true
 */
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

/*
 * Add a path between a starting and destination region
 * 1 for land, 0 for water
 */
void Map::addPath(Region *start, Region *destination, bool land) {
  for (int i = 0; i < (*regions).size(); i++) {
    if ((*regions)[i].first == start) {
      (*regions)[i].second.push_back(make_pair(destination, land));
    } else if ((*regions)[i].first == destination) {
      (*regions)[i].second.push_back(make_pair(start, land));
    }
  }
}

/*
 * Checks if two regions are adjacent: 1 = land, 0 = water, -1 not adjacent
*/
int Map::isAdjacent(Region *start, Region *end) {
  for (int i = 0; i < regions->size(); i++) {
    if ((*regions)[i].first == start) {
      vector<pair<Region *, bool>> adjacentRegions = (*regions)[i].second;
      for (i = 0; i < adjacentRegions.size(); i++) {
        if (adjacentRegions[i].first == end && adjacentRegions[i].second == 1) {
          return 1;
        }
        if (adjacentRegions[i].first == end && adjacentRegions[i].second == 0) {
          return 0;
        }
      }
    }
  }
  return -1;
}

/*
Check if a map is valid by calling three functions
*/
bool Map::isValid() {
  bool valid1 = areRegionsConnected();
  bool valid2 = areContinentsConnected();
  bool valid3 = eachRegionBelongsToOneContinent();
  return valid1 && valid2 && valid3;
}

/*
 * Check if each region belongs to exactly one continent
*/
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
      eachCountryBelongsToOneRegion = false;
      return eachCountryBelongsToOneRegion;
    }
  }
  return eachCountryBelongsToOneRegion;
}

/*
 * Check if the continents are connected
*/
bool Map::areContinentsConnected() {

  // initialize all continents to not visited (false)
  auto visitedContinents = new vector<pair<Continent *, bool>>;
  for (auto continent: *(continents)) {
    visitedContinents->push_back(make_pair(continent.first, false));
  }

  // initialize all regions to not visited (false)
  auto visitedRegions = new vector<pair<Region *, bool>>;
  for (auto region: *(regions)) {
    visitedRegions->push_back(make_pair(region.first, false));
  }

  // get the regions in the first continent and set them as visited
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

  // look through all the other regions and see if it connected to the first initial continent
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

/*
 * Check if regions are connected
*/
bool Map::areRegionsConnected() {

  // initialize all regions to not visited (false)
  auto visitedRegions = new vector<pair<Region *, bool>>;
  for (auto region: *(regions)) {
    visitedRegions->push_back(make_pair(region.first, false));
  }

  stack<Region *> stack;
  stack.push((*regions)[0].first);
  (*visitedRegions)[0].second = true;

  //Loop through the whole graph to check if regions are connected using a stack
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
  //Check if regions are all visited
  for (pair<Region *, bool> visitedRegion: *visitedRegions) {
    if (!visitedRegion.second) {
      isRegionsConnected = false;
    }
  }
  delete visitedRegions;
  cout << "Regions are " << ((isRegionsConnected) ? "connected" : "not connected") << endl;
  return isRegionsConnected;
}

/*
Get the adjacent regions of a region
*/
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

/*
 * Returns a pointer to a continent after it searches for it
*/
Continent *Map::findContinent(string continent) {
  vector<Continent>::iterator i;
  for (auto it = continents->begin(); it != continents->end(); it++) {
    if (it->first->name->compare(continent) == 0) {
      return it->first;
    }
  }
  return nullptr;
}

/*
 * Returns a pointer to a region after it searches for it
*/
Region *Map::findRegion(string region) {
  vector<Continent>::iterator i;
  for (auto it = regions->begin(); it != regions->end(); it++) {
    if (it->first->name->compare(region) == 0) {
      return it->first;
    }
  }
  return nullptr;
}

/*Displays the map object*/
void Map::displayMap() {
  if (this != NULL) {
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
  } else {
    cout << "There was nothing added to the map";
  }
};