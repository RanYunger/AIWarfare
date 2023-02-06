// Includes
#include "SelfHealState.h"

#include "SearchShooterState.h"
#include "CollectSuppliesState.h"
#include "Courier.h"

// Properties

// Constuctors & Destructors
SelfHealState::SelfHealState() {}
SelfHealState::~SelfHealState() {}

// Methods
void SelfHealState::Transform(NPC* npc)
{
	Courier* courier = (Courier*)npc;
	State* nextState = (courier->GetMeds() > 0) || (courier->GetArms() > 0) ? (State*)new SearchShooterState()
		: (State*)new CollectSuppliesState();

	OnExit(npc);

	courier->SetState(nextState);
	courier->GetState()->OnEnter(npc);
}

void SelfHealState::OnEnter(NPC* npc) { ((Courier*)npc)->SetIsSelfHealing(true); }

void SelfHealState::OnExit(NPC* npc) { ((Courier*)npc)->SetIsSelfHealing(false); }