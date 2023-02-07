// Includes
#include "SearchAllyState.h"

// Properties

// Constructors & Destructors
SearchAllyState::SearchAllyState()
	: State() {}
SearchAllyState::~SearchAllyState() {}

// Methods

/// <summary>
/// Transforms the courier to the next state.
/// </summary>
/// <param name="courier">The courier</param>
void SearchAllyState::Transform(Courier* courier)
{
	// TODO: COMPLETE
}

/// <summary>
/// Enters the courier's state.
/// </summary>
/// <param name="courier">The courier</param>
void SearchAllyState::OnEnter(Courier* courier) { courier->SetIsSearchingAlly(true); }

/// <summary>
/// Exits the courier's state.
/// </summary>
/// <param name="courier">The courier</param>
void SearchAllyState::OnExit(Courier* courier) { courier->SetIsSearchingAlly(false); }