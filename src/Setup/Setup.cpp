//
// Created by Mohamed Ashraf on 2021-02-27.
//

#include "Setup.h"

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

void Setup::addArmy(Player &player, int *count) {
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

void Setup::moveOverLand(Player &player, int *count) {
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

void Setup::moverOverLandOrWater(Player &player, int *count) {
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
  bool executed = player.MoveArmies(armiesNum, from, to);
  if (executed) {
    *count -= armiesNum;
  }
}

void Setup::buildCity(Player &player, int *count) {
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

void Setup::destroyArmy(Player &player, int *count) {
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

Player *Setup::findPlayer(string playerName) {
  vector<Player *>::iterator p;
  for (p = (players)->begin(); p != (players)->end(); ++p) {
    if ((*p)->GetName() == playerName) {
      return *p;
    }
  }
  return nullptr;
}