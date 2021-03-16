//
// Created by Mohamed Ashraf on 2021-02-27.
//

#include "Setup.h"

void Setup::loadGame() {
  // Prompt to let user select the file
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

void Setup::initializeDeck() {
  deck = new Deck();
  cout << "Top Board: " << endl;
  deck->showTopBoard();
}

void Setup::initializeBidding() {
  cout << "*********************Bidding Setup*********************" << endl;
  Player *winner = players->at(0);  // set current bid winner as the first player
  // Loop enabling each player to enter their bid
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
  cout << "Winner with highest bid: " << *winner->GetBiddingFacility()->getLastName()
       << endl
       << "Coins remaining: " << *winner->GetBiddingFacility()->getPlayerCoins() << endl;
  startingPlayer = winner;
}

void Setup::Startup() {
  cout << "*********************Startup Phase*********************" << endl;
  for (auto &player : *players) {
    player->PlaceNewArmies(4, map->startingRegion);
  }
  if (players->size() == 2) {
    cout << "Since there are only two players, each player takes turns placing one army "
            "at a time of a third non-player in any region on the board until ten armies "
            "have been placed."
         << endl;
    for (int i = 0; i < 10; i++) {
      int turn = i % 2;
      string region_name;
      Region *region = nullptr;
      while (!region) {
        cout << "[" << (*players)[turn]->GetName()
             << "'s turn] Pick a region in which one army for the non-player will be "
                "placed: ";
        cin >> region_name;
        region = map->findRegion(region_name);
        if (!region) {
          cout << "\"" << region_name << "\" does not exist. Try again." << endl;
        }
      }
      non_player->PlaceNewArmies(1, region, true);
    }
  }
}

int Setup::mainLoop() {
  int turn = 1;
  bool gameOver = false;
  int indexOfCurrentPlayer = 0;
  int playersSize = players->size();

  for (auto player : *players) {
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
  cout << "(" + player->GetName() +
              ") Choose a card by entering its position (1-6) or enter any other number "
              "to skip:"
       << endl;
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
