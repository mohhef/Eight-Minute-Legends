#include "PlayerStrategy.h"
#include "../Utils/Utils.h"
#include "Player.h"
#include <iostream>
#include <limits>
#include <string>
#include <sstream>

/*
Utility function to get the action count of a card
*/
int getActionCount(string cardAction) {
  const char delim = '|';
  vector<string> tokenizedCardAbility;
  Utils::tokenize(cardAction, delim, tokenizedCardAbility);
  
  string abilityName = tokenizedCardAbility.at(0);
  int firstAbilityCount = 0, secondAbilityCount = 0;
  if (tokenizedCardAbility.size() >= 2) {
    firstAbilityCount = stoi(tokenizedCardAbility.at(1));
  }
  // Build city. Give more weight to it
  else {
    firstAbilityCount = 2;
  }
  if (tokenizedCardAbility.size() >= 3) {
    secondAbilityCount = stoi(tokenizedCardAbility.at(2));
  }
  return firstAbilityCount + secondAbilityCount;
}

/*
Get the input for the region choice
*/
Region* getRegionInput(Map *map, string type) {
  string message = "";
  string regionName;
  Region *region = nullptr;
  if (type == "move_from") {
    message = " move from: ";
  }
  else if (type == "move_to") {
    message = " move to: ";
  }
  else {
    if (type == "destroy") {
      message = " destroy the player's army: ";
    }
    else if (type == "build") {
      message = " build a city in?:  ";
    }
    else if (type == "place"){
      message = " add armies in?: ";
    }
  }
  while (!region) {
    cout << "Which region do you want to" << message << endl;
    cin >> regionName;
    region = map->findRegion(regionName);
  }
  return region;
}

/*
Get the input for the army number choice
*/
int getArmiesInput(string type, int count) {
  int armiesNum = 0;
  while (true) {
    cout << "Enter the number of armies you wish to " << type << ", remaining " << count << endl;
    cin >> armiesNum;
    if (armiesNum > count || armiesNum <= 0) {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Please enter a valid number." << endl;
    } else {
      return armiesNum;
    }
  }
}

/*
The HumanStrategy is basically functions that expect input from a player
*/
int HumanStrategy::pickCard(Deck* deck, int playerCoins, string selfName, vector<Player *> *players) {
  int choiceIndex = 0;
  while (true) {
    cout << "(" << selfName << ") Choose a card by entering its position (1-6) or enter any other number "
        "to skip:"
         << endl;
    cin >> choiceIndex;
    
    int cardCost = deck->getBoardPositionCost(choiceIndex - 1);
    Cards *chosenCard = deck->getTopBoardCard(choiceIndex - 1);
    if (cin.fail() || playerCoins < Deck::getBoardPositionCost(choiceIndex - 1)) {
      cin.clear();
      cin.ignore(256, '\n');
      cout << "Please enter a valid number or make sure you have enough coins:" << endl;
    } else {
      return choiceIndex; 
    }
  }
}

int HumanStrategy::pickAction(string cardAction) {
  int actionChoiceIndex;
    while (true) {
      cout << "Which action would you like to choose, 1 for the first, 2 for the second: " 
           << cardAction << endl;
      cin >> actionChoiceIndex;
      if (actionChoiceIndex > 2 || actionChoiceIndex < 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please enter a valid number." << endl;
      } else {
        return actionChoiceIndex;
      }
    }
}

Region* HumanStrategy::pickRegion(Map *map, string type, string selfName, vector<Player *> *players) {
  return getRegionInput(map, type);
}

int HumanStrategy::pickArmies(string type, int count) {
  return getArmiesInput(type, count);
}

Player* HumanStrategy::pickPlayer(string selfName, vector<Player *> *players) {
  Player *targetPlayer = nullptr;
  string targetPlayerName = "";
  while (!targetPlayer) {
    cout << "Which player do you wish to destroy their army?: " << endl;
    cin >> targetPlayerName;
    if (targetPlayerName != selfName) {
      targetPlayer = Utils::findPlayer(targetPlayerName, players);
    }
  }
  return targetPlayer;
}

bool HumanStrategy::skipTurn() {
  string choice;
  cout << "To skip your turn enter: 0, or anything else to proceed" << endl;
  cin >> choice;
  if (choice == "0") {
    return true;
  }
  return false;
}

/*
Greedy AI to maximize greed by picking card that has the most greed points
AI chooses card at index 0 if it doesn't find a better one or no coins.
*/
int GreedyAIStrategy::pickCard(Deck* deck, int playerCoins, string selfName, vector<Player *> *players) {
  int maxGreed = 0, choiceIndex = 1;
  Player *tempPlayer = Utils::findPlayer(selfName, players);
  for (int i = 0; i < deck->getTopBoardSize(); ++i) {
    string cardAction = deck->getTopBoardCard(i)->getAction();
    string abilityName = Utils::getActionName(cardAction);
    if ((tempPlayer->GetDiscs() < 1 && abilityName.find("BUILD") != string::npos) ||
        (tempPlayer->GetCubes() < 1 && abilityName.find("PLACE") != string::npos)) {
      continue;
    }
    if (abilityName == "BUILD_CITY" || abilityName == "MOVE_ARMIES_AND_BUILD_CITY" ||
        abilityName == "BUILD_CITY_AND_PLACE_ARMIES" || abilityName == "MOVE_ARMIES_AND_DESTROY_ARMIES" ||
        abilityName == "PLACE_ARMIES_AND_DESTROY_ARMIES"|| abilityName == "PLACE_ARMIES_OR_BUILD_CITY" || 
        abilityName == "PLACE_ARMIES") {
      int totalGreed = getActionCount(cardAction);
      if (totalGreed > maxGreed && playerCoins >= deck->getBoardPositionCost(i)) {
        maxGreed = totalGreed;
        choiceIndex = i + 1;
      }
    }
  }
  return choiceIndex;
}

int GreedyAIStrategy::pickAction(string cardAction) {
  string abilityName = Utils::getActionName(cardAction);
  if (abilityName == "PLACE_ARMIES_OR_BUILD_CITY") {
    return 2;
  }
  else {
    return 1;
  }
}

Region* GreedyAIStrategy::pickRegion(Map *map, string type, string selfName, vector<Player *> *players) {
  return getRegionInput(map, type);
}

Player* GreedyAIStrategy::pickPlayer(string selfName, vector<Player*> *players) {
  return Utils::getPlayerWithMostArmies(selfName, players);
}

int GreedyAIStrategy::pickArmies(string type, int count) {
  return getArmiesInput(type, count);
}

bool GreedyAIStrategy::skipTurn() {
  return false;
}

/*
Moderate AI to maximize utility by focusing on having most armies in regions
AI chooses card at index 0 if it doesn't find a better one or has no coins.
*/
int ModerateAIStrategy::pickCard(Deck* deck, int playerCoins, string selfName, vector<Player *> *players) {
  int maxUtility = 0, choiceIndex = 1;
  string focus = "place";
  Player *tempPlayer = Utils::findPlayer(selfName, players);
  Region *maxRegion = Utils::getRegionWithMostArmies(tempPlayer);
  // If region is already heavily populated, move
  if(tempPlayer->GetArmiesInRegion(maxRegion)->second >= 8 && 
     tempPlayer->GetArmiesInRegion(tempPlayer->getRegionWithLeastArmies())->second >= 4) {
    focus = "move";
  }
  for (int i = 0; i < deck->getTopBoardSize(); ++i) {
    string cardAction = deck->getTopBoardCard(i)->getAction();
    string abilityName = Utils::getActionName(cardAction);
    if ((tempPlayer->GetDiscs() < 1 && abilityName.find("BUILD") != string::npos) ||
        (tempPlayer->GetCubes() < 1 && abilityName.find("PLACE") != string::npos)) {
      continue;
    }
    if (focus == "place" && (abilityName == "PLACE_ARMIES" || abilityName == "BUILD_CITY_AND_PLACE_ARMIES" ||
        abilityName == "PLACE_ARMIES_OR_BUILD_CITY" || abilityName == "PLACE_ARMIES_OR_MOVE_ARMIES" ||
        abilityName == "PLACE_ARMIES_AND_DESTROY_ARMIES" || abilityName == "BUILD_CITY")) {
      int totalUtility = getActionCount(cardAction);
      if (totalUtility > maxUtility && playerCoins >= deck->getBoardPositionCost(i)) {
        maxUtility = totalUtility;
        choiceIndex = i + 1;
      }
    }
    else if (focus == "move" && (abilityName == "MOVE_ARMIES" || abilityName == "MOVE_ARMIES_AND_BUILD_CITY" ||
             abilityName == "MOVE_ARMIES_AND_PLACE_ARMIES" || abilityName == "MOVE_ARMIES_AND_DESTROY_ARMIES" ||
             abilityName == "BUILD_CITY")) {
      int totalUtility = getActionCount(cardAction);
      if (totalUtility > maxUtility && playerCoins >= deck->getBoardPositionCost(i)) {
        maxUtility = totalUtility;
        choiceIndex = i + 1;
      }
    }
  }
  return choiceIndex;
}

int ModerateAIStrategy::pickAction(string cardAction) {
  // Prefer PLACE_ARMIES always
  return 1;
}

Region* ModerateAIStrategy::pickRegion(Map *map, string type, string selfName, vector<Player *> *players) {
  return getRegionInput(map, type);
}

Player* ModerateAIStrategy::pickPlayer(string selfName, vector<Player*> *players) {
  return Utils::getPlayerWithMostArmies(selfName, players);
}

int ModerateAIStrategy::pickArmies(string type, int count) {
  return getArmiesInput(type, count);
}

bool ModerateAIStrategy::skipTurn() {
  return false;
}