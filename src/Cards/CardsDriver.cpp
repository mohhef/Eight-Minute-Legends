#include <iostream>
#include "Cards.h"

int main(){
    Cards *testCard = new Cards("FLYING", "PLACE_ARMIES,4", "Dire Eye");
    Cards *testCard2 = new Cards(*testCard);

    cout << *testCard << endl;
    cout << *testCard2 << endl;

    string test = "Changed";
    testCard->setAbility(test);

    cout << *testCard << endl;
    cout << *testCard2 << endl;

    delete testCard;
    delete testCard2;
    return 0;
}