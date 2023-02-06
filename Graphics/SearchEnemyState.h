#pragma once
#include "State.h"

class SearchEnemyState : public State
{
public:
	// Properties

	// Constuctors & Destructors
	SearchEnemyState();
	~SearchEnemyState();

	// Methods
	void Transform(NPC* npc);
	void OnEnter(NPC* npc);
	void OnExit(NPC* npc);
};