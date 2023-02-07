// Includes
#include "SearchShelterState.h"

// Properties

// Constructors & Destructors
SearchShelterState::SearchShelterState()
	: State() {}
SearchShelterState::~SearchShelterState() {}

// Methods

/// <summary>
/// Transforms the attacker to the next state.
/// </summary>
/// <param name="attacker">The attacker</param>
void SearchShelterState::Transform(Attacker* attacker)
{
	// TODO: COMPLETE
}

/// <summary>
/// Enters the attacker's state.
/// </summary>
/// <param name="attacker">The attacker</param>
void SearchShelterState::OnEnter(Attacker* attacker) { attacker->SetIsSearchingShelter(true); }

/// <summary>
/// Exits the attacker's state.
/// </summary>
/// <param name="attacker">The attacker</param>
void SearchShelterState::OnExit(Attacker* attacker) { attacker->SetIsSearchingShelter(false); }