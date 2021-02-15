#pragma once
#include <vector>
#include <string>

using namespace std;

class BiddingFacility {
private:
	int* amountBid;
	string* lastName;
	int* playerCoins;

public:
	BiddingFacility(int numTokens, string lastName);
	int* getAmountBid();
	string* getLastName();
	int* getPlayerCoins();
	void setAmountBid(int* amountBid);
	void bid();
	bool higherBid(BiddingFacility* previousBidder);
	void subtractBid();
	
};