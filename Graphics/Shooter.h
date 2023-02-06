#pragma once

// Includes
#include "NPC.h"

class Shooter : public NPC
{
private:
	// Fields
	NPC* target;
	int arms, previousCellContent;
	bool isSearchingCourier, isSearchingEnemy, isAttacking;

public:
	// Properties
	NPC* GetTarget();
	void SetTarget(NPC* t);

	int GetArms();
	void SetArms(int a);

	int GetPreviousCellContent();
	void SetPreviousCellContent(int c);

	bool IsSearchingCourier();
	void SetIsSearchingCourier(bool i);

	bool IsSearchingEnemy();
	void SetIsSearchingEnemy(bool i);

	bool IsAttacking();
	void SetIsAttacking(bool i);

	// Constructors & Destructors
	Shooter();
	Shooter(Position p, int t, int r);
	~Shooter();

	// Methods
	void TakeArms();
	void Shoot(Position attackPosition);
	void ThrowGranade(Position attackPosition);
	void Attack(Position attackPosition);
};