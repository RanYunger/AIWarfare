#pragma once

// Imports
#include "Attacker.h"
#include "NPC.h"

#include <string>
using namespace std;

class Attacker;

class Courier : public NPC
{
private:
	// Fields
	int supply, transaction;
	bool isSearchingSupply, isSearchingAlly, isCalled;

public:
	// Properties
	int GetSupply();
	void SetSupply(int s);

	int GetTransaction();
	void SetTransaction(int t);

	bool IsSearchingSupply();
	void SetIsSearchingSupply(bool i);

	bool IsSearchingAlly();
	void SetIsSearchingAlly(bool i);

	bool IsCalled();
	void SetIsCalled(bool i);

	string GetStateName();

	// Constructors & Destructors
	Courier();
	Courier(Position l, int t, int r, int a, int m);
	~Courier();

	// Methods
	void GiveArms(Attacker* ally);
	void TakeArms(Attacker* ally);
	void GiveMeds(Attacker* ally);
	void TakeMeds(Attacker* ally);

	void PickSupply(int supply);
	void TradeSupply(Attacker* ally);
};