#include "Cards.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <random>
#include <algorithm>
#include <vector>

int HAND_SIZE = 8;
int TOP_BOARD_SIZE = 6;

// =============================================
// Start of Cards class function implementations
// =============================================
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

// Copy constructor
Cards::Cards(const Cards &card) {
  this->ability = new string;
  this->action = new string;
  this->name = new string;
  this->number = new int;

  *this->ability = card.getAbility();
  *this->action = card.getAction();
  *this->name = card.getName();
  *this->number = card.getNumber();
}

Cards::~Cards() {
  delete ability;
  delete action;
  delete name;
  delete number;
}

ostream &operator<<(ostream &os, const Cards &cards) {
  os << "Name: " << cards.getName() << " Ability: " << cards.getAbility() << " Action: " << cards.getAction();
  if (cards.getNumber() != 0) {
    os << " Number: " << cards.getNumber();
  }
  return os;
}

Cards &Cards::operator=(const Cards &cards) {
  *this->ability = cards.getAbility();
  *this->action = cards.getAction();
  *this->name = cards.getName();
  if (cards.getNumber() != 0) {
    *this->number = cards.getNumber();
  }
  return *this;
}

string Cards::getAbility() const {
  return *this->ability;
}

string Cards::getAction() const {
  return *this->action;
}

string Cards::getName() const {
  return *this->name;
}

int Cards::getNumber() const {
  return *this->number;
}

void Cards::setAbility(string ability) {
  *this->ability = ability;
}

void Cards::setAction(string action) {
  *this->action = action;
}

void Cards::setName(string name) {
  *this->name = name;
}

void Cards::setNumber(int number) {
  *this->number = number;
}
// ===========================================
// End of Cards class function implementations
// ===========================================

// ============================================
// Start of Deck class function implementations
// ============================================
Deck::Deck() {
  string myText;
  ifstream MyReadFile("../Files/cards.txt");
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
      deckCards->push_back(new Cards(cardStrings[1], cardStrings[2], cardStrings[0], std::stoi(cardStrings[3])));
    } else if (cardStrings[2] != "") {
      deckCards->push_back(new Cards(cardStrings[1], cardStrings[2], cardStrings[0]));
    } else if (cardStrings[0] != "") {
      deckCards->push_back(new Cards(cardStrings[1], cardStrings[0]));
    }
  }
  std::random_device rng;
  std::mt19937 urng(rng());
  std::shuffle(deckCards->begin(), deckCards->end(), urng);

  boardCosts = new int[HAND_SIZE]{0, 1, 1, 2, 2, 3};
  deckSize = new int(deckCards->size());

  for (int i = 0; i < TOP_BOARD_SIZE; i++) {
    topBoard->push_back(this->draw());
  }
}

Deck::Deck(const Deck &deck) {
  this->deckSize = new int;
  *this->deckSize = deck.getDeckSize();
  deckCards = new vector<Cards *>;
  topBoard = new vector<Cards *>;
  this->boardCosts = new int[HAND_SIZE];
  for (int i = 0; i < HAND_SIZE; i++) {
    this->boardCosts[i] = deck.getBoardCosts()[i];
  }
  for (int i = 0; i < deck.getDeckSize(); i++) {
    deckCards->push_back(new Cards(*deck.getCard(i)));
  }
  for (int i = 0; i < TOP_BOARD_SIZE; i++) {
    topBoard->push_back(new Cards(*deck.getTopBoardCard(i)));
  }
}

Deck::~Deck() {
  for (int i = 0; i < *deckSize; i++) {
    delete deckCards->at(i);
  }
  for (int i = 0; i < TOP_BOARD_SIZE; i++) {
    delete topBoard->at(i);
  }
  deckCards->clear();
  delete deckCards;
  topBoard->clear();
  delete topBoard;
  delete deckSize;
  delete boardCosts;
}

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

Deck &Deck::operator=(const Deck &deck) {
  this->boardCosts = deck.getBoardCosts();
  this->deckCards = deck.getDeckCards();
  *this->deckSize = deck.getDeckSize();
  return *this;
}

int Deck::getDeckSize() const {
  return *this->deckSize;
}

int *Deck::getBoardCosts() const {
  return this->boardCosts;
}

vector<Cards *>* Deck::getDeckCards() const {
  return deckCards;
}

int Deck::getBoardPositionCost(int position) const {
  return boardCosts[position];
}

Cards *Deck::getCard(int position) const {
  return deckCards->at(position);
}

Cards *Deck::getTopBoardCard(int position) const {
  return topBoard->at(position);
}

Cards *Deck::draw() {
  Cards *card = deckCards->back();
  deckCards->pop_back();
  *this->deckSize = *this->deckSize - 1;
  return card;
}

void Deck::showTopBoard() {
  for (int i = 0; i < TOP_BOARD_SIZE; i++) {
    cout << *this->getTopBoardCard(i) << " Cost: " << boardCosts[i] << endl;
  }
}

void Deck::removeFromTopBoard(int position) {
  topBoard->erase(topBoard->begin() + position);
  topBoard->push_back(this->draw());
}
// ==========================================
// End of Deck class function implementations
// ==========================================

// ============================================
// Start of Hand class function implementations
// ============================================
Hand::Hand() {
  this->maxHandSize = new int(HAND_SIZE);
  this->handCards = new vector<Cards *>;
}

Hand::Hand(const int maxHandSize) {
  this->maxHandSize = new int(maxHandSize);
  this->handCards = new vector<Cards *>;
}
Hand::Hand(const Hand &hand) {
  this->maxHandSize = new int(hand.getMaxHandSize());
  this->handCards = new vector<Cards *>;
  for (int i = 0; i < hand.getCurrentHandSize(); i++) {
    this->handCards->push_back(new Cards(*hand.getCard(i)));
  }
}
Hand::~Hand() {
  for (int i = 0; i < this->getCurrentHandSize(); i++) {
    delete handCards->at(i);
  }
  handCards->clear();
  delete handCards;
  delete maxHandSize;
}

ostream &operator<<(ostream &os, const Hand &hand) {
  for (int i = 0; i < hand.getCurrentHandSize(); i++) {
    os << *hand.getCard(i) << endl;
  }
  return os;
}

Hand &Hand::operator=(const Hand &hand) {
  for (int i = 0; i < hand.getCurrentHandSize(); i++) {
    this->handCards->at(i) = hand.getCard(i);
  }
  *this->maxHandSize = hand.getMaxHandSize();
  return *this;
}

int Hand::getCurrentHandSize() const {
  return this->handCards->size();
}

int Hand::getMaxHandSize() const {
  return *this->maxHandSize;
}

Cards *Hand::getCard(int position) const {
  return handCards->at(position);
}

void Hand::exchange(int position, Deck &deck) {
  // TODO: Change to check available coins and wait for y/n decision. Also change hand size depending on players
  if (this->handCards->size() < *maxHandSize - 1) {
    this->handCards->push_back(new Cards(*deck.getTopBoardCard(position)));
    deck.removeFromTopBoard(position);
  }
}

void Hand::addCard(Cards *card) {
  this->handCards->push_back(card);
}
// ==========================================
// End of Hand class function implementations
// ==========================================