#pragma once

// Includes
#include "Courier.h"
#include "NPC.h"
#include "Weapon.h"

#include <string>
using namespace std;

class Courier;

class Attacker : public NPC
{
private:
	// Fields
	int steppedOn;
	bool isSearchingEnemy, isSearchingShelter, isAttacking;

public:
	// Properties
	int GetSteppedOn();
	void SetSteppedOn(int s);

	bool IsSearchingEnemy();
	void SetIsSearchingEnemy(bool i);

	bool IsSearchingShelter();
	void SetIsSearchingShelter(bool i);

	bool IsAttacking();
	void SetIsAttacking(bool i);

	string GetStateName();

	// Constructors & Destructors
	Attacker();
	~Attacker();

	// Methods
	void CallCourier(Courier* courier, int supply, int transaction);
	Weapon* Attack(Position destination);

	bool HasLineOfSight(NPC enemyNPC, int map[MAP_DIMENSION][MAP_DIMENSION]);
	bool IsAllyRequiresSupply(Attacker* attackers, int supply);
};