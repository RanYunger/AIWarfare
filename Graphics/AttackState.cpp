// Includes
#include "AttackState.h"

// Constructors & Destructors
AttackState::AttackState() {}
AttackState::~AttackState() {}

// Methods

/// <summary>
/// Transforms the NPC to the next state.
/// </summary>
/// <param name="npc">The NPC</param>
void AttackState::Transform(NPC* npc)
{
	// TODO: COMPLETE
}

/// <summary>
/// Enters the NPC's state.
/// </summary>
/// <param name="npc">The NPC</param>
void AttackState::OnEnter(NPC* npc) { ((Attacker*)npc)->SetIsAttacking(true); }

/// <summary>
/// Exits the NPC's state.
/// </summary>
/// <param name="attacker">The attacker</param>
void AttackState::OnExit(NPC* npc) { ((Attacker*)npc)->SetIsAttacking(false); }