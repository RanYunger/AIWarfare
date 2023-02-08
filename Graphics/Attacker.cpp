// Includes
#include "Attacker.h"

#include <math.h>
#include <stdlib.h>
#include "SearchEnemyState.h"
#include "Settings.h"

// Properties
Bullet* Attacker::GetBullet() { return bullet; }
void Attacker::SetBullet(Bullet* b) { bullet = b; }

Grenade* Attacker::GetGrenade() { return grenade; }
void Attacker::SetGrenade(Grenade* g) { grenade = g; }

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
	SetBullet(nullptr);
	SetGrenade(nullptr);
	SetPreviousCellContent(SPACE);
	SetIsSearchingEnemy(false);
	SetIsSearchingShelter(false);
	SetIsAttacking(false);
	SetIsCallingCourier(false);

	SetActiveState((State*)new SearchEnemyState());
	activeState->OnEnter(this);
}
Attacker::~Attacker() {}

// Methods

/// <summary>
/// Shoots a bullet towards a given destination.
/// </summary>
/// <param name="map">The map to simulate the grenade at</param>
/// <param name="securityMap">The security map to simulate the bullet at</param>
/// <param name="angle">The angle the bullet is shot at</param>
void Attacker::ShootBullet(int map[MAP_DIMENSION][MAP_DIMENSION], double securityMap[MAP_DIMENSION][MAP_DIMENSION], double angle)
{
	arms = arms - 1 <= 0 ? 0 : arms - 1;
	
	bullet = new Bullet(location, team, angle);
	bullet->Fire(map, securityMap);
}

/// <summary>
/// Throws a grenade towards a given destination.
/// </summary>
/// <param name="map">The map to simulate the grenade at</param>
/// <param name="securityMap">The security map to simulate the bullet at</param>
/// <param name="angle">The angle the granade is thrown at</param>
void Attacker::ThrowGranade(int map[MAP_DIMENSION][MAP_DIMENSION], double securityMap[MAP_DIMENSION][MAP_DIMENSION], double angle)
{
	arms = arms - 1 <= 0 ? 0 : arms - 1;

	grenade = new Grenade(location, team, angle);
	grenade->Explode(map, securityMap);
}

/// <summary>
/// Activates a random attack.
/// </summary>
/// <param name="destination">The position the attack is heading to</param>
/// <param name="map">The map to simulate the grenade at</param>
/// <param name="securityMap">The security map to simulate the bullet at</param>
void Attacker::Attack(Position destination, int map[MAP_DIMENSION][MAP_DIMENSION], double securityMap[MAP_DIMENSION][MAP_DIMENSION])
{
	double dRow = destination.GetRow() - location.GetRow(), dColumn = destination.GetColumn() - location.GetColumn();
	double angle = atan(dRow / dColumn);
	bool isShootingBullet = rand() % 2 == 0;

	if ((isShootingBullet) && (bullet == nullptr))
		ShootBullet(map, securityMap, angle);
	else if ((!isShootingBullet) && (grenade == nullptr))
		ThrowGranade(map, securityMap, angle);
}

/// <summary>
/// Indicates whether there's a line of sight between the attacker and another NPC.
/// </summary>
/// <param name="npc">The other npc</param>
/// <param name="map">The map to check line of sight on</param>
/// <returns>True if there's a line of sight, False otherwise</returns>
bool Attacker::HasLineOfSight(NPC* npc, int map[MAP_DIMENSION][MAP_DIMENSION])
{
	double myRow = location.GetRow(), myColumn = location.GetColumn(),
		otherRow = npc->GetLocation().GetRow(), otherColumn = npc->GetLocation().GetColumn();
	double rowDiff = otherRow - myRow, columnDiff = otherColumn - myColumn;
	double length = sqrt(rowDiff * rowDiff + columnDiff * columnDiff);
	double dRow = rowDiff / length, dColumn = columnDiff / length;

	for (double row = myRow, column = myColumn; ((int)row != otherRow) && ((int)column != otherColumn); row += dRow, column += dColumn)
		if (map[(int)round(row)][(int)round(column)] == WALL)
			return false;

	return true;
}