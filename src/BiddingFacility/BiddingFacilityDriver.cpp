#include <iostream>
#include "BiddingFacility.h"
#include "../Player/Player.h"
#include "../MainDriver.h"

void Driver::part5(){

  //each bidding facility belongs to a player
  vector <Player> playerObjs;

  //create bidding facilities, with default bid as 0
  auto *bf1 = new Player(11, "johnson");
  auto *bf2 = new Player(11, "brown");
  auto *bf3 = new Player(11, "green");
  playerObjs.push_back(*bf1);
  playerObjs.push_back(*bf2);
  playerObjs.push_back(*bf3);

  Player *winner = &playerObjs[0]; //set current bid winner as the first player
  //Loop enabling each player to enter their bid
  for (int i = 0; i < playerObjs.size(); i++) {
    cout << "\n" << *playerObjs[i].GetBiddingFacility()->getLastName() << ", ";
    playerObjs[i].GetBiddingFacility()->bid();
    cout << "\n";
  }

  for (int i = 0; i < playerObjs.size(); i++) {
    if (playerObjs[i].GetBiddingFacility()->higherBid(winner->GetBiddingFacility())) {
      winner = &playerObjs[i];
    }
  }
  winner->GetBiddingFacility()->subtractBid();
  cout << "Winner with highest bid: " << *winner->GetBiddingFacility()->getLastName() << endl
       << "Coins remaining: " << *winner->GetBiddingFacility()->getPlayerCoins() << endl;

  delete bf1;
  delete bf2;
  delete bf3;
}