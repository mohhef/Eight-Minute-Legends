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
  Cards &operator=(Cards card);

  void deepCopy(const Cards &card);

  string getAction() const;
  string getAbility() const;
  string getName() const;
  int getNumber() const;

  void setAction(string action);
  void setAbility(string ability);
  void setName(string name);
  void setNumber(int number);

  friend void swap(Cards &first, Cards &second);
};

class Deck {
 private:
  int *deckSize;
  vector<Cards *> *deckCards;
  vector<Cards *> *topBoard;

 public:
  Deck(const int num_of_players = 2);
  Deck(const Deck &deck);
  virtual ~Deck();

  friend ostream &operator<<(ostream &os, const Deck &deck);
  Deck &operator=(Deck deck);

  void deepCopy(const Deck &deck);

  int getDeckSize() const;
  int *getBoardCosts() const;
  vector<Cards *> *getDeckCards() const;
  int static getBoardPositionCost(int position);
  Cards *getCard(int position) const;
  Cards *getTopBoardCard(int position) const;
  Cards *draw();

  void showTopBoard();
  void removeFromTopBoard(int position);
  void shuffleDeck();
  void drawTopBoard();

  friend void swap(Deck &first, Deck &second);
};

class Hand {
 private:
  int *maxHandSize;
  vector<Cards *> *handCards;

 public:
  Hand();
  Hand(const int maxHandSize);
  Hand(const Hand &hand);
  virtual ~Hand();

  friend ostream &operator<<(ostream &os, const Hand &hand);
  Hand &operator=(Hand hand);

  void deepCopy(const Hand &hand);

  int getCurrentHandSize() const;
  int getMaxHandSize() const;
  // vector<Cards*> getHandCards() const;
  Cards *getCard(int position) const;
  void exchange(int position, Deck &deck, int totalPlayerCoin);
  void addCard(Cards *card);

  friend void swap(Hand &first, Hand &second);
};
