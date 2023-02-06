// Includes
#include "SearchCourierState.h"

#include "SearchEnemyState.h"
#include "Shooter.h"

// Properties

// Constuctors & Destructors
SearchCourierState::SearchCourierState() {}
SearchCourierState::~SearchCourierState() {}

// Methods
void SearchCourierState::Transform(NPC* npc)
{
	OnExit(npc);

	npc->SetState(new SearchEnemyState());
	npc->GetState()->OnEnter(npc);
}

void SearchCourierState::OnEnter(NPC* npc) { ((Shooter*)npc)->SetIsSearchingCourier(true); }

void SearchCourierState::OnExit(NPC* npc) { ((Shooter*)npc)->SetIsSearchingCourier(false); }