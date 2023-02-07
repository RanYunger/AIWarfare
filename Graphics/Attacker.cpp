// Includes
#include "Attacker.h"

#include <math.h>
#include <stdlib.h>
#include "Bullet.h"
#include "Grenade.h"
#include "SearchEnemyState.h"
#include "Settings.h"

// Properties
int Attacker::GetPreviousCellContent() { return previousCellContent; }
void Attacker::SetPreviousCellContent(int c) { previousCellContent = c; }

bool Attacker::IsSearchingEnemy() { return isSearchingEnemy; }
void Attacker::SetIsSearchingEnemy(bool i) { isSearchingEnemy = i; }

bool Attacker::IsSearchingShelter() { return isSearchingShelter; }
void Attacker::SetIsSearchingShelter(bool i) { isSearchingShelter = i; }

bool Attacker::IsAttacking() { return isAttacking; }
void Attacker::SetIsAttacking(bool i) { isAttacking = i; }

bool Attacker::IsCallingCourier() { return isCallingCourier; }
void Attacker::SetIsCallingCourier(bool i) { isCallingCourier = i; }

// Constructors & Destructors
Attacker::Attacker()
	: NPC()
{
	SetPreviousCellContent(SPACE);
	SetIsSearchingEnemy(false);
	SetIsSearchingShelter(false);
	SetIsAttacking(false);
	SetIsCallingCourier(false);

	SetActiveState((State*)new SearchEnemyState());
}
Attacker::~Attacker() {}

// Methods

/// <summary>
/// Shoots a bullet towards a given destination.
/// </summary>
/// <param name="destination">The position the bullet is heading to</param>
void Attacker::ShootBullet(Position destination)
{
	// TODO: COMPLETE
}

/// <summary>
/// Throws a grenade towards a given destination.
/// </summary>
/// <param name="destination">The position the grenade is heading to</param>
void Attacker::ThrowGranade(Position destination)
{
	// TODO: COMPLETE
	//Bullet* bullet = new Bullet(this, &location, (rand() % 360) * PI / 180.0);
}

/// <summary>
/// Activates a random attack.
/// </summary>
/// <param name="destination">The position the attack is heading to</param>
void Attacker::Attack(Position destination)
{
	arms = arms - 1 <= 0 ? 0 : arms - 1;

	if (rand() % 2)
		ShootBullet(destination);
	else
		ThrowGranade(destination);
}

/// <summary>
/// Indicates whether there's a line of sight between the attacker and another NPC.
/// </summary>
/// <param name="npc">The other npc</param>
/// <param name="map">The map to check line of sight on</param>
/// <returns>True if there's a line of sight, False otherwise</returns>
bool Attacker::HasLineOfSight(NPC* npc, int map[MAP_DIMENSION][MAP_DIMENSION])
{
	int myRow = (int)location.GetRow(), myColumn = (int)location.GetColumn();
	int otherRow = (int)npc->GetLocation().GetRow(), otherColumn = (int)npc->GetLocation().GetColumn();
	int rowDiff = otherRow - myRow, columnDiff = otherColumn - myColumn;
	double length = sqrt(rowDiff * rowDiff + columnDiff * columnDiff);
	double dRow = rowDiff / length, dColumn = columnDiff / length;

	for (double row = myRow, column = myColumn; ((int)row != otherRow) && ((int)column != otherColumn); row += dRow, column += dColumn)
		if (map[(int)round(row)][(int)round(column)] == WALL)
			return false;

	return true;
}