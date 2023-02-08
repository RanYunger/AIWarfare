#pragma once

// Includes
#include "Attacker.h"
#include "State.h"

class SearchShelterState : State
{
private:
	// Fields

public:
	// Properties

	// Constructors & Destructors
	SearchShelterState();
	~SearchShelterState();

	// Methods
	void Transform(NPC* npc);
	void OnEnter(NPC* npc);
	void OnExit(NPC* npc);
};