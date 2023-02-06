#pragma once

// Imports
#include "NPC.h"
#include "Shooter.h"

class Shooter;

class Courier : public NPC
{
private:
	// Fields
	int arms, meds;
	bool isSelfHealing, isSearchingShooter, isCollectingSupplies;

public:
	// Properties
	int GetArms();
	void SetArms(int a);

	int GetMeds();
	void SetMeds(int m);

	bool IsSelfHealing();
	void SetIsSelfHealing(bool i);

	bool IsSearchingShooter();
	void SetIsSearchingShooter(bool i);

	bool IsCollectingSuuplies();
	void SetIsCollectingSupplies(bool i);

	// Constructors & Destructors
	Courier();
	Courier(Position p, int t, int r);
	~Courier();

	// Methods
	void TakeArms();
	void TakeMeds();
	void GiveArms(Shooter* teammate);
	void GiveMeds(NPC* teammate);
	void SelfHeal();
};