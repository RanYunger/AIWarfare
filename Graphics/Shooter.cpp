// Includes
#include "Shooter.h"

#include <stdlib.h>
#include "Bullet.h"
#include "Grenade.h"
#include "SearchEnemyState.h"
#include "Settings.h"

// Properties
NPC* Shooter::GetTarget() { return target; }
void Shooter::SetTarget(NPC* t) { target = t; }

int Shooter::GetArms() { return arms; }
void Shooter::SetArms(int a) { arms = a; }

int Shooter::GetPreviousCellContent() { return previousCellContent; }
void Shooter::SetPreviousCellContent(int c) { previousCellContent = c; }

bool Shooter::IsSearchingCourier() { return isSearchingCourier; }
void Shooter::SetIsSearchingCourier(bool i) { isSearchingCourier = i; }

bool Shooter::IsSearchingEnemy() { return isSearchingEnemy; }
void Shooter::SetIsSearchingEnemy(bool i) { isSearchingEnemy = i; }

bool Shooter::IsAttacking() { return isAttacking; }
void Shooter::SetIsAttacking(bool i) { isAttacking = i; }

// Constructors & Destructors
Shooter::Shooter()
	: NPC() {}
Shooter::Shooter(Position p, int t, int r)
	: NPC(p, t, r)
{
	SetTarget(nullptr);
	SetArms(MAX_ARMS);
	SetPreviousCellContent(SPACE);

	SetState((State*)new SearchEnemyState());
	GetState()->OnEnter(this);
}
Shooter::~Shooter() {}

// Methods

/// <summary>
/// Increases the arms counter.
/// </summary>
void Shooter::TakeArms() { SetArms(arms + 1 >= MAX_ARMS ? MAX_ARMS : arms + 1); }

/// <summary>
/// Shoots a bullet and decreases the arms counter.
/// </summary>
void Shooter::Shoot(Position attackPosition)
{
	// TODO: COMPLETE
	//Bullet* b = new Bullet();

	SetArms(arms - 1 <= 0 ? 0 : arms - 1);
}

/// <summary>
/// Throws a granade and decreases the arms counter.
/// </summary>
void Shooter::ThrowGranade(Position attackPosition)
{
	// TODO: COMPLETE
	//Grenade* g = new Grenade();

	SetArms(arms - 1 <= 0 ? 0 : arms - 1);
}

/// <summary>
/// Activates a random attack.
/// </summary>
/// <param name="attackPosition">The position the attack is heading to</param>
void Shooter::Attack(Position attackPosition)
{
	if (rand() % 2)
		Shoot(attackPosition);
	else
		ThrowGranade(attackPosition);
}