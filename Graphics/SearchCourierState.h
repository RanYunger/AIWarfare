#pragma once
#include "State.h"

class SearchCourierState : public State
{
public:
	// Properties

	// Constuctors & Destructors
	SearchCourierState();
	~SearchCourierState();

	// Methods
	void Transform(NPC* npc);
	void OnEnter(NPC* npc);
	void OnExit(NPC* npc);
};