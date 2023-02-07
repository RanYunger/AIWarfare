// Includes
#include "SearchEnemyState.h"

// Properties

// Consturctors & Destructors
SearchEnemyState::SearchEnemyState()
	: State() {}
SearchEnemyState::~SearchEnemyState() {}

// Methods

/// <summary>
/// Transforms the attacker to the next state.
/// </summary>
/// <param name="attacker">The attacker</param>
void SearchEnemyState::Transform(Attacker* attacker)
{
	// TODO: COMPLETE
}

/// <summary>
/// Enters the attacker's state.
/// </summary>
/// <param name="attacker">The attacker</param>
void SearchEnemyState::OnEnter(Attacker* attacker) { attacker->SetIsSearchingEnemy(true); }

/// <summary>
/// Exits the attacker's state.
/// </summary>
/// <param name="attacker">The attacker</param>
void SearchEnemyState::OnExit(Attacker* attacker) { attacker->SetIsSearchingEnemy(false); }