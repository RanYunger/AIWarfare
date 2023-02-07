#pragma once

// Includes
#include "Courier.h"
#include "State.h"

class SearchAllyState : State
{
private:
	// Fields

public:
	// Properties

	// Constructors & Destructors
	SearchAllyState();
	~SearchAllyState();

	// Methods
	void Transform(Courier* courier);
	void OnEnter(Courier* courier);
	void OnExit(Courier* courier);
};