#include <iostream>
#include "BiddingFacility.h"
#include "../MainDriver.h"

void Driver::part5(){

  //each bidding facility belongs to a player
  vector <BiddingFacility> players;

  //create bidding facilities, with default bid as 0
  BiddingFacility *bf1 = new BiddingFacility(11, "johnson");
  BiddingFacility *bf2 = new BiddingFacility(11, "brown");
  BiddingFacility *bf3 = new BiddingFacility(11, "green");
  players.push_back(*bf1);
  players.push_back(*bf2);
  players.push_back(*bf3);

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

  delete bf1;
  delete bf2;
  delete bf3;
}