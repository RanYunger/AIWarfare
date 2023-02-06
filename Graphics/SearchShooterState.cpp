// Includes
#include "SearchShooterState.h"

#include <stdlib.h>
#include "CollectSuppliesState.h"
#include "SelfHealState.h"
#include "Courier.h"

// Properties

// Constuctors & Destructors
SearchShooterState::SearchShooterState() {}
SearchShooterState::~SearchShooterState() {}

// Methods
void SearchShooterState::Transform(NPC* npc)
{
	State* nextState = npc->GetHealth() < (10 + rand() % 10) ? (State*)new SelfHealState() : (State*)new CollectSuppliesState();

	OnExit(npc);

	npc->SetState(nextState);
	npc->GetState()->OnEnter(npc);
}

void SearchShooterState::OnEnter(NPC* npc) { ((Courier*)npc)->SetIsSearchingShooter(true); }

void SearchShooterState::OnExit(NPC* npc) { ((Courier*)npc)->SetIsSearchingShooter(false); }