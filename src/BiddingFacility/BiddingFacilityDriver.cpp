#include <iostream>
#include "BiddingFacility.h"


void part5(){

  //each bidding facility belongs to a player
  vector <BiddingFacility> players;

  //create bidding facilities, with default bid as 0
  BiddingFacility bf1 = BiddingFacility(11, "johnson");
  players.push_back(bf1);
  BiddingFacility bf2 = BiddingFacility(11, "brown");
  players.push_back(bf2);
  BiddingFacility bf3 = BiddingFacility(11, "green");
  players.push_back(bf3);

  BiddingFacility *winner = nullptr; //set current bid winner as null
  //Loop enabling each player to enter their bid
  for (int i = 0; i < players.size(); i++) {
    cout << "\n" << *players[i].getLastName() << ", ";
    players[i].bid();
    cout << "\n";
  }

  for (int i = 0; i < players.size(); i++) {
    if (players[i].higherBid(winner)) {
      winner = &players[i];
    }
  }
  winner->subtractBid();
  cout << "Winner with highest bid: " << *winner->getLastName() << endl
       << "Coins remaining: " << *winner->getPlayerCoins() << endl;

}