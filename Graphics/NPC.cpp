// Includes
#include "NPC.h"

#include <math.h>
#include "Settings.h"

// Properties
Position NPC::GetLocation() { return location; }
void NPC::SetLocation(Position l) { location = l; }

Position NPC::GetDestination() { return destination; }
void NPC::SetDestination(Position d) { destination = d; }

State* NPC::GetState() { return state; }
void NPC::SetState(State* s) { state = s; }

int NPC::GetHealth() { return health; }
void NPC::SetHealth(int h) { health = h; }

int NPC::GetTeam() { return team; }
void NPC::SetTeam(int t) { team = t; }

int NPC::GetRoom() { return room; }
void NPC::SetRoom(int r) { room = r; }

int NPC::GetArms() { return arms; }
void NPC::SetArms(int a) { arms = a; }

int NPC::GetMeds() { return meds; }
void NPC::SetMeds(int m) { meds = m; }

// Constructors & Destructors
NPC::NPC()
{
	Position* defaultPosition = new Position(-1, -1);

	SetLocation(*defaultPosition);
	SetHealth(-1);
	SetTeam(-1);
	SetRoom(-1);
}
NPC::NPC(Position l, int t, int r)
{
	SetLocation(l);
	SetHealth(MAX_HEALTH);
	SetTeam(t);
	SetRoom(r);
}
NPC::~NPC() {}

// Methods

/// <summary>
/// Inflicts damage to the NPC's health.
/// </summary>
/// <param name="damage">The damage to inflict</param>
void NPC::TakeDamage(int damage) { SetHealth(health - damage <= 0 ? 0 : health - damage); }

/// <summary>
/// Gives arms to a teammate.
/// </summary>
/// <param name="teammate">The teammate who takes the arms</param>
void NPC::GiveArms(NPC* teammate)
{
	teammate->arms = teammate->arms + 1 >= MAX_ARMS ? MAX_ARMS : teammate->arms + 1;
	arms = arms - 1 <= 0 ? 0 : arms - 1;
}

/// <summary>
/// Take arms from a teammate.
/// </summary>
/// <param name="teammate">The teammate to take arms from</param>
void NPC::TakeArms(NPC* teammate)
{
	arms = arms + 1 >= MAX_ARMS ? MAX_ARMS : arms + 1;
	teammate->arms = teammate->arms - 1 <= 0 ? 0 : teammate->arms - 1;
}

/// <summary>
/// Gives meds to a teammate.
/// </summary>
/// <param name="teammate">The teammate who takes the meds</param>
void NPC::GiveMeds(NPC* teammate)
{
	teammate->meds = teammate->meds + 1 >= MAX_MEDS ? MAX_MEDS : teammate->meds + 1;
	meds = meds - 1 <= 0 ? 0 : meds - 1;
}

/// <summary>
/// Take meds from a teammate.
/// </summary>
/// <param name="teammate">The teammate to take meds from</param>
void NPC::TakeMeds(NPC* teammate)
{
	meds = meds + 1 >= MAX_MEDS ? MAX_MEDS : meds + 1;
	teammate->meds = teammate->meds - 1 <= 0 ? 0 : teammate->meds - 1;
}

/// <summary>
/// Indicates whether there's a line of sight between two locations.
/// </summary>
/// <param name="p1">The first location</param>
/// <param name="p2">The second location</param>
/// <returns>True if there's a line of sight between the two locations, False otherwise</returns>
bool NPC::HasLineOfSight(int map[MAP_DIMENSION][MAP_DIMENSION], NPC* other)
{
	int myRow = (int)location.GetRow(), myColumn = (int)location.GetColumn();
	int otherRow = (int)other->location.GetRow(), otherColumn = (int)other->location.GetColumn();
	int rowDiff = otherRow - myRow, columnDiff = otherColumn - myColumn;
	double length = sqrt(rowDiff * rowDiff + columnDiff * columnDiff);
	double dRow = rowDiff / length, dColumn = columnDiff / length;

	for (double row = myRow, column = myColumn; ((int)row != otherRow) && ((int)column != otherColumn); row += dRow, column += dColumn)
		if (map[(int)round(row)][(int)round(column)] == WALL)
			return false;

	return true;
}
