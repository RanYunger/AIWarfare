#pragma once

// Includes
#include "NPC.h"

class NPC;

class State
{
private:
	// Fields

public:
	// Properties

	// Constructors & Destructors

	// Methods
	virtual void Transform(NPC* npc) = 0;	// To the next state
	virtual void OnEnter(NPC* npc) = 0;		// To the current state
	virtual void OnExit(NPC* npc) = 0;		// From the current state
};