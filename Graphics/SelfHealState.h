#pragma once
#include "State.h"

class SelfHealState : public State
{
public:
	// Properties

	// Constuctors & Destructors
	SelfHealState();
	~SelfHealState();

	// Methods
	void Transform(NPC* npc);
	void OnEnter(NPC* npc);
	void OnExit(NPC* npc);
};