// Includes
#include "NPC.h"

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
	SetLocation(EMPTY_POSITION);
	SetDestination(EMPTY_POSITION);
	SetActiveState(nullptr);
	SetInterruptedState(nullptr);
	SetHealth(MAX_HEALTH);
	SetTeam(-1);
	SetRoom(-1);
	SetArms(MAX_ARMS);
	SetMeds(MAX_MEDS);
}
NPC::~NPC() {}

// Methods

/// <summary>
/// Inflicts damage to the NPC's health.
/// </summary>
/// <param name="damage">The damage to inflict</param>
void NPC::TakeDamage(int damage)
{
	health = health - damage <= 0 ? 0 : health - damage;

	if (meds > 0)
	{
		health += HEALTH_BOOST;
		meds--;
	}
}
