#pragma once

// Includes
#include "Bullet.h"
#include "Courier.h"
#include "Grenade.h"
#include "NPC.h"

#include <string>
using namespace std;

class Courier;

class Attacker : public NPC
{
private:
	// Fields
	Bullet* bullet;
	Grenade* grenade;
	int steppedOn;
	bool isSearchingEnemy, isSearchingShelter, isAttacking;

public:
	// Properties
	Bullet* GetBullet();
	void SetBullet(Bullet* b);

	Grenade* GetGrenade();
	void SetGrenade(Grenade* g);

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

	void ShootBullet(double angle, int map[MAP_DIMENSION][MAP_DIMENSION], double securityMap[MAP_DIMENSION][MAP_DIMENSION]);
	void ThrowGranade(Position destination, int map[MAP_DIMENSION][MAP_DIMENSION], double securityMap[MAP_DIMENSION][MAP_DIMENSION]);
	void Attack(Position destination, int map[MAP_DIMENSION][MAP_DIMENSION], double securityMap[MAP_DIMENSION][MAP_DIMENSION]);

	bool HasLineOfSight(NPC npc, int map[MAP_DIMENSION][MAP_DIMENSION]);
	bool IsAllyRequiresSupply(Attacker* attackers, int supply);
};