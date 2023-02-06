// Includes
#include "SearchEnemyState.h"

#include <stdlib.h>
#include "SearchCourierState.h"
#include "Shooter.h"
#include "AttackState.h"

// Properties

// Constuctors & Destructors
SearchEnemyState::SearchEnemyState() {}
SearchEnemyState::~SearchEnemyState() {}

// Methods
void SearchEnemyState::Transform(NPC* npc)
{
	State* nextState = npc->GetHealth() < (10 + rand() % 10) ? (State*)new SearchCourierState() : (State*)new AttackState();
	OnExit(npc);

	npc->SetState(nextState);
	npc->GetState()->OnEnter(npc);
}

void SearchEnemyState::OnEnter(NPC* npc) { ((Shooter*)npc)->SetIsSearchingEnemy(true); }

void SearchEnemyState::OnExit(NPC* npc) { ((Shooter*)npc)->SetIsSearchingEnemy(false); }