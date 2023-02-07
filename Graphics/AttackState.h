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
	void Transform(Attacker* attacker);
	void OnEnter(Attacker* attacker);
	void OnExit(Attacker* attacker);
};