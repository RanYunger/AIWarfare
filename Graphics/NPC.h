#pragma once

// Includes
#include "Position.h"
#include "State.h"

class State;

class NPC
{
private:
	// Fields
	Position position, destination;
	State* state;
	int health, team, room;

public:
	// Properties
	Position GetPosition();
	void SetPosition(Position p);

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

	// Constructors & Destructors
	NPC();
	NPC(Position p, int t, int r);
	~NPC();

	// Methods
	void TakeDamage(int damage);
};