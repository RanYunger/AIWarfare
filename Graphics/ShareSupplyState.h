#pragma once

// Includes
#include "Courier.h"
#include "State.h"

class ShareSupplyState : State
{
private:
	// Fields

public:
	// Properties

	// Constructors & Destructors
	ShareSupplyState();
	~ShareSupplyState();

	// Methods
	void Transform(Courier* courier);
	void OnEnter(Courier* courier);
	void OnExit(Courier* courier);
};