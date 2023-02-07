#pragma once

// Includes
#include "Courier.h"
#include "State.h"

class SearchSupplyState : State
{
private:
	// Fields

public:
	// Properties

	// Constructors & Destructors
	SearchSupplyState();
	~SearchSupplyState();

	// Methods
	void Transform(Courier* courier);
	void OnEnter(Courier* courier);
	void OnExit(Courier* courier);
};