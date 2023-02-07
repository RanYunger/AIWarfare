#pragma once

// Includes
#include "Attacker.h"
#include "State.h"

class SearchShelterState : State
{
private:
	// Fields

public:
	// Properties

	// Constructors & Destructors
	SearchShelterState();
	~SearchShelterState();

	// Methods
	void Transform(Attacker* attacker);
	void OnEnter(Attacker* attacker);
	void OnExit(Attacker* attacker);
};