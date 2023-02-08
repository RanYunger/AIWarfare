// Includes
#include "ShareSupplyState.h"

// Properties

// Constructors & Destructors
ShareSupplyState::ShareSupplyState() {}
ShareSupplyState::~ShareSupplyState() {}

// Methods

/// <summary>
/// Transforms the NPC to the next state.
/// </summary>
/// <param name="npc">The NPC</param>
void ShareSupplyState::Transform(NPC* npc)
{
	// TODO: COMPLETE
}

/// <summary>
/// Enters the NPC's state.
/// </summary>
/// <param name="npc">The NPC</param>
void ShareSupplyState::OnEnter(NPC* npc) { ((Courier*)npc)->SetIsSharingSupply(true); }

/// <summary>
/// Exits the NPC's state.
/// </summary>
/// <param name="attacker">The attacker</param>
void ShareSupplyState::OnExit(NPC* npc) { ((Courier*)npc)->SetIsSharingSupply(false); }