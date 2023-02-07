// Includes
#include "SearchSupplyState.h"

// Properties

// Constructors & Destructors
SearchSupplyState::SearchSupplyState()
	: State() {}
SearchSupplyState::~SearchSupplyState() {}

// Methods

/// <summary>
/// Transforms the courier to the next state.
/// </summary>
/// <param name="courier">The courier</param>
void SearchSupplyState::Transform(Courier* courier)
{
	// TODO: COMPLETE
}

/// <summary>
/// Enters the courier's state.
/// </summary>
/// <param name="courier">The courier</param>
void SearchSupplyState::OnEnter(Courier* courier) { courier->SetIsSearchingSupply(true); }

/// <summary>
/// Exits the courier's state.
/// </summary>
/// <param name="courier">The courier</param>
void SearchSupplyState::OnExit(Courier* courier) { courier->SetIsSearchingSupply(false); }