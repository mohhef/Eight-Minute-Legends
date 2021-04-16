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
	~BiddingFacility();
    BiddingFacility(const BiddingFacility &obj);
    BiddingFacility &operator=(const BiddingFacility &rhs);
    friend ostream &operator<<(ostream &output, BiddingFacility &region);
	int* getAmountBid();
	string* getLastName();
	void setLastName(string name);
	int* getPlayerCoins();
	void setAmountBid(int amountBid);
	void bid();
	bool higherBid(BiddingFacility* previousBidder);
	void subtractBid();
	
};