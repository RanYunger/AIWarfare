// Includes
#include "Attacker.h"

#include <vector>
#include <math.h>
#include <stdlib.h>
#include "SearchEnemyState.h"
#include "Settings.h"

#include <string>
using namespace std;

// Properties
int Attacker::GetSteppedOn() { return steppedOn; }
void Attacker::SetSteppedOn(int s) { steppedOn = s; }

bool Attacker::IsSearchingEnemy() { return isSearchingEnemy; }
void Attacker::SetIsSearchingEnemy(bool i) { isSearchingEnemy = i; }

bool Attacker::IsSearchingShelter() { return isSearchingShelter; }
void Attacker::SetIsSearchingShelter(bool i) { isSearchingShelter = i; }

// Constructors & Destructors
Attacker::Attacker()
{
	SetSteppedOn(SPACE);
	SetIsSearchingEnemy(false);
	SetIsSearchingShelter(false);

	SetActiveState((State*)new SearchEnemyState());
	GetActiveState()->OnEnter(this);
}
Attacker::Attacker(Position l, int t, int r, int a, int m)
	: NPC(l, t, r, a, m)
{
	SetSteppedOn(SPACE);
	SetIsSearchingEnemy(false);
	SetIsSearchingShelter(false);

	SetActiveState((State*)new SearchEnemyState());
	GetActiveState()->OnEnter(this);
}
Attacker::~Attacker() {}

// Methods

/// <summary>
/// Calls a teammate courier for resupply.
/// </summary>
/// <param name="courier">The teammate courier to call</param>
/// <param name="supply">The supply the courier is called for</param>
/// <param name="transaction">The type of transaction (give or take) to be made with the courier</param>
void Attacker::CallCourier(Courier* courier, int supply, int transaction)
{
	courier->SetIsCalled(true);
	courier->SetSupply(supply);
	courier->SetTransaction(transaction == GIVE ? TAKE : GIVE);
}

/// <summary>
/// Activates a random weapon.
/// </summary>
/// <param name="destination">The position the attack is heading to</param>
/// <returns>The spawned weapon</returns>
Weapon* Attacker::Attack(Position destination)
{
	double angle = (10.0 +(rand() % 20)) + atan2(fabs(location.GetColumn() - destination.GetColumn()), fabs(location.GetRow() - destination.GetRow())) * (180 / PI);

	arms = arms - 1 <= 0 ? 0 : arms - 1;

	return new Weapon(location, angle, team, rand() % 2 == 0);
}

/// <summary>
/// Indicates whether there's a line of sight between the attacker and another NPC.
/// </summary>
/// <param name="enemyNPC">The enemy npc</param>
/// <param name="map">The map to check line of sight on</param>
/// <returns>True if there's a line of sight, False otherwise</returns>
bool Attacker::HasLineOfSight(NPC enemyNPC, int map[MAP_DIMENSION][MAP_DIMENSION])
{
	double myRow, myColumn, otherRow, otherColumn;
	double x, y, dx, dy, err, err2;
	int sx, sy;

	// Validation
	if (location == enemyNPC.GetLocation())
		return true;

	myRow = location.GetRow();
	myColumn = location.GetColumn();
	otherRow = enemyNPC.GetLocation().GetRow();
	otherColumn = enemyNPC.GetLocation().GetColumn();

	x = myRow;
	dx = fabs(otherRow - myRow);
	y = myColumn;
	dy = fabs(otherColumn - myColumn);

	err = dx - dy;

	sx = (myRow < otherRow) ? 1 : -1;
	sy = (myColumn < otherColumn) ? 1 : -1;

	while (true)
	{
		if (map[(int)x][(int)y] == WALL)
			return false; // Line of sight is blocked
		if ((fabs(x - otherRow) < 0.1) && (fabs(y - otherColumn) < 0.1))
			return true; // Line of sight is unblocked

		err2 = 2 * err;
		if (err2 > -dy)
		{
			err -= dy;
			x += sx;
		}
		if (err2 < dx)
		{
			err += dx;
			y += sy;
		}
	}

	return true;
}

/// <summary>
/// Indicates whether the attacker can attack an enemy NPC.
/// </summary>
/// <param name="enemyNPC">The enemy NPC to attack</param>
/// <param name="map">The map to check Line of Sight</param>
/// <returns>True if the attacker can attack the enemy NPC, False otherwise</returns>
bool Attacker::CanAttack(NPC enemyNPC, int map[MAP_DIMENSION][MAP_DIMENSION])
{
	return (room != -1) && (room == enemyNPC.GetRoom()) && (HasLineOfSight(enemyNPC, map)) && (arms > 0);
}

string Attacker::GetStateName()
{
	if (isSearchingEnemy)
		return "Searching Enemy State";
	if (isSearchingShelter)
		return "Searching Shelter State";

	return "ERROR - no state detected";
}