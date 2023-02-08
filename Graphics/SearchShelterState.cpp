// Includes
#include "SearchShelterState.h"

#include "SearchEnemyState.h"

// Properties

// Constructors & Destructors
SearchShelterState::SearchShelterState() {}
SearchShelterState::~SearchShelterState() {}

// Methods

/// <summary>
/// Transforms the NPC to the next state.
/// </summary>
/// <param name="npc">The NPC</param>
void SearchShelterState::Transform(NPC* npc)
{
	OnExit(npc);
	npc->SetActiveState((State*)new SearchEnemyState());
	npc->GetActiveState()->OnEnter(npc);
}

/// <summary>
/// Enters the NPC's state.
/// </summary>
/// <param name="npc">The NPC</param>
void SearchShelterState::OnEnter(NPC* npc) { ((Attacker*)npc)->SetIsSearchingShelter(true); }

/// <summary>
/// Exits the NPC's state.
/// </summary>
/// <param name="attacker">The attacker</param>
void SearchShelterState::OnExit(NPC* npc) { ((Attacker*)npc)->SetIsSearchingShelter(false); }