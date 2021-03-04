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
