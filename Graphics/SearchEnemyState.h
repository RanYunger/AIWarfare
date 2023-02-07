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
	void Transform(Attacker* attacker);
	void OnEnter(Attacker* attacker);
	void OnExit(Attacker* attacker);
};