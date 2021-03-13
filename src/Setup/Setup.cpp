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
    // TODO: We currently have two independent trackers for player coins.
    winner->PayCoin(*winner->GetBiddingFacility()->getAmountBid());
    cout << "Winner with highest bid: " << *winner->GetBiddingFacility()->getLastName() << endl
         << "Coins remaining: " << *winner->GetBiddingFacility()->getPlayerCoins() << endl;
    startingPlayer = winner;
    return 0;
};

int Setup::mainLoop() {
  int turn = 1;
  bool gameover = false;
  cout << "*********************Game Start*********************" << endl;
  while(!gameover) {
    int choiceIndex = 0;
    cout << "****** Turn #" + to_string(turn) + " ******" << endl;
    for(int i = 0; i < players->size(); ++i) {
      cout << "Top Board: " << endl;
      deck->showTopBoard();
      cout << "(" + players->at(i)->GetName() + ") Choose a card by entering its index (1-6) or enter any other number to skip:" << endl;
      cin >> choiceIndex;
      while(cin.fail()){
        cout << "Please enter a valid number:";
        cin.clear();
        cin.ignore(256,'\n');
        cin >> choiceIndex;
      }
      if(choiceIndex > 0 && choiceIndex <= 6){
        int cardCost = deck->getBoardPositionCost(choiceIndex - 1);
        int playerCoins = players->at(i)->GetCoins();
        if(playerCoins < cardCost){
          cout << "Insufficient funds to purchase this card. You have " + to_string(playerCoins) + " remaining." << endl;
          // Temporary hacky way to run same player again, if they chose a card they cannot buy.
          i--;
          continue;
        }
        else{
          players->at(i)->GetHand()->exchange(choiceIndex - 1, *deck, players->at(i)->GetCoins());
          players->at(i)->PayCoin(deck->getBoardPositionCost(choiceIndex - 1));
          int currentHandSize = players->at(i)->GetHand()->getCurrentHandSize();
          string playerAction = players->at(i)->GetHand()->getCard(currentHandSize - 1)->getAction();
          cout << players->at(i)->GetName() + "'s action is: " + playerAction << endl;
        }

        // Add code/method to take action and after action here
      }
    }
    cout << "Checking for gameover..." << endl;
    turn++;
    gameover = checkGameover();
  }

  // Add code/method to compute game score here
  return 0;
}

bool Setup::checkGameover(){
  for(int i = 0; i < players->size(); ++i) {
    if(players->at(i)->GetHand()->getCurrentHandSize() == 11) {
      return true;
    }
  }
  return false;
}
