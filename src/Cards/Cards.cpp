#include "Cards.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

int HAND_SIZE = 8;
int TOP_BOARD_SIZE = 6;
int boardCosts[]{0, 1, 1, 2, 2, 3};
// =============================================
// Start of Cards class function implementations
// =============================================
/*
Cards default constructor
 note: ability = goods
*/
Cards::Cards() {
  this->ability = nullptr;
  this->action = nullptr;
  this->name = nullptr;
  this->number = nullptr;
}

Cards::Cards(string ability, string name) {
  this->ability = new string(ability);
  this->action = new string("None");
  this->name = new string(name);
  this->number = new int(0);
}

Cards::Cards(string ability, string action, string name) {
  this->ability = new string(ability);
  this->action = new string(action);
  this->name = new string(name);
  this->number = new int(0);
}

Cards::Cards(string ability, string action, string name, int number) {
  this->ability = new string(ability);
  this->action = new string(action);
  this->name = new string(name);
  this->number = new int(number);
}

/*
Cards copy constructor
*/
Cards::Cards(const Cards &card) { deepCopy(card); }

/*
Cards deconstructor
*/
Cards::~Cards() {
  delete ability;
  delete action;
  delete name;
  delete number;
}

/*
Stream insertion operator for cards
*/
ostream &operator<<(ostream &os, const Cards &cards) {
  os << "Name: " << cards.getName() << " Ability: " << cards.getAbility()
     << " Action: " << cards.getAction();
  if (cards.getNumber() != 0) {
    os << " Number: " << cards.getNumber();
  }
  return os;
}

/*
Assignment operator for cards
*/
Cards &Cards::operator=(Cards cards) {
  swap(*this, cards);
  return *this;
}

void Cards::deepCopy(const Cards &cards) {
  this->ability = new string(cards.getAbility());
  this->action = new string(cards.getAction());
  this->name = new string(cards.getName());
  this->number = new int(cards.getNumber());
}

string Cards::getAbility() const { return *this->ability; }

string Cards::getAction() const { return *this->action; }

string Cards::getName() const { return *this->name; }

int Cards::getNumber() const { return *this->number; }

void Cards::setAbility(string ability) { *this->ability = ability; }

void Cards::setAction(string action) { *this->action = action; }

void Cards::setName(string name) { *this->name = name; }

void Cards::setNumber(int number) { *this->number = number; }

void swap(Cards &first, Cards &second) {
  using std::swap;

  swap(first.ability, second.ability);
  swap(first.action, second.action);
  swap(first.name, second.name);
  swap(first.number, second.number);
}
// ===========================================
// End of Cards class function implementations
// ===========================================

// ============================================
// Start of Deck class function implementations
// ============================================
/*
Default deck constructor
It loads all the cards from a file located at Files folder
It shuffles all the cards
It initializes the card costs
It puts the cards to be chosen from on the board
 */
Deck::Deck(const int num_of_players) {
  string myText;
  string filePath;
  // Workaround for relative path depending on .exe location.
  cout << "Enter the path to the file containing the cards:" << endl;
  cin >> filePath;
  ifstream MyReadFile(filePath);
  deckCards = new vector<Cards *>;
  topBoard = new vector<Cards *>;
  while (getline(MyReadFile, myText)) {
    stringstream s_stream(myText);
    vector<string> cardStrings;
    while (s_stream.good()) {
      string substr;
      getline(s_stream, substr, ',');
      cardStrings.push_back(substr);
    }
    // Constructor order is: Ability, Action, Name, Number
    // Card order is:      Name, Ability, Action, Number
    if (cardStrings[3] != "") {
      if (num_of_players != 2) {
        deckCards->push_back(new Cards(cardStrings[1], cardStrings[2], cardStrings[0],
                                       std::stoi(cardStrings[3])));
      }
    } else if (cardStrings[2] != "") {
      deckCards->push_back(new Cards(cardStrings[1], cardStrings[2], cardStrings[0]));
    } else if (cardStrings[0] != "") {
      deckCards->push_back(new Cards(cardStrings[1], cardStrings[0]));
    }
  }
  std::random_device rng;
  std::mt19937 urng(rng());
  std::shuffle(deckCards->begin(), deckCards->end(), urng);
  deckSize = new int(deckCards->size());

  for (int i = 0; i < TOP_BOARD_SIZE; i++) {
    topBoard->push_back(this->draw());
  }
}

/*
Deck copy constructor
*/
Deck::Deck(const Deck &deck) { deepCopy(deck); }

/*
Deck deconstructor
*/
Deck::~Deck() {
  for (int i = 0; i < *deckSize; i++) {
    delete (*deckCards)[i];
  }
  for (int i = 0; i < TOP_BOARD_SIZE; i++) {
    delete (*topBoard)[i];
  }
  deckCards->clear();
  delete deckCards;
  topBoard->clear();
  delete topBoard;
  delete deckSize;
}

/*
Deck stream insertion operator
*/
ostream &operator<<(ostream &os, const Deck &deck) {
  os << "Deck: " << endl;
  for (int i = 0; i < deck.getDeckSize(); i++) {
    os << *deck.getCard(i) << endl;
  }

  os << "Top Board: " << endl;
  for (int i = 0; i < TOP_BOARD_SIZE; i++) {
    os << *deck.getTopBoardCard(i) << endl;
  }
  return os;
}

/*
Deck assignment operator
*/
Deck &Deck::operator=(Deck deck) {
  swap(*this, deck);
  return *this;
}

void Deck::deepCopy(const Deck &deck) {
  this->deckSize = new int(deck.getDeckSize());
  this->deckCards = new vector<Cards *>;
  this->topBoard = new vector<Cards *>;
  for (int i = 0; i < deck.getDeckSize(); i++) {
    this->deckCards->push_back(new Cards(*deck.getCard(i)));
  }
  for (int i = 0; i < TOP_BOARD_SIZE; i++) {
    this->topBoard->push_back(new Cards(*deck.getTopBoardCard(i)));
  }
}

int Deck::getDeckSize() const { return *this->deckSize; }

vector<Cards *> *Deck::getDeckCards() const { return deckCards; }

int Deck::getBoardPositionCost(int position) { return boardCosts[position]; }

Cards *Deck::getCard(int position) const { return (*deckCards)[position]; }

Cards *Deck::getTopBoardCard(int position) const { return (*topBoard)[position]; }

/*
Takes a card from a deck of cards
*/
Cards *Deck::draw() {
  Cards *card = deckCards->back();
  deckCards->pop_back();
  *this->deckSize = *this->deckSize - 1;
  return card;
}

/*
Shows the cards that can be picked from with their costs
*/
void Deck::showTopBoard() {
  for (int i = 0; i < TOP_BOARD_SIZE; i++) {
    cout << *this->getTopBoardCard(i) << " Cost: " << boardCosts[i] << endl;
  }
}

/*
Removes a card by position from the cards available to be picked from
*/
void Deck::removeFromTopBoard(int position) {
  topBoard->erase(topBoard->begin() + position);
  topBoard->push_back(this->draw());
}

void swap(Deck &first, Deck &second) {
  using std::swap;
  swap(first.deckSize, second.deckSize);
  swap(first.deckCards, second.deckCards);
  swap(first.topBoard, second.topBoard);
}
// ==========================================
// End of Deck class function implementations
// ==========================================

// ============================================
// Start of Hand class function implementations
// This class represents a hand of a player
// ============================================

/*
A hand default constructor
*/
Hand::Hand() {
  this->maxHandSize = new int(HAND_SIZE);
  this->handCards = new vector<Cards *>;
}

Hand::Hand(const int maxHandSize) {
  this->maxHandSize = new int(maxHandSize);
  this->handCards = new vector<Cards *>;
}

Hand::Hand(const Hand &hand) { deepCopy(hand); }

/*
deconstructor for hand
*/
Hand::~Hand() {
  for (int i = 0; i < this->getCurrentHandSize(); i++) {
    delete (*handCards)[i];
  }
  handCards->clear();
  delete handCards;
  delete maxHandSize;
}

/*
stream insertion operator for hand class
*/
ostream &operator<<(ostream &os, const Hand &hand) {
  for (int i = 0; i < hand.getCurrentHandSize(); i++) {
    os << *hand.getCard(i) << endl;
  }
  return os;
}

/*
Assignment operator for hand class
*/
Hand &Hand::operator=(Hand hand) {
  swap(*this, hand);
  return *this;
}

void Hand::deepCopy(const Hand &hand) {
  this->maxHandSize = new int(hand.getMaxHandSize());
  this->handCards = new vector<Cards *>;
  for (int i = 0; i < hand.getCurrentHandSize(); i++) {
    this->handCards->push_back(new Cards(*hand.getCard(i)));
  }
}

int Hand::getCurrentHandSize() const { return this->handCards->size(); }

int Hand::getMaxHandSize() const { return *this->maxHandSize; }

/*
gets a card from a hand of cards
*/
Cards *Hand::getCard(int position) const { return (*handCards)[position]; }

/*
Lets a player pick a card by position from the available cards and readjusts the available
cards and the deck of cards
*/
void Hand::exchange(int position, Deck &deck) {
  if (this->handCards->size() < *maxHandSize - 1) {
    this->handCards->push_back(new Cards(*deck.getTopBoardCard(position)));
    deck.removeFromTopBoard(position);
  } else {
    cout << "You already have the maximum amount of cards";
  }
}

/*
adds a card to the player's hand of cards
*/
void Hand::addCard(Cards *card) { this->handCards->push_back(card); }
vector<Cards *> *Hand::getHandCards() const {
  return handCards;
}
void swap(Hand &first, Hand &second) {
  using std::swap;

  swap(first.maxHandSize, second.maxHandSize);
  swap(first.handCards, second.handCards);
}
// ==========================================
// End of Hand class function implementations
// ==========================================