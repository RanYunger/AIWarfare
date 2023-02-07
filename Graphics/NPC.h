#pragma once

// Includes
#include "Position.h"
#include "State.h"
#include "Settings.h"

class State;

class NPC
{
protected:
	// Fields
	Position location, destination;
	State* state;
	int health, team, room, arms, meds;

public:
	// Properties
	Position GetLocation();
	void SetLocation(Position l);

	Position GetDestination();
	void SetDestination(Position d);

	State* GetState();
	void SetState(State* s);

	int GetHealth();
	void SetHealth(int h);

	int GetTeam();
	void SetTeam(int t);

	int GetRoom();
	void SetRoom(int r);

	int GetArms();
	void SetArms(int a);

	int GetMeds();
	void SetMeds(int m);

	// Constructors & Destructors
	NPC();
	NPC(Position l, int t, int r);
	~NPC();

	// Methods
	void TakeDamage(int damage);
	
	void GiveArms(NPC* teammate);
	void TakeArms(NPC* teammate);

	void GiveMeds(NPC* teammate);
	void TakeMeds(NPC* teammate);

	bool HasLineOfSight(int map[MAP_DIMENSION][MAP_DIMENSION], NPC* other);
};