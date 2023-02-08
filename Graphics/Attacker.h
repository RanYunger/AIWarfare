#pragma once

// Includes
#include "Bullet.h"
#include "Grenade.h"
#include "NPC.h"

class Attacker : public NPC
{
private:
	// Fields
	Bullet* bullet;
	Grenade* grenade;
	int previousCellContent;
	bool isSearchingEnemy, isSearchingShelter, isAttacking, isCallingCourier;

public:
	// Properties
	Bullet* GetBullet();
	void SetBullet(Bullet* b);

	Grenade* GetGrenade();
	void SetGrenade(Grenade* g);

	int GetPreviousCellContent();
	void SetPreviousCellContent(int c);

	bool IsSearchingEnemy();
	void SetIsSearchingEnemy(bool i);

	bool IsSearchingShelter();
	void SetIsSearchingShelter(bool i);

	bool IsAttacking();
	void SetIsAttacking(bool i);

	bool IsCallingCourier();
	void SetIsCallingCourier(bool i);

	// Constructors & Destructors
	Attacker();
	~Attacker();

	// Methods
	void ShootBullet(int map[MAP_DIMENSION][MAP_DIMENSION], double securityMap[MAP_DIMENSION][MAP_DIMENSION], double angle);
	void ThrowGranade(int map[MAP_DIMENSION][MAP_DIMENSION], double securityMap[MAP_DIMENSION][MAP_DIMENSION], double angle);
	void Attack(Position destination, int map[MAP_DIMENSION][MAP_DIMENSION], double securityMap[MAP_DIMENSION][MAP_DIMENSION]);

	bool HasLineOfSight(NPC* npc, int map[MAP_DIMENSION][MAP_DIMENSION]);
};