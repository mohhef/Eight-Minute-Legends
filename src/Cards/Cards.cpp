#include "Cards.h"
#include <iostream>

Cards::Cards(){
    this->ability = nullptr;
    this->action = nullptr;
    this->name = nullptr;
    this->number = nullptr;
}

Cards::Cards(string ability, string action, string name){
    this->ability = new string(ability);
    this->action = new string(action);
    this->name = new string(name);
}

Cards::Cards(string ability, string action, string name, int number){
    this->ability = new string(ability);
    this->action = new string(action);
    this->name = new string(name);
    this->number = new int(number);
}

Cards::Cards(const Cards &card){
    this->ability = new string;
    this->action = new string;
    this->name = new string;
    this->number = new int;
    
    *this->ability = *card.getAbility();
    *this->action = *card.getAction();
    *this->name = *card.getName();
    if(card.getNumber() != nullptr){
        *this->number = *card.getNumber();
    }
}

Cards::~Cards(){
    delete ability;
    delete action;
    delete name;
    delete number;
}

ostream &operator<<(ostream &os, const Cards &cards) {
    os << "Name: " << *cards.getName() << " Ability: " << *cards.getAbility() << " Action: " << *cards.getAction();
    return os;
}

Cards& Cards::operator=(const Cards& cards){
    this->ability = cards.getAbility();
    this->action = cards.getAction();
    this->name = cards.getName();
    if(cards.getNumber() != nullptr){
        this->number = cards.getNumber();
    }
    return *this;
}

string* Cards::getAbility() const{
    return this->ability;
}

string* Cards::getAction() const{
    return this->action;
}

string* Cards::getName() const{
    return this->name;
}

int* Cards::getNumber() const{
    return this->number;
}

void Cards::setAbility(string ability){
    *this->ability = ability;
}

void Cards::setAction(string action){
    *this->action = action;
}

void Cards::setName(string name){
    *this->name = name;
}

void Cards::setNumber(int number){
    *this->number = number;
}