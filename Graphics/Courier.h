#pragma once

// Imports
#include "NPC.h"
#include "Shooter.h"

class Shooter;

class Courier : public NPC
{
private:
	// Fields
	bool isSearchingShooter, isCollectingSupplies;

public:
	// Properties
	bool IsSearchingShooter();
	void SetIsSearchingShooter(bool i);

	bool IsCollectingSuuplies();
	void SetIsCollectingSupplies(bool i);

	// Constructors & Destructors
	Courier();
	Courier(Position l, int t, int r);
	~Courier();

	// Methods
};