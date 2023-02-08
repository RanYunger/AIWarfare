// Includes
#include "SearchEnemyState.h"

#include "AttackState.h"

// Properties

// Consturctors & Destructors
SearchEnemyState::SearchEnemyState() {}
SearchEnemyState::~SearchEnemyState() {}

// Methods

/// <summary>
/// Transforms the NPC to the next state.
/// </summary>
/// <param name="npc">The NPC</param>
void SearchEnemyState::Transform(NPC* npc)
{
	OnExit(npc);
	npc->SetActiveState((State*)new AttackState());
	npc->GetActiveState()->OnEnter(npc);
}

/// <summary>
/// Enters the NPC's state.
/// </summary>
/// <param name="npc">The NPC</param>
void SearchEnemyState::OnEnter(NPC* npc) { ((Attacker*)npc)->SetIsSearchingEnemy(true); }

/// <summary>
/// Exits the NPC's state.
/// </summary>
/// <param name="attacker">The attacker</param>
void SearchEnemyState::OnExit(NPC* npc) { ((Attacker*)npc)->SetIsSearchingEnemy(false); }