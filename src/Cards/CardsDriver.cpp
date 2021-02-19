#include <iostream>
#include "Cards.h"

int main(){
  Deck *deck = new Deck();
  Hand *myHand = new Hand();

  cout << "Top Board: " << endl;
  deck->showTopBoard();

  myHand->exchange(2, *deck);

  cout << "\nMy Hand after exchange: " << endl;
  cout << *myHand << endl;
  
  cout << "\nTop Board after exchange: " << endl;
  deck->showTopBoard();

  delete deck;
  delete myHand;
  
  return 0;
}