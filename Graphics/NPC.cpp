// Includes
#include "NPC.h"

#include "Settings.h"

// Properties
Position NPC::GetPosition() { return position; }
void NPC::SetPosition(Position p) { position = p; }

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

// Constructors & Destructors
NPC::NPC() {}
NPC::NPC(Position p, int t, int r)
{
	SetPosition(p);
	SetHealth(MAX_HEALTH);
	SetTeam(t);
	SetRoom(r);
}
NPC::~NPC() {}

// Methods

/// <summary>
/// Activates the NPC by its current state.
/// </summary>
void Act() {}

/// <summary>
/// Inflicts damage to the NPC's health.
/// </summary>
/// <param name="damage">The damage to inflict</param>
void NPC::TakeDamage(int damage) { SetHealth(health - damage <= 0 ? 0 : health - damage); }