#include <iostream>
#include "BiddingFacility.h"

BiddingFacility::BiddingFacility(int numTokens, string lastName) {
	this->amountBid = new int(0);
	this->playerCoins = new int(numTokens);
	this->lastName = new string(lastName);

}

int* BiddingFacility::getAmountBid() {
	return amountBid;
}

string* BiddingFacility::getLastName() {
	return this->lastName;
}

void BiddingFacility::setAmountBid(int* amountBid) {
	this->amountBid = amountBid;
}

int* BiddingFacility::getPlayerCoins() {
	return playerCoins;
}

void BiddingFacility::bid() {
	int bid;
	cout << "Enter a bid between 0 and " << *playerCoins << ": ";
	cin >> bid;

	while (cin.fail() || bid > * playerCoins || bid < 0) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		cout << "Invalid entry, please enter a bid between 0 and " << *playerCoins << ": ";
		cin >> bid;
		*amountBid = bid;
		
	}

	*amountBid = bid;
}

//Returns true if current players bid is higher than the previous highest bidder.
bool BiddingFacility::higherBid(BiddingFacility* previousBidder) {

	//If "previousBidder" is a nullptr 
	if (previousBidder == nullptr) {
		return true;
	}
	
	if (*amountBid > *previousBidder->getAmountBid()) {
		return true;
	}
	else if (*amountBid < *previousBidder->getAmountBid()) {
		return false;
	}
	else if (*amountBid == *previousBidder->getAmountBid()) { 
		//Select bidder with higher last name in alphabetical order
		int i =  0;
		string z;
		z = *previousBidder->getLastName();
		string y = this->lastName[0];

		while (y[i] != '\0' &&  z[i] != '\0') {
			if (y[i] < z[i]) {
				return true;
			}
			else if (y[i] > z[i]) {
				return false;
			}
			else {
				i++;
			}
			
		}
		
		if (y.length() < z.length()) {
			return true;
		}
		else if (y.length() > z.length()) {
			return false;
		}
		else {
			return true;
		}
	}
}

void BiddingFacility::subtractBid() {

	*playerCoins -= *amountBid;
}