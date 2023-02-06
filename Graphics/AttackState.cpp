// Includes
#include "AttackState.h"

#include "SearchEnemyState.h"
#include "SearchCourierState.h"
#include "Shooter.h"

// Properties

// Constuctors & Destructors
AttackState::AttackState() {}
AttackState::~AttackState() {}

// Methods
void AttackState::Transform(NPC* npc)
{
	Shooter* shooter = (Shooter*)npc;
	State* nextState = shooter->GetTarget() == nullptr ? (State*)new SearchEnemyState() : (State*)new SearchCourierState();

	OnExit(npc);

	npc->SetState(nextState);
	npc->GetState()->OnEnter(npc);
}
void AttackState::OnEnter(NPC* npc) { ((Shooter*)npc)->SetIsAttacking(true); }

void AttackState::OnExit(NPC* npc) { ((Shooter*)npc)->SetIsAttacking(false); }