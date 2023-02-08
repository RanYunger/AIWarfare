// Includes
#include "SearchAllyState.h"

// Properties

// Constructors & Destructors
SearchAllyState::SearchAllyState() {}
SearchAllyState::~SearchAllyState() {}

// Methods

/// <summary>
/// Transforms the NPC to the next state.
/// </summary>
/// <param name="npc">The NPC</param>
void SearchAllyState::Transform(NPC* npc)
{
	// TODO: COMPLETE
}

/// <summary>
/// Enters the NPC's state.
/// </summary>
/// <param name="npc">The NPC</param>
void SearchAllyState::OnEnter(NPC* npc) { ((Courier*)npc)->SetIsSearchingAlly(true); }

/// <summary>
/// Exits the NPC's state.
/// </summary>
/// <param name="attacker">The attacker</param>
void SearchAllyState::OnExit(NPC* npc) { ((Courier*)npc)->SetIsSearchingAlly(false); }