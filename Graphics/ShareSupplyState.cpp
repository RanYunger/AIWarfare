// Includes
#include "ShareSupplyState.h"

#include "SearchAllyState.h"
#include "SearchSupplyState.h"

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
	State* nextState = ((Courier*)npc)->IsCalled() ? (State*)new SearchAllyState() : (State*)new SearchSupplyState();

	OnExit(npc);
	npc->SetActiveState(nextState);
	npc->GetActiveState()->OnEnter(npc);
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