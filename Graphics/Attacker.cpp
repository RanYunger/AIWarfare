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
/// Activates a random attack.
/// </summary>
/// <param name="destination">The position the attack is heading to</param>
/// <param name="shotBullet">The shot bullet (if randomized)</param>
/// <param name="thrownGrenade">The thrown grenade (if randomized)</param>
void Attacker::Attack(Position destination, Bullet** shotBullet, Grenade** thrownGrenade)
{
	// TODO: FIX (angle calculation)
	double angle = atan2(fabs(location.GetRow() - destination.GetRow()), fabs(location.GetColumn() - destination.GetColumn())) * (180 / PI);

	// Spawns a random bullet / grenade
	if (rand() % 2 == 0)
		*shotBullet = new Bullet(location, angle, team);
	else
		*thrownGrenade = new Grenade(location, destination, angle, team);

	arms = arms - 1 <= 0 ? 0 : arms - 1;
}

/// <summary>
/// Indicates whether there's a line of sight between the attacker and another NPC.
/// </summary>
/// <param name="npc">The other npc</param>
/// <param name="map">The map to check line of sight on</param>
/// <returns>True if there's a line of sight, False otherwise</returns>
bool Attacker::HasLineOfSight(NPC npc, int map[MAP_DIMENSION][MAP_DIMENSION])
{
	double myRow = location.GetRow(), myColumn = location.GetColumn(),
		otherRow = npc.GetLocation().GetRow(), otherColumn = npc.GetLocation().GetColumn();
	double rowDiff = otherRow - myRow, columnDiff = otherColumn - myColumn;
	double length = sqrt(rowDiff * rowDiff + columnDiff * columnDiff);
	double dRow = rowDiff / length, dColumn = columnDiff / length;

	for (double row = myRow, column = myColumn; ((int)row != otherRow) && ((int)column != otherColumn); row += dRow, column += dColumn)
		if (map[(int)round(row)][(int)round(column)] == WALL)
			return false;

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