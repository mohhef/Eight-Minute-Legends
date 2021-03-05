//
// Created by Mohamed Ashraf on 2021-02-27.
//

#include <string.h>
#include <stdio.h>
#include <sstream>
#include "Setup.h"

void tokenize(std::string const &str, const char delim,
              std::vector<std::string> &out) {
  // construct a stream from the string
  std::stringstream ss(str);

  std::string s;
  while (std::getline(ss, s, delim)) {
    out.push_back(s);
  }
}

int Setup::startGame() {
  //Prompt to let user select the file
  cout << "*********************Map Loader*********************" << endl;
  MapLoader *mapLoader;
  map = mapLoader->chooseMap();

  // initialize the players
  cout << "*********************Player Setup*********************" << endl;
  players = new vector<Player *>;
  int playerCount;
  while (playerCount != 2) {
    cout << "Enter the number of players that would play" << endl;
    cout << "Select 2 to start the game with two players" << endl;
    cin >> playerCount;
  }
  for (int i = 0; i < playerCount; i++) {
    string playerName;
    cout << "Enter the player's name" << endl;
    cin >> playerName;
    Player *player = new Player(map, playerName, 18, 3, 12);
    players->push_back(player);

  }

  deck = new Deck();
  cout << "Top Board: " << endl;
  deck->showTopBoard();

  Player *winner = players->at(0); //set current bid winner as the first player
  //Loop enabling each player to enter their bid
  for (int i = 0; i < players->size(); i++) {
    cout << "\n" << *players->at(i)->GetBiddingFacility()->getLastName() << ", ";
    players->at(i)->GetBiddingFacility()->bid();
    cout << "\n";
  }
  for (int i = 0; i < players->size(); i++) {
    if (players->at(i)->GetBiddingFacility()->higherBid(winner->GetBiddingFacility())) {
      winner = players->at(i);
    }
  }
  winner->GetBiddingFacility()->subtractBid();
  cout << "Winner with highest bid: " << *winner->GetBiddingFacility()->getLastName() << endl
       << "Coins remaining: " << *winner->GetBiddingFacility()->getPlayerCoins() << endl;
  startingPlayer = winner;
  return 0;
};

bool Setup::andOrAction(Player &player, Cards &card) {
  string choice;
  cout << "Selected Card: " << card << endl;

  cout << "To skip your turn enter: 0, or anything else to proceed" << endl;
  if(choice == "0"){
    return false;
  }

  string cardAbility = card.getAbility();
  string abilityName;
  int *firstAbilityCount = new int(0);
  int *secondAbilityCount = new int(0);
  const char delim = '|';
  vector<string> tokenizedCardAbility;
  tokenize(cardAbility, delim, tokenizedCardAbility);

  abilityName = tokenizedCardAbility.at(0);
  if (tokenizedCardAbility.size() == 2) {
    *firstAbilityCount = stoi(tokenizedCardAbility.at(1));
  } else if (tokenizedCardAbility.size() == 3) {
    *secondAbilityCount = stoi(tokenizedCardAbility.at(2));
  }

  if (abilityName == "BUILD_CITY") {
    buildCity(player, new int(1));
  } else if (abilityName == "MOVE_ARMIES") {
    moverOverLandOrWater(player, firstAbilityCount);
  } else if (abilityName == "PLACE_ARMIES") {
    addArmy(player, firstAbilityCount);
  } else if (abilityName == "MOVE_ARMIES_AND_BUILD_CITY") {
    moverOverLandOrWater(player, firstAbilityCount);
    buildCity(player, new int(1));
  } else if (abilityName == "BUILD_CITY_AND_PLACE_ARMIES") {
    buildCity(player, new int(1));
    addArmy(player, firstAbilityCount);
  } else if (abilityName == "MOVE_ARMIES_AND_PLACE_ARMIES") {
    moverOverLandOrWater(player, firstAbilityCount);
    addArmy(player, secondAbilityCount);
  } else if (abilityName == "MOVE_ARMIES_AND_DESTROY_ARMIES") {
    addArmy(player, firstAbilityCount);
    destroyArmy(player, secondAbilityCount);
  } else if (abilityName == "PLACE_ARMIES_AND_DESTROY_ARMIES") {
    addArmy(player, firstAbilityCount);
    destroyArmy(player, secondAbilityCount);
  } else {

    int actionChoiceIndex;
    while (true) {
      cout << "Which action would you like to choose, 1 for the first, 2 for the second: " << cardAbility << endl;
      cin >> actionChoiceIndex;
      if (actionChoiceIndex > 2 || actionChoiceIndex < 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please enter a valid number." << endl;
      } else {
        break;
      }
    }

    if (abilityName == "PLACE_ARMIES_OR_BUILD_CITY") {
      if (actionChoiceIndex == 1) addArmy(player, firstAbilityCount);
      else buildCity(player, new int(1));
    }
    if (abilityName == "PLACE_ARMIES_OR_MOVE_ARMIES") {
      if (actionChoiceIndex == 1) addArmy(player, firstAbilityCount);
      else moverOverLandOrWater(player, secondAbilityCount);
    }
  }
  return true;
}

void Setup::addArmy(Player &player, int *count) {
  while (*count > 0) {
    string regionName;
    int armiesNum;
    Region *region = nullptr;

    while (!region) {
      cout << "Which region do you want to add armies in?: " << endl;
      cin >> regionName;
      region = map->findRegion(regionName);
    }
    while (true) {
      cout << "Enter the number of armies you wish to add";
      cin >> armiesNum;
      if (armiesNum > *count || armiesNum <= 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please enter a valid number." << endl;
      } else {
        break;
      }
    }
    bool executed = player.PlaceNewArmies(armiesNum, region);
    if (executed) {
      *count -= armiesNum;
    }
  }
}

void Setup::moveOverLand(Player &player, int *count) {
  while (*count > 0) {
    string regionFrom;
    string regionTo;
    int armiesNum;
    Region *from = nullptr;
    Region *to = nullptr;

    while (!from) {
      cout << "Enter region to move from: ";
      cin >> regionFrom;
      from = map->findRegion(regionFrom);
    }
    while (!to) {
      cout << "Name country to move armies to: ";
      cin >> regionTo;
      to = map->findRegion(regionTo);
    }
    while (true) {
      cout << "Enter the number of armies you wish to add";
      cin >> armiesNum;
      if (armiesNum > *count || armiesNum <= 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please enter a valid number." << endl;
      } else {
        break;
      }
    }
    bool executed = player.MoveOverLand(armiesNum, from, to);
    if (executed) {
      *count -= armiesNum;
    }
  }
}

void Setup::moverOverLandOrWater(Player &player, int *count) {
  while (*count > 0) {

    string regionFrom;
    string regionTo;
    int armiesNum;
    int remainingCount;
    Region *from = nullptr;
    Region *to = nullptr;

    while (!from) {
      cout << "Enter region to move from: ";
      cin >> regionFrom;
      from = map->findRegion(regionFrom);
    }
    while (true) {
      cout << "Enter the number of armies you wish to add";
      cin >> armiesNum;
      int adjacency = map->isAdjacent(from, to);
      if (adjacency == 1) {
         remainingCount = *count - armiesNum;
      } else if (adjacency == 0) {
         remainingCount = *count - 3 * armiesNum;
      }
      if (armiesNum > *count || armiesNum <= 0 || remainingCount < 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        remainingCount = 0;
        cout << "Please enter a valid number." << endl;
      } else {
        break;
      }
    }
    bool executed = player.MoveArmies(armiesNum, from, to);
    if (executed) {
      *count = remainingCount;
    }
  }
}

void Setup::buildCity(Player &player, int *count) {
  while (*count > 0) {

    string regionName;
    Region *region = nullptr;
    while (!region) {
      cout << "Which region do you want to add armies in?:";
      cin >> regionName;
      region = map->findRegion(regionName);
    }
    bool executed = player.BuildCity(region);
    if (executed) {
      *count -= 1;
    }
  }
}

void Setup::destroyArmy(Player &player, int *count) {
  while (*count > 0) {
    Player *targetPlayer = nullptr;
    string targetPlayerName;
    Region *region = nullptr;
    string targetCountry;
    while (!targetPlayer) {
      cout << "Which player do you wish to destroy their army?: ";
      cin >> targetPlayerName;
      targetPlayer = findPlayer(targetPlayerName);
    }
    while (!region) {
      cout << "Which region do you want to destroy the player's army";
      cin >> targetCountry;
      region = map->findRegion(targetCountry);
    }

    bool executed = player.DestroyArmy(targetPlayer, region);
    if (executed) {
      count -= 1;
    }
  }
}

Player *Setup::findPlayer(string playerName) {
  vector<Player *>::iterator p;
  for (p = (players)->begin(); p != (players)->end(); ++p) {
    if ((*p)->GetName() == playerName) {
      return *p;
    }
  }
  return nullptr;
}
