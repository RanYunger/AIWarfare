#pragma once

// Includes
#include "NPC.h"

class Shooter : public NPC
{
private:
	// Fields
	int previousCellContent;
	bool isSearchingEnemy, isAttacking;

public:
	// Properties
	int GetPreviousCellContent();
	void SetPreviousCellContent(int c);

	bool IsSearchingEnemy();
	void SetIsSearchingEnemy(bool i);

	bool IsAttacking();
	void SetIsAttacking(bool i);

	// Constructors & Destructors
	Shooter();
	Shooter(Position l, int t, int r);
	~Shooter();

	// Methods
	void Shoot(Position destination);
	void ThrowGranade(Position destination);
	void Attack(Position destination);
};