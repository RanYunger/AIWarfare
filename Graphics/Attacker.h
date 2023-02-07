#pragma once

// Includes
#include "NPC.h"

class Attacker : public NPC
{
private:
	// Fields
	int previousCellContent;
	bool isSearchingEnemy, isSearchingShelter, isAttacking, isCallingCourier;

public:
	// Properties
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
	void ShootBullet(Position destination);
	void ThrowGranade(Position destination);
	void Attack(Position destination);

	bool HasLineOfSight(NPC* npc, int map[MAP_DIMENSION][MAP_DIMENSION]);
};