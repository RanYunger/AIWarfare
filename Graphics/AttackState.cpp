// Includes
#include "AttackState.h"

#include "SearchShelterState.h"

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
	OnExit(npc);
	npc->SetActiveState((State*)new SearchShelterState());
	npc->GetActiveState()->OnEnter(npc);
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