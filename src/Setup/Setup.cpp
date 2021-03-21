//
// Created by Mohamed Ashraf on 2021-02-27.
//

#include "Setup.h"
using namespace std;

Setup::Setup()
{
  this->players = nullptr;
  this->map = nullptr;
  this->deck = nullptr;
  this->startingPlayer = nullptr;
}


void Setup::loadGame() {
  //Prompt to let user select the file
  cout << "*********************Map Loader*********************" << endl;
  MapLoader *mapLoader;
  map = mapLoader->chooseMap();
}

void Setup::initializePlayers() {
  // initialize the players
  cout << "*********************Player Setup*********************" << endl;
  players = new vector<Player *>;
  int playerCount;
  while (playerCount != 2) {
    cout << "Enter the number of players that would play(2 for two players)" << endl;
    cin >> playerCount;
  }
  for (int i = 0; i < playerCount; i++) {
    string playerName;
    cout << "Enter the player's name" << endl;
    cin >> playerName;
    Player *player = new Player(map, playerName, 18, 3, 12);
    players->push_back(player);
  }

  for (auto player : *players) {
    cout << *player << endl;
  }
}

void Setup::initializeDeck() {
  deck = new Deck();
  cout << "Top Board: " << endl;
  deck->showTopBoard();
}

void Setup::initializeBidding() {
  cout << "*********************Bidding Setup*********************" << endl;
  Player *winner = players->at(0); //set current bid winner as the first player
  //Loop enabling each player to enter their bid
  for (int i = 0; i < players->size(); i++) {
    cout << *players->at(i)->GetBiddingFacility()->getLastName() << ", ";
    players->at(i)->GetBiddingFacility()->bid();
    cout << "\n";
  }
  for (int i = 0; i < players->size(); i++) {
    if (players->at(i)->GetBiddingFacility()->higherBid(winner->GetBiddingFacility())) {
      winner = players->at(i);
    }
  }
  winner->GetBiddingFacility()->subtractBid();
  // TODO: We currently have two independent trackers for player coins.
  winner->PayCoin(*winner->GetBiddingFacility()->getAmountBid());
  cout << "Winner with highest bid: " << *winner->GetBiddingFacility()->getLastName() << endl
       << "Coins remaining: " << *winner->GetBiddingFacility()->getPlayerCoins() << endl;
  startingPlayer = winner;
}

int Setup::mainLoop() {
  int turn = 1;
  bool gameOver = false;
  int indexOfCurrentPlayer = 0;
  int playersSize = players->size();

  for (auto player: *players) {
    if (startingPlayer == player) {
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
  // Add code/method to compute game score here
  return 0;
}

void Setup::takeTurn(Player *player, int turn) {
  int choiceIndex = 0;
  cout << "****** Turn #" + to_string(turn) + " ******" << endl;
  cout << "Top Board: " << endl;
  deck->showTopBoard();
  cout << "(" + player->GetName()
      + ") Choose a card by entering its position (1-6) or enter any other number to skip:" << endl;
  cin >> choiceIndex;
  while (cin.fail() || choiceIndex < 1 || choiceIndex > 6) {
    cout << "Please enter a valid number:";
    cin.clear();
    cin.ignore(256, '\n');
    cin >> choiceIndex;
  }

  int cardCost = deck->getBoardPositionCost(choiceIndex - 1);
  int playerCoins = player->GetCoins();

  player->GetHand()->exchange(choiceIndex - 1, *deck, playerCoins);
  player->PayCoin(cardCost);
  int currentHandSize = player->GetHand()->getCurrentHandSize();
  string playerAction = player->GetHand()->getCard(currentHandSize - 1)->getAction();
  cout << player->GetName() + "'s action is: " + playerAction << endl;
  // Add code/method to take action and after action here
}

bool Setup::checkGameOver() {
  for (int i = 0; i < players->size(); ++i) {
    if (players->at(i)->GetHand()->getCurrentHandSize() == 11) {
      return true;
    }
  }
  return false;
}

int Setup::computeScore(){
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
    vector<Cards*>* handCards = player->GetHand()->getHandCards();
    for (auto pcard : *handCards) {
      if (!pcard->getAbility().compare("VP_PER_DIRE")) {
        VP += player->countHandCardNameStartsWith("Dire");
      }
      else if (!pcard->getAbility().compare("VP_PER_FLYING")) {
        VP += player->countHandCardAbilityEquals("FLYING");
      }
      else if (!pcard->getAbility().compare("VP_PER_3_COINS")) {
        VP += player->GetCoins() / 3;
      }
      else if (!pcard->getAbility().compare("VP_PER_FOREST")) {
        VP += player->countHandCardNameStartsWith("Forest");
      }
      else if (!pcard->getAbility().compare("VP_PER_ANCIENT")) {
        VP += player->countHandCardNameStartsWith("Ancient");
      }
      else if (!pcard->getAbility().compare("VP_PER_CURSED")) {
        VP += player->countHandCardNameStartsWith("Cursed");
      }
      else if (!pcard->getAbility().compare("VP_PER_ARCANE")) {
        VP += player->countHandCardNameStartsWith("Arcane");
      }
      else if (!pcard->getAbility().compare("VP_PER_NIGHT")) {
        VP += player->countHandCardNameStartsWith("Night");
      }
    }
    player->SetScore(VP);
    delete handCards;
  }

  std::map<Player*, int> player_has_region;
  std::map<Player*, int> player_has_continent;
  /*
  Regions: A player gains one victory point for each region on the map he controls.
  A player controls a region if he has more armies there than any other player
  (cities count as armies when determining control).
  If players have the same number of armies in a region, no one controls it.
  */
  auto continents = map->continents;
  vector<pair<Region*, Player*>> region_control;
  for (auto it = continents->begin(); it != continents->end(); it++) {
    Continent* continent = it->first;
    auto regions = it->second;
    for (auto reg : regions) {
      Player* owner = nullptr;
      int max_ctrl_power = -1;
      for (auto player : *this->players) {
        int armies = player->GetArmiesInRegion(reg)->second;
        int cities = player->GetCitiesInRegion(reg)->second;
        //(cities count as armies when determining control).
        int controls = armies + cities;
        if (controls > max_ctrl_power) {
          max_ctrl_power = controls;
          owner = player;
        }
        else if (controls == max_ctrl_power) {
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
    Player* continent_owner = nullptr;
    std::map<Player*, int> counter;
    for (auto reg : regions) {
      Player* reg_owner = nullptr;
      for (auto ctinfo : region_control) {
        if (ctinfo.first == reg) {
          reg_owner = ctinfo.second;
          break;
        }
      }
      if (reg_owner) {
        if (counter.count(reg_owner) > 1) {
          counter[reg_owner] += 1;
        }
        else {
          counter[reg_owner] = 1;
        }
        if (counter[reg_owner] > max_ctrl_power) {
          max_ctrl_power = counter[reg_owner];
          continent_owner = reg_owner;
        }
      }
    }
    if (continent_owner) {
      continent_owner->SetScore(continent_owner->GetScore() + 1);
      player_has_continent[continent_owner] += 1;
    }
  }

  Player* winner = nullptr;
  int max_score = -1;
  cout << "PlayerName\tScores" << endl;
  for (auto player : *this->players) {
    cout << player->GetName() << "\t" << player->GetScore() << endl;
    if (player->GetScore() > max_score) {
      max_score = player->GetScore();
      winner = player;
    }
    else if (player->GetScore() == max_score) {
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
    }
    else if (player->GetCoins() == max_score) {
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
    }
    else if (total_armies == max_score) {
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
    }
    else if (e.second == max_score) {
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