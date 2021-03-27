//
// Created by Mohamed Ashraf on 2021-02-27.
//

#include <string.h>
#include <stdio.h>
#include <sstream>
#include <limits>
#include "Setup.h"
using namespace std;

Setup::Setup() {
  this->players = nullptr;
  this->map = nullptr;
  this->deck = nullptr;
  this->starting_player = nullptr;
  this->non_player = nullptr;
}

/*
 * Destructor
 */
Setup::~Setup() {
  for (int i = 0; i < players->size(); i++) {
    delete (*players)[i];
  }
  players->clear();
  delete players;
  delete map;
  delete deck;
  delete starting_player;
  delete non_player;
}

/*
 * Copy constructor
 */
Setup::Setup(const Setup &setup) {
  deepCopy(setup);
}

/*
 * Assignment Operator
 */
Setup &Setup::operator=(Setup rhs) {
  if (this != &rhs) {
    this->~Setup();
    deepCopy(rhs);
  }
  return *this;
}

/*
 * Deep copy
*/
void Setup::deepCopy(const Setup &obj) {
  for (int i = 0; i < obj.players->size(); i++) {
    this->players->push_back(new Player(*obj.players->at(i)));
  }
  this->map = new Map(*obj.map);
  this->deck = new Deck(*obj.deck);
  this->starting_player = new Player(*obj.starting_player);
  this->non_player = new Player(*obj.non_player);
}

/*
 * Stream insertion operator
 */
ostream &operator<<(ostream &os, const Setup &setup) {
  os << "*********************Game Setup*********************" << endl;
  if (setup.map != nullptr) {
    cout << *setup.map << endl;
  }
  if (setup.deck != nullptr) {
    cout << *setup.deck << endl;
  }
  if (setup.players != nullptr) {
    for (int i = 0; i < setup.players->size(); i++) {
      cout << *setup.players->at(i) << endl;
    }
  }
  os << "Starting player: " << endl;
  if (setup.starting_player != nullptr) {
    cout << *(setup.starting_player) << endl;
  }
  os << "Non player: " << endl;
  if (setup.non_player != nullptr) {
    cout << *(setup.non_player) << endl;
  }
  os << "******************************************************" << endl;
  return os;
}

/*
 * Tokenizes a string according to a delimiter
 */
void tokenize(std::string const &str, const char delim,
              std::vector<std::string> &out) {
  // construct a stream from the string
  std::stringstream ss(str);

  std::string s;
  while (std::getline(ss, s, delim)) {
    out.push_back(s);
  }
}

/*
 * Loads the game map from a file
 */
void Setup::loadGame() {
  // Prompt to let user select the file
  cout << "*********************Map Loader*********************" << endl;
  MapLoader *mapLoader;
  map = mapLoader->chooseMap();
}

/*
 * Initializes all the game players
 */
void Setup::initializePlayers() {
  // initialize the players
  cout << "*********************Player Setup*********************" << endl;
  players = new vector<Player *>;
  int playerCount;
  while (playerCount != 2) {
    cin.clear();
    cin.ignore(256, '\n');
    cout << "Enter the number of players that would play(2 for two players)" << endl;
    cin >> playerCount;
  }
  for (int i = 0; i < playerCount; i++) {
    string playerName;
    cout << "Enter the player's name" << endl;
    cin >> playerName;
    Player *player = new Player(map, playerName, 18, 3, 14);
    players->push_back(player);
  }
  if (players->size() == 2) {
    non_player = new Player(map, "Non-player", 18, 3, 14);
  } else {
    non_player = nullptr;
  }
  for (auto player : *players) {
    cout << *player << endl;
  }
}

/*
 * Shuffle, initialize, remove necessary cards from the  deck
 */
void Setup::initializeDeck() {
  deck = new Deck();
  deck->shuffleDeck();
  deck->drawTopBoard();
  cout << "Top Board: " << endl;
  deck->showTopBoard();
}

void Setup::Startup() {
  cout << "\n*********************Startup Phase*********************" << endl;
  // Placement of player armies
  for (auto &player : *players) {
    player->PlaceNewArmies(4, map->startingRegion);
  }
  // Placement of non-player armies
  if (players->size() == 2) {
    cout << "Since there are only two players, each player takes turns placing one army\n"
            "at a time of a third non-player in any region on the board until ten armies\n"
            "have been placed." << endl;
    for (int i = 0; i < 10; i++) {
      int turn = i % 2;
      string region_name;
      Region *region = nullptr;
      while (!region) {
        cout << "[" << (*players)[turn]->GetName()
             << "'s turn] Pick a region in which one army for the non-player will be "
                "placed: " << endl;
        cin >> region_name;
        region = map->findRegion(region_name);
        if (!region) {
          cout << "\"" << region_name << "\" does not exist. Try again." << endl;
        }
      }
      non_player->PlaceNewArmies(1, region, true);
    }
  }
  // Start of the bidding
  cout << "Bidding has started..." << endl;
  Player *winner = players->at(0);
  for (auto &player : *players) {
    cout << "[" << player->GetName() << "'s turn] ";
    player->GetBiddingFacility()->bid();
  }
  for (auto &player : *players) {
    if (player->GetBiddingFacility()->higherBid(winner->GetBiddingFacility())) {
      winner = player;
    }
  }
  cout << "Winner with highest bid: " << winner->GetName() << endl;
  winner->PayCoin(*winner->GetBiddingFacility()->getAmountBid(), true);
  cout << "Coins remaining: " << *winner->GetBiddingFacility()->getPlayerCoins() << endl;
  starting_player = winner;
}

/*
 * Gives each player a turn to play the game and checks if the game is over
 */
int Setup::mainLoop() {
  int turn = 1;
  bool gameOver = false;
  int indexOfCurrentPlayer = 0;
  int playersSize = players->size();

  for (auto player : *players) {
    if (starting_player == player) {
      break;
    } else {
      indexOfCurrentPlayer++;
    }
  }
  cout << "*********************Game Start*********************" << endl;
  while (!gameOver) {
    takeTurn(players->at(indexOfCurrentPlayer), turn);
    turn++;
    indexOfCurrentPlayer = (indexOfCurrentPlayer + 1) % playersSize;
    gameOver = checkGameOver();
  }
  cout << "Current Player holdings" << endl;
  for (auto player : *players) {
    cout << *player << endl;
  }
  // Add code/method to compute game score here
  return 0;
}

/*
 * Lets the player choose from the available cards, picks up the card, pays the coins
 */
void Setup::takeTurn(Player *player, int turn) {
  int choiceIndex = 0;
  cout << "****** Turn #" + to_string(turn) + " ******" << endl;
  cout << "Current Player holdings" << endl;
  for (auto player : *players) {
    cout << *player << endl;
  }
  cout << "Top Board: " << endl;
  deck->showTopBoard();

  while (true) {
    cout << "(" + player->GetName() +
        ") Choose a card by entering its position (1-6) or enter any other number "
        "to skip:"
         << endl;
    cin >> choiceIndex;
    if (choiceIndex < 1 || choiceIndex > 6) {
      return;
    }
    int cardCost = deck->getBoardPositionCost(choiceIndex - 1);
    Cards *chosenCard = deck->getTopBoardCard(choiceIndex - 1);
    int playerCoins = player->GetCoins();
    if (cin.fail() || playerCoins < Deck::getBoardPositionCost(choiceIndex - 1)) {
      cin.clear();
      cin.ignore(256, '\n');
      cout << "Please enter a valid number or make sure you have enough coins:" << endl;
    } else {
      player->GetHand()->exchange(choiceIndex - 1, *deck);
      player->PayCoin(cardCost);
      int currentHandSize = player->GetHand()->getCurrentHandSize();
      string playerAction = player->GetHand()->getCard(currentHandSize - 1)->getAction();
      cout << player->GetName() + "'s action is: " + playerAction << endl;
      // Add code/method to take action and after action here
      andOrAction(*player, *chosenCard);
      break;
    }
  }
}

/*
 * According to the card's action the proper action method will be procceded with
 */
bool Setup::andOrAction(Player &player, Cards &card) {
  string choice;
  cout << "Selected Card: " << card << endl << "\n";
  cout << "To skip your turn enter: 0, or anything else to proceed" << endl;
  cin >> choice;
  if (choice == "0") {
    return false;
  }

  string cardAction = card.getAction();
  string abilityName;
  int *firstAbilityCount = new int(0);
  int *secondAbilityCount = new int(0);
  const char delim = '|';
  vector<string> tokenizedCardAbility;
  tokenize(cardAction, delim, tokenizedCardAbility);

  abilityName = tokenizedCardAbility.at(0);
  if (tokenizedCardAbility.size() >= 2) {
    *firstAbilityCount = stoi(tokenizedCardAbility.at(1));
  }
  if (tokenizedCardAbility.size() >= 3) {
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
      cout << "(" + player.GetName() + ")"
          + " Which action would you like to choose, 1 for the first, 2 for the second: " << cardAction << endl;
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
  delete firstAbilityCount;
  delete secondAbilityCount;
  return true;
}

/*
 * Action for adding armies
 */
void Setup::addArmy(Player &player, int *count) {
  while (*count > 0) {
    string regionName;
    int armiesNum;
    Region *region = nullptr;

    while (!region) {
      cout << "(" + player.GetName() + ")" + " Which region do you want to add armies in?: " << endl;
      cin >> regionName;
      region = map->findRegion(regionName);
    }
    while (true) {
      cout << "(" + player.GetName() + ")" + " Enter the number of armies you wish to add, remaining " << *count
           << endl;
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

/*
 * Action for moving over land or water
 */
void Setup::moverOverLandOrWater(Player &player, int *count) {
  while (*count > 0) {

    string regionFrom;
    string regionTo;
    int armiesNum;
    int remainingCount;
    Region *from = nullptr;
    Region *to = nullptr;

    while (!from) {
      cout << "(" + player.GetName() + ")" + " Enter region to move from: " << endl;
      cin >> regionFrom;
      from = map->findRegion(regionFrom);
    }

    while (!to) {
      cout << "(" + player.GetName() + ")" + " Enter region to move to: " << endl;
      cin >> regionTo;
      to = map->findRegion(regionTo);
    }

    while (true) {
      cout << "(" + player.GetName() + ")" + " Enter the number of armies you wish to move, remaining " << *count
           << endl;
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

/*
 * Action for building a city
 */
void Setup::buildCity(Player &player, int *count) {
  while (*count > 0) {

    string regionName;
    Region *region = nullptr;
    while (!region) {
      cout << "(" + player.GetName() + ")" + " Which region do you want to build city in?: ";
      cin >> regionName;
      region = map->findRegion(regionName);
    }
    bool executed = player.BuildCity(region);
    if (executed) {
      *count -= 1;
    }
  }
}

/*
 * Action for destroying armies
 */
void Setup::destroyArmy(Player &player, int *count) {
  while (*count > 0) {
    Player *targetPlayer = nullptr;
    string targetPlayerName;
    Region *region = nullptr;
    string targetCountry;
    while (!targetPlayer) {
      cout << "(" + player.GetName() + ")" + " Which player do you wish to destroy their army?: " << endl;
      cin >> targetPlayerName;
      targetPlayer = findPlayer(targetPlayerName);
    }
    while (!region) {
      cout << "(" + player.GetName() + ")" + " Which region do you want to destroy the player's army: ";
      cin >> targetCountry;
      region = map->findRegion(targetCountry);
    }

    bool executed = player.DestroyArmy(targetPlayer, region);
    if (executed) {
      *count -= 1;
    }
  }
}

/*
 * Finds a player in the game
 */
Player *Setup::findPlayer(string playerName) {
  vector<Player *>::iterator p;
  for (p = (players)->begin(); p != (players)->end(); ++p) {
    if ((*p)->GetName() == playerName) {
      return *p;
    }
  }
  return nullptr;
}

/*
 * Checks if the game is over by checking the hand sizes
 */
bool Setup::checkGameOver() {
  for (int i = 0; i < players->size(); ++i) {
    if (players->at(i)->GetHand()->getCurrentHandSize() != 13) {
      return false;
    }
  }
  return true;
}

/*
 * Computes the score of each player and determines the winner
 */
int Setup::computeScore() {
  /*
  VP_PER_DIRE, --> "Dire..."
  VP_PER_FLYING, --> ability is FLYING
  VP_PER_3_COINS, --> 3 coins = 1 vp
  VP_PER_FOREST,  --> "Forest..."
  VP_PER_ANCIENT, --> "Ancient..."
  VP_PER_CURSED, --> "Cursed..."
  VP_PER_ARCANE, --> "Arcane..."
  VP_PER_NIGHT--> "Night..."
  */
  for (auto player : *this->players) {
    int VP = 0;
    vector<Cards *> *handCards = player->GetHand()->getHandCards();
    for (auto pcard : *handCards) {
      if (!pcard->getAbility().compare("VP_PER_DIRE")) {
        VP += player->countHandCardNameStartsWith("Dire");
      } else if (!pcard->getAbility().compare("VP_PER_FLYING")) {
        VP += player->countHandCardAbilityEquals("FLYING");
      } else if (!pcard->getAbility().compare("VP_PER_3_COINS")) {
        VP += player->GetCoins() / 3;
      } else if (!pcard->getAbility().compare("VP_PER_FOREST")) {
        VP += player->countHandCardNameStartsWith("Forest");
      } else if (!pcard->getAbility().compare("VP_PER_ANCIENT")) {
        VP += player->countHandCardNameStartsWith("Ancient");
      } else if (!pcard->getAbility().compare("VP_PER_CURSED")) {
        VP += player->countHandCardNameStartsWith("Cursed");
      } else if (!pcard->getAbility().compare("VP_PER_ARCANE")) {
        VP += player->countHandCardNameStartsWith("Arcane");
      } else if (!pcard->getAbility().compare("VP_PER_NIGHT")) {
        VP += player->countHandCardNameStartsWith("Night");
      }
    }
    player->SetScore(VP);
  }

  std::map<Player *, int> player_has_region;
  std::map<Player *, int> player_has_continent;
  /*
  Regions: A player gains one victory point for each region on the map he controls.
  A player controls a region if he has more armies there than any other player
  (cities count as armies when determining control).
  If players have the same number of armies in a region, no one controls it.
  */
  auto continents = map->continents;
  vector<pair<Region *, Player *>> region_control;
  for (auto it = continents->begin(); it != continents->end(); it++) {
    Continent *continent = it->first;
    auto regions = it->second;
    for (auto reg : regions) {
      Player *owner = nullptr;
      int max_ctrl_power = -1;
      for (auto player : *this->players) {
        int armies = player->GetArmiesInRegion(reg)->second;
        int cities = player->GetCitiesInRegion(reg)->second;
        //(cities count as armies when determining control).
        int controls = armies + cities;
        if (controls > max_ctrl_power) {
          max_ctrl_power = controls;
          owner = player;
        } else if (controls == max_ctrl_power) {
          // If players have the same number of armies in a region, no one controls it.
          owner = nullptr;
        }
      }
      region_control.push_back(make_pair(reg, owner));
      if (owner) {
        owner->SetScore(owner->GetScore() + 1);
        player_has_region[owner] += 1;
      }
    }
  }
  /*
  Continents: A player gains one victory point for each continent he controls.
  A player controls a continent if he controls more regions in the continent than anyone else.
  If players are tied for controlled regions, no one controls the continent.
  */
  for (auto it = continents->begin(); it != continents->end(); it++) {
    auto regions = it->second;
    int max_ctrl_power = -1;
    Player *continent_owner = nullptr;
    std::map<Player *, int> counter;
    for (auto reg : regions) {
      Player *reg_owner = nullptr;
      for (auto ctinfo : region_control) {
        if (ctinfo.first == reg) {
          reg_owner = ctinfo.second;
          break;
        }
      }
      if (reg_owner) {
        if (counter.count(reg_owner) >= 1) {
          counter[reg_owner] += 1;
        } else {
          counter[reg_owner] = 1;
        }
        if (counter[reg_owner] > max_ctrl_power) {
          max_ctrl_power = counter[reg_owner];
          continent_owner = reg_owner;
        } else if (counter[reg_owner] == max_ctrl_power) {
          // If players have the same number of armies in a region, no one controls it.
          continent_owner = nullptr;
        }
      }
    }
    if (continent_owner) {
      continent_owner->SetScore(continent_owner->GetScore() + 1);
      player_has_continent[continent_owner] += 1;
    }
  }

  Player *winner = nullptr;
  int max_score = -1;
  cout << "PlayerName\tScores" << endl;
  for (auto player : *this->players) {
    cout << player->GetName() << "\t" << player->GetScore() << endl;
    if (player->GetScore() > max_score) {
      max_score = player->GetScore();
      winner = player;
    } else if (player->GetScore() == max_score) {
      winner = nullptr;
    }
  }
  if (winner) {
    cout << "Winner:" << winner->GetName() << endl;
    return winner->GetScore();
  }
  max_score = -1;
  //If players are tied, the player with the most coins wins.
  for (auto player : *this->players) {
    cout << player->GetName() << "\t" << player->GetScore() << endl;
    if (player->GetCoins() > max_score) {
      max_score = player->GetCoins();
      winner = player;
    } else if (player->GetCoins() == max_score) {
      winner = nullptr;
    }
  }
  if (winner) {
    cout << "Winner(has most coins):" << winner->GetName() << endl;
    return winner->GetScore();
  }
  max_score = -1;
  // If still tied, the player with the most armies on the board wins
  for (auto player : *this->players) {
    int total_armies = 0;
    for (auto e : *player->GetArmies()) {
      total_armies += e.second;
    }
    if (total_armies > max_score) {
      max_score = total_armies;
      winner = player;
    } else if (total_armies == max_score) {
      winner = nullptr;
    }
  }
  if (winner) {
    cout << "Winner(has most armies):" << winner->GetName() << endl;
    return winner->GetScore();
  }
  max_score = -1;
  //  If still tied, the player with the most controlled regions wins.
  for (auto e : player_has_region) {
    if (e.second > max_score) {
      max_score = e.second;
      winner = e.first;
    } else if (e.second == max_score) {
      winner = nullptr;
    }
  }
  if (winner) {
    cout << "Winner(has most regions):" << winner->GetName() << endl;
    return winner->GetScore();
  }
  cout << "Tie!!" << endl;
  return 0;
}