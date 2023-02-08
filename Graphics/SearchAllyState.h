#pragma once

// Includes
#include "Courier.h"
#include "State.h"

class SearchAllyState : State
{
private:
	// Fields

public:
	// Properties

	// Constructors & Destructors
	SearchAllyState();
	~SearchAllyState();

	// Methods
	void Transform(NPC* npc);
	void OnEnter(NPC* npc);
	void OnExit(NPC* npc);
};