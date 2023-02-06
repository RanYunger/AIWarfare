// Includes
#include "CollectSuppliesState.h"

#include <stdlib.h>
#include "SelfHealState.h"
#include "SearchShooterState.h"
#include "Courier.h"

// Properties

// Constuctors & Destructors
CollectSuppliesState::CollectSuppliesState() {}
CollectSuppliesState::~CollectSuppliesState() {}

// Methods
void CollectSuppliesState::Transform(NPC* npc)
{
	State* nextState = npc->GetHealth() < (10 + rand() % 10) ? (State*)new SelfHealState() : (State*)new SearchShooterState();

	OnExit(npc);

	npc->SetState(nextState);
	npc->GetState()->OnEnter(npc);
}
void CollectSuppliesState::OnEnter(NPC* npc) { ((Courier*)npc)->SetIsCollectingSupplies(true); }

void CollectSuppliesState::OnExit(NPC* npc) { ((Courier*)npc)->SetIsCollectingSupplies(false); }