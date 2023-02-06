#pragma once
#include "State.h"

class AttackState : public State
{
public:
	// Properties

	// Constuctors & Destructors
	AttackState();
	~AttackState();

	// Methods
	void Transform(NPC* npc);
	void OnEnter(NPC* npc);
	void OnExit(NPC* npc);
};