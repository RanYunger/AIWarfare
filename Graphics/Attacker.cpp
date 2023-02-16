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

bool Attacker::IsAttacking() { return isAttacking; }
void Attacker::SetIsAttacking(bool i) { isAttacking = i; }

// Constructors & Destructors
Attacker::Attacker()
	: NPC()
{
	SetSteppedOn(SPACE);
	SetIsSearchingEnemy(false);
	SetIsSearchingShelter(false);
	SetIsAttacking(false);

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
	double angle = atan2(fabs(location.GetColumn() - destination.GetColumn()), fabs(location.GetRow() - destination.GetRow())) * (180 / PI);

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
	double myRow = location.GetRow(), myColumn = location.GetColumn(),
		otherRow = enemyNPC.GetLocation().GetRow(), otherColumn = enemyNPC.GetLocation().GetColumn();
	double x = myRow, dx = abs(otherRow - myRow);
	double y = myColumn, dy = abs(otherColumn - myColumn);
	int sx = (myRow < otherRow) ? 1 : -1;
	int sy = (myColumn < otherColumn) ? 1 : -1;
	double err = dx - dy, err2;

	while (true)
	{
		if (map[(int)x][(int)y] == WALL)
			return false; // Line of sight is blocked
		if ((abs(x - otherRow) < 0.1) && (abs(y - otherColumn) < 0.1))
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
/// Indicates whether any of the attacker's allies requires a supply.
/// </summary>
/// <param name="attackers">The attackers of both teams</param>
/// <param name="supply">The supply to check if any of the allies needs</param>
/// <returns>True if the supply is required, False otherwise</returns>
bool Attacker::IsAllyRequiresSupply(Attacker* attackers, int supply)
{
	for (int i = 0; i < ATTACKERS_IN_TEAM; i++)
	{
		// Validation
		if ((attackers[i].GetHealth() == 0) || (attackers[i].GetTeam() != team))
			continue;

		if ((supply == ARMS) && (attackers[i].GetArms() == 0))
			return true;
		if ((supply == MEDS) && (attackers[i].GetMeds() == 0))
			return true;
	}

	return false;
}

string Attacker::GetStateName()
{
	if (isSearchingEnemy)
		return "Searching Enemy State";
	if (isAttacking)
		return "Attacking State";
	if (isSearchingShelter)
		return "Searching Shelter State";

	return "ERROR - no state detected";
}