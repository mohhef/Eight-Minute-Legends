#include "Utils.h"

/*
 * Tokenizes a string according to a delimiter
 */
void Utils::tokenize(std::string const &str, const char delim,
              std::vector<std::string> &out) {
  // construct a stream from the string
  std::stringstream ss(str);

  std::string s;
  while (std::getline(ss, s, delim)) {
    out.push_back(s);
  }
};

string Utils::getActionName(string cardAction) {
  const char delim = '|';
  vector<string> tokenizedCardAbility;
  tokenize(cardAction, delim, tokenizedCardAbility);
  return tokenizedCardAbility.at(0);
}

/*
 * Finds a player in the game
 */
Player* Utils::findPlayer(string playerName, vector<Player *> *players) {
  vector<Player *>::iterator p;
  for (p = (players)->begin(); p != (players)->end(); ++p) {
    if ((*p)->GetName() == playerName) {
      return *p;
    }
  }
  return nullptr;
}

/*
 * Finds the top player of the game
 */
Player* Utils::getPlayerWithMostArmies(string selfName, vector<Player *> *players) {
  Player *topPlayer = nullptr;
  int topArmyCount = 0;
  vector<Player *>::iterator p;
  for (p = (players)->begin(); p != (players)->end(); ++p) {
    if ((*p)->GetName() == selfName) {
      continue;
    }
    if ((*p)->getArmyCount() > topArmyCount) {
      topPlayer = *p;
      topArmyCount = (*p)->getArmyCount();
    }
  }
  return topPlayer;
}

Region* Utils::getRegionWithMostArmies(Player *player) {
  return player->getRegionWithMostArmies();
}

Region* Utils::getRegionWithLeastArmies(Player *player) {
  return player->getCityRegionWithLeastArmies();
}

Region* Utils::getRegionToBuild(Player *player) {
  vector<Region*> cityRegions = player->getRegionsWithArmies();
  Region *minCityRegion = nullptr;
  int minCities = 4;
  for (int i = 0; i < cityRegions.size(); ++i) {
    int tempMinCities = getCitiesInRegion(player, cityRegions.at(i));
    if (tempMinCities < minCities) {
      minCityRegion = cityRegions.at(i);
      minCities = tempMinCities;
    }
  }
  if (minCityRegion == nullptr) {
    minCityRegion = player->GetMap()->startingRegion;
  }
  return minCityRegion;
}

Region* Utils::getRegionToPlace(Player *player) {
  return player->getRegionWithLeastArmies();
}

Region* Utils::getRegionToDestroy(string selfName, vector<Player *> *players) {
  Player *player = findPlayer(selfName, players);
  Player *otherPlayer = getPlayerWithMostArmies(selfName, players);
  vector<Region*> cityRegions = player->getRegionsWithArmies();
  for (int i = 0; i < cityRegions.size(); ++i) {
    if (getArmiesInRegion(otherPlayer, cityRegions.at(i)) > 0) {
      return cityRegions.at(i);
    }
  }
  return nullptr;
}

int Utils::getArmiesInRegion(Player *player, Region *region) {
  return player->GetArmiesInRegion(region)->second;
}

int Utils::getCitiesInRegion(Player *player, Region *region) { 
  return player->GetCitiesInRegion(region)->second;
}