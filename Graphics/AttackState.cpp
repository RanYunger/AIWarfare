// Includes
#include "AttackState.h"

#include <stdlib.h>
#include "SearchEnemyState.h"
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
	State* nextState = npc->GetHealth() <= 10 + rand() % (HEALTH_BOOST - 10) ? (State*)new SearchShelterState() : (State*)new SearchEnemyState();

	OnExit(npc);
	npc->SetActiveState(nextState);
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