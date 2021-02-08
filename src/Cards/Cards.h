#include <string>
#include <ostream>
#include <iostream>
#include <vector>

using namespace std;

class Cards{

private:
    string* ability;
    string* action;
    string* name;
    int* number;

public:
    Cards();
    Cards(string ability, string action, string name);
    Cards(string ability, string action, string name, int number);
    Cards(const Cards &card);

    virtual ~Cards();

    string* getAction() const;
    string* getAbility() const;
    string* getName() const;
    int* getNumber() const;

    void setAction(string action);
    void setAbility(string ability);
    void setName(string name);
    void setNumber(int number);

    friend ostream &operator<<(ostream &os, const Cards &cards);
    Cards& operator=(const Cards& card);
};

class Deck{

private:
    int boardCosts[6] = {0,1,1,2,2,3};
    vector<Cards*> deckCards;

    Deck();
    // Deck(Hand &handCards);

    virtual ~Deck();

    int getBoardPositionCost(int position) const;
    Cards* draw() const;

    friend ostream &operator<<(ostream &os, const Deck &deck);
    Deck& operator=(const Deck& deck);
    Deck(const Deck &deck);
};

class Hand{

private:
    Cards* boardCards[6];

    Hand();
    Hand(Cards &boardCards);

    Cards* exchange(int position) const;

    friend ostream &operator<<(ostream &os, const Hand &hand);
    Hand& operator=(const Hand& hand);
    Hand(const Hand &hand);
};
