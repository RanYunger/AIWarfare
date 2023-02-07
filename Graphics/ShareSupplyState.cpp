// Includes
#include "ShareSupplyState.h"

// Properties

// Constructors & Destructors
ShareSupplyState::ShareSupplyState()
	: State() {}
ShareSupplyState::~ShareSupplyState() {}

// Methods

/// <summary>
/// Transforms the courier to the next state.
/// </summary>
/// <param name="courier">The courier</param>
void ShareSupplyState::Transform(Courier* courier)
{
	// TODO: COMPLETE
}

/// <summary>
/// Enters the courier's state.
/// </summary>
/// <param name="courier">The courier</param>
void ShareSupplyState::OnEnter(Courier* courier) { courier->SetIsSharingSupply(true); }

/// <summary>
/// Exits the courier's state.
/// </summary>
/// <param name="courier">The courier</param>
void ShareSupplyState::OnExit(Courier* courier) { courier->SetIsSharingSupply(false); }