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

Region* Utils::getRegionToBuild(Player *player) {
  return player->getRegionWithLeastCities();
}

Region* Utils::getRegionToPlace(Player *player) {
  return player->getRegionWithLeastArmies();
}