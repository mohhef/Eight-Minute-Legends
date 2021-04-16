//
// Created by Mohamed Ashraf on 2021-04-04.
//
#pragma once
#include <list>
using namespace std;

class Observer {
 public:
  Observer();
  virtual void update() = 0;
};

class Subject{
 public:
  virtual void Attach(Observer* o);
  virtual void Detach(Observer* o);
  virtual void Notify();
  Subject();
  list<Observer*> *observers;
};

