#include "BiddingFacility.h"

#include <iostream>
#include <limits>

BiddingFacility::BiddingFacility(int numTokens, string lastName) {
  this->amountBid = new int(0);
  this->playerCoins = new int(numTokens);
  this->lastName = new string(lastName);
}
BiddingFacility::~BiddingFacility() {
  delete amountBid;
  delete playerCoins;
  delete lastName;
}

BiddingFacility::BiddingFacility(const BiddingFacility &obj) {
  this->amountBid = new int;
  this->playerCoins = new int;
  this->lastName = new string;

  *amountBid = *obj.amountBid;
  *playerCoins = *obj.playerCoins;
  *lastName = *obj.lastName;
}

BiddingFacility &BiddingFacility::operator=(const BiddingFacility &rhs) {
  if (this != &rhs) {
    *amountBid = *rhs.amountBid;
    *playerCoins = *rhs.playerCoins;
    *lastName = *rhs.lastName;
  }
  return *this;
}

std::ostream &operator<<(ostream &output, BiddingFacility &bidding_facility) {
  output << "Amount Bid: " << *(bidding_facility.amountBid) << ", "
         << "Player Coins: " << *(bidding_facility.playerCoins) << ", "
         << "Last Name: " << *(bidding_facility.lastName) << endl;
  return output;
}

int *BiddingFacility::getAmountBid() { return amountBid; }

string *BiddingFacility::getLastName() { return this->lastName; }

void BiddingFacility::setLastName(string *name) { this->lastName = name;}
void BiddingFacility::setAmountBid(int *amountBid) { this->amountBid = amountBid; }

int *BiddingFacility::getPlayerCoins() { return playerCoins; }

void BiddingFacility::bid() {
  int bid;
  cout << "Enter a bid between 0 and " << *playerCoins << ": " << endl;
  cin >> bid;

  while (cin.fail() || bid > *playerCoins || bid < 0) {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Invalid entry, please enter a bid between 0 and " << *playerCoins << ": ";
    cin >> bid;
    *amountBid = bid;
  }

  *amountBid = bid;
}

// Returns true if current players bid is higher than the previous highest bidder.
bool BiddingFacility::higherBid(BiddingFacility *previousBidder) {
  // If "previousBidder" is a nullptr
  if (previousBidder == nullptr) {
    return true;
  }

  if (*amountBid > *previousBidder->getAmountBid()) {
    return true;
  } else if (*amountBid == *previousBidder->getAmountBid()) {
    // Select bidder with higher last name in alphabetical order
    int i = 0;
    string previousBidderLastName = *previousBidder->getLastName();
    string currentBidderLastName = this->lastName[0];

    while (currentBidderLastName[i] != '\0' && previousBidderLastName[i] != '\0') {
      if (currentBidderLastName[i] < previousBidderLastName[i]) {
        return true;
      } else if (currentBidderLastName[i] > previousBidderLastName[i]) {
        return false;
      } else {
        i++;
      }
    }
    if (currentBidderLastName.length() < previousBidderLastName.length()) {
      return true;
    } else if (currentBidderLastName.length() > previousBidderLastName.length()) {
      return false;
    } else {
      return true;
    }
  }
  return false;
}

void BiddingFacility::subtractBid() { *playerCoins -= *amountBid; }