#pragma once

// Includes
#include "Attacker.h"
#include "State.h"

class SearchEnemyState : State
{
private:
	// Fields

public:
	// Properties

	// Constructors & Destructors
	SearchEnemyState();
	~SearchEnemyState();

	// Methods
	void Transform(NPC* npc);
	void OnEnter(NPC* npc);
	void OnExit(NPC* npc);
};