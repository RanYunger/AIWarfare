#pragma once

// Imports
#include "Attacker.h"
#include "NPC.h"

class Shooter;

class Courier : public NPC
{
private:
	// Fields
	bool isSearchingSupply, isSearchingAlly, isSharingSupply;

public:
	// Properties
	bool IsSearchingSupply();
	void SetIsSearchingSupply(bool i);

	bool IsSearchingAlly();
	void SetIsSearchingAlly(bool i);

	bool IsSharingSupply();
	void SetIsSharingSupply(bool i);

	// Constructors & Destructors
	Courier();
	~Courier();

	// Methods
	void GiveArms(Attacker* teammate);
	void TakeArms(Attacker* teammate);
	void GiveMeds(Attacker* teammate);
	void TakeMeds(Attacker* teammate);

	void PickSupply(int supply);

};