// Includes
#include "SearchSupplyState.h"

// Properties

// Constructors & Destructors
SearchSupplyState::SearchSupplyState() {}
SearchSupplyState::~SearchSupplyState() {}

// Methods

/// <summary>
/// Transforms the NPC to the next state.
/// </summary>
/// <param name="npc">The NPC</param>
void SearchSupplyState::Transform(NPC* npc)
{
	// TODO: COMPLETE
}

/// <summary>
/// Enters the NPC's state.
/// </summary>
/// <param name="npc">The NPC</param>
void SearchSupplyState::OnEnter(NPC* npc) { ((Courier*)npc)->SetIsSearchingSupply(true); }

/// <summary>
/// Exits the NPC's state.
/// </summary>
/// <param name="attacker">The attacker</param>
void SearchSupplyState::OnExit(NPC* npc) { ((Courier*)npc)->SetIsSearchingSupply(false); }