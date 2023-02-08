// Includes
#include "SearchShelterState.h"

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
	// TODO: COMPLETE
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