// Includes
#include "Shooter.h"

#include <math.h>
#include <stdlib.h>
#include "Bullet.h"
#include "Grenade.h"
#include "Settings.h"

// Properties
int Shooter::GetPreviousCellContent() { return previousCellContent; }
void Shooter::SetPreviousCellContent(int c) { previousCellContent = c; }

bool Shooter::IsSearchingEnemy() { return isSearchingEnemy; }
void Shooter::SetIsSearchingEnemy(bool i) { isSearchingEnemy = i; }

bool Shooter::IsAttacking() { return isAttacking; }
void Shooter::SetIsAttacking(bool i) { isAttacking = i; }

// Constructors & Destructors
Shooter::Shooter()
	: NPC() {}
Shooter::Shooter(Position l, int t, int r)
	: NPC(l, t, r)
{
	SetArms(MAX_ARMS);
	SetMeds(MAX_MEDS);

	SetPreviousCellContent(SPACE);

	SetState(nullptr); //(State*)new SearchEnemyState());
	//GetState()->OnEnter(this);
}
Shooter::~Shooter() {}

// Methods

/// <summary>
/// Shoots a bullet towards a given destination.
/// </summary>
/// <param name="destination">The position the bullet is heading to</param>
void Shooter::Shoot(Position destination)
{
	// TODO: COMPLETE
}

/// <summary>
/// Throws a grenade towards a given destination.
/// </summary>
/// <param name="destination">The position the grenade is heading to</param>
void Shooter::ThrowGranade(Position destination)
{
	// TODO: COMPLETE
	//Bullet* bullet = new Bullet(this, &location, (rand() % 360) * PI / 180.0);
}

/// <summary>
/// Activates a random attack.
/// </summary>
/// <param name="destination">The position the attack is heading to</param>
void Shooter::Attack(Position destination)
{
	arms = arms - 1 <= 0 ? 0 : arms -1;

	if (rand() % 2)
		Shoot(destination);
	else
		ThrowGranade(destination);
}