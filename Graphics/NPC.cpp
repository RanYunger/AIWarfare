// Includes
#include "NPC.h"

#include <math.h>
#include "Settings.h"

// Properties
Position NPC::GetLocation() { return location; }
void NPC::SetLocation(Position l) { location = l; }

Position NPC::GetDestination() { return destination; }
void NPC::SetDestination(Position d) { destination = d; }

State* NPC::GetActiveState() { return activeState; }
void NPC::SetActiveState(State* s) { activeState = s; }

State* NPC::GetInterruptedState() { return interruptedState; }
void NPC::SetInterruptedState(State* s) { interruptedState = s; }

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
	SetDestination(*defaultPosition);
	SetActiveState(nullptr);
	SetInterruptedState(nullptr);
	SetHealth(-1);
	SetTeam(-1);
	SetRoom(-1);
}
NPC::~NPC() {}

// Methods

/// <summary>
/// Inflicts damage to the NPC's health.
/// </summary>
/// <param name="damage">The damage to inflict</param>
void NPC::TakeDamage(int damage) { SetHealth(health - damage <= 0 ? 0 : health - damage); }
