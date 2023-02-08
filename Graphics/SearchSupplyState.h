#pragma once

// Includes
#include "Courier.h"
#include "State.h"

class SearchSupplyState : State
{
private:
	// Fields

public:
	// Properties

	// Constructors & Destructors
	SearchSupplyState();
	~SearchSupplyState();

	// Methods
	void Transform(NPC* npc);
	void OnEnter(NPC* npc);
	void OnExit(NPC* npc);
};