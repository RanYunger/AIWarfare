#pragma once
#include "State.h"

class SearchShooterState : public State
{
public:
	// Properties

	// Constuctors & Destructors
	SearchShooterState();
	~SearchShooterState();

	// Methods
	void Transform(NPC* npc);
	void OnEnter(NPC* npc);
	void OnExit(NPC* npc);
};