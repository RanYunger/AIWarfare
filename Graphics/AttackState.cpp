// Includes
#include "AttackState.h"

// Constructors & Destructors
AttackState::AttackState()
	: State() {}
AttackState::~AttackState() {}

// Methods

/// <summary>
/// Transforms the attacker to the next state.
/// </summary>
/// <param name="attacker">The attacker</param>
void AttackState::Transform(Attacker* attacker)
{
	// TODO: COMPLETE
}

/// <summary>
/// Enters the attacker's state.
/// </summary>
/// <param name="attacker">The attacker</param>
void AttackState::OnEnter(Attacker* attacker) { attacker->SetIsAttacking(true); }

/// <summary>
/// Exits the attacker's state.
/// </summary>
/// <param name="attacker">The attacker</param>
void AttackState::OnExit(Attacker* attacker) { attacker->SetIsAttacking(false); }