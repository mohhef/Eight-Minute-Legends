#pragma once
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

using namespace std;

class Cards {
 private:
  string *ability;
  string *action;
  string *name;
  int *number;

 public:
  Cards();
  Cards(string ability, string name);
  Cards(string ability, string action, string name);
  Cards(string ability, string action, string name, int number);
  Cards(const Cards &card);
  virtual ~Cards();

  friend ostream &operator<<(ostream &os, const Cards &cards);
  Cards &operator=(const Cards &card);

  string getAction() const;
  string getAbility() const;
  string getName() const;
  int getNumber() const;

  void setAction(string action);
  void setAbility(string ability);
  void setName(string name);
  void setNumber(int number);
};

class Deck {
 private:
  int *boardCosts;
  int *deckSize;
  vector<Cards *> deckCards;
  vector<Cards *> topBoard;

 public:
  Deck();
  Deck(const Deck &deck);
  virtual ~Deck();

  friend ostream &operator<<(ostream &os, const Deck &deck);
  Deck &operator=(const Deck &deck);

  int getDeckSize() const;
  int *getBoardCosts() const;
  vector<Cards *> getDeckCards() const;
  int getBoardPositionCost(int position) const;
  Cards *getCard(int position) const;
  Cards *getTopBoardCard(int position) const;
  Cards *draw();

  void showTopBoard();
  void removeFromTopBoard(int position);
};

class Hand {
 private:
  int *maxHandSize;
  vector<Cards *> handCards;

 public:
  Hand();
  Hand(const int maxHandSize);
  Hand(const Hand &hand);
  virtual ~Hand();

  friend ostream &operator<<(ostream &os, const Hand &hand);
  Hand &operator=(const Hand &hand);

  int getCurrentHandSize() const;
  int getMaxHandSize() const;
  // vector<Cards*> getHandCards() const;
  Cards *getCard(int position) const;
  void exchange(int position, Deck &deck);
  void addCard(Cards *card);
};
