#include <iostream>
#include "BiddingFacility.h"

int main() {

  //each bidding facility belongs to a player
  vector <BiddingFacility> players;

  //create bidding facilities, with default bid as 0
  BiddingFacility bf1 = BiddingFacility(11, "johnson");
  players.push_back(bf1);
  BiddingFacility bf2 = BiddingFacility(11, "brown");
  players.push_back(bf2);
  BiddingFacility bf3 = BiddingFacility(11, "green");
  players.push_back(bf3);
//  BiddingFacility bf4 = BiddingFacility(11, "flynn");
//  players.push_back(bf4);
//  BiddingFacility bf5 = BiddingFacility(11, "square");
//  players.push_back(bf5);


  //show that player whose last name is first in alphabetical order wins bid,
  //when all bids are 0.

//  BiddingFacility *winner1 = nullptr; //set current bid winner as null
//  for (int i = 0; i < 3; i++) {
//    if (players[i].higherBid(winner1)) {
//      winner1 = &players[i];
//    }
//  }
//  cout << "Winner when all bids are 0: " << *winner1->getLastName() << endl;


  //Show that player with the highest bid is the winner.

  BiddingFacility *winner2 = nullptr; //set current bid winner as null
  //Loop enabling each player to enter their bid
  for (int i = 0; i < players.size(); i++) {
    cout << "\n" << *players[i].getLastName() << ", ";
    players[i].bid();
    cout << "\n";
  }

  for (int i = 0; i < players.size(); i++) {
    if (players[i].higherBid(winner2)) {
      winner2 = &players[i];
    }
  }
  winner2->subtractBid();
  cout << "Winner with highest bid: " << *winner2->getLastName() << endl
       << "Coins remaining: " << *winner2->getPlayerCoins() << endl;



  //Show that when players have equal bids, the player whose name is first in alphabetical order,
  //is the winner.

/*  BiddingFacility *winner3 = nullptr; //set current bid winner as null

  //set equal bids for each bidding facility, then find highest bid
  for (int i = 3; i < 5; i++) {
    int *bid = new int(6);
    players[i].setAmountBid(bid);
  }
  for (int i = 3; i < 5; i++) {
    if (players[i].higherBid(winner3)) {
      winner3 = &players[i];
    }
  }
  winner3->subtractBid();
  cout << "\nWinner when players have equal highest bids: " << *winner3->getLastName() << endl;
  cout << "Coins remaining: " << *winner3->getPlayerCoins() << endl;*/

  return 0;
}