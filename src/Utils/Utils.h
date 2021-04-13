#pragma once
#include <string>
#include <sstream>
#include <vector>

#include "../Player/Player.h"

class Utils{
public:
    static void tokenize(std::string const &str, const char delim, std::vector<std::string> &out);
    static string getActionName(string cardAction);
    static Player* findPlayer(string playerName, vector<Player *> *players);
    static Player* getPlayerWithMostArmies(string selfName, vector<Player *> *players);
    static Region* getRegionWithMostArmies(Player *player);
    static Region* getRegionToBuild(Player *player);
    static Region* getRegionToPlace(Player *player);
};