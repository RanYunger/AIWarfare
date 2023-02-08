#pragma once

// Includes
#include "Courier.h"
#include "State.h"

class ShareSupplyState : State
{
private:
	// Fields

public:
	// Properties

	// Constructors & Destructors
	ShareSupplyState();
	~ShareSupplyState();

	// Methods
	void Transform(NPC* npc);
	void OnEnter(NPC* npc);
	void OnExit(NPC* npc);
};