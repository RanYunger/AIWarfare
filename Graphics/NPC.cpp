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
	SetHealth(MAX_HEALTH);
	SetTeam(-1);
	SetRoom(-1);
	SetArms(-1);
	SetMeds(-1);
}
NPC::NPC(Position l, int t, int r, int a, int m)
{
	SetLocation(l);
	SetDestination(EMPTY_POSITION);
	SetActiveState(nullptr);
	SetHealth(MAX_HEALTH);
	SetTeam(t);
	SetRoom(r);
	SetArms(a);
	SetMeds(m);
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
		health = health + HEALTH_BOOST >= MAX_HEALTH ? MAX_HEALTH : health + HEALTH_BOOST;
		meds = meds - 1 <= 0 ? 0 : meds - 1;
	}
}