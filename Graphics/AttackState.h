#pragma once

// Includes
#include "Attacker.h"
#include "State.h"

class AttackState : State
{
private:
	// Fields

public:
	// Properties

	// Constructors & Destructors
	AttackState();
	~AttackState();

	// Methods
	void Transform(NPC* npc);
	void OnEnter(NPC* npc);
	void OnExit(NPC* npc);
};