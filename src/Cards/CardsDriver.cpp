#include <iostream>
#include "Cards.h"
#include "../MainDriver.h"

void Driver::part4() {
  Deck *deck = new Deck();
  Hand *myHand = new Hand();

  cout << "Top Board: " << endl;
  deck->showTopBoard();
  int coins  = int(12);
  myHand->exchange(2, *deck, coins);

  cout << "\nMy Hand after exchange: " << endl;
  cout << *myHand << endl;
  
  cout << "\nTop Board after exchange: " << endl;
  deck->showTopBoard();

  delete deck;
  delete myHand;
}