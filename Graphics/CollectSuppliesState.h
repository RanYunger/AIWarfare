#pragma once
#include "State.h"

class CollectSuppliesState : public State
{
public:
	// Properties

	// Constuctors & Destructors
	CollectSuppliesState();
	~CollectSuppliesState();

	// Methods
	void Transform(NPC* npc);
	void OnEnter(NPC* npc);
	void OnExit(NPC* npc);
};