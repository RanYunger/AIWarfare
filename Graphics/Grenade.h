#pragma once

// Includes
#include "Bullet.h"
#include "Position.h"
#include "Settings.h"

class Grenade
{
private:
	// Fields
	Position location;
	Bullet* shards[SHARDS_IN_GRENADE];
	int team;

public:
	// Properties
	Position GetLocation();
	void SetLocation(Position l);

	int GetTeam();
	void SetTeam(int t);

	// Consturctors & Destructors
	Grenade();
	Grenade(Position l, int t);
	~Grenade();

	// Methods
	bool Move(int map[MAP_DIMENSION][MAP_DIMENSION]);
	void Explode(int map[MAP_DIMENSION][MAP_DIMENSION], int securityMap[MAP_DIMENSION][MAP_DIMENSION]);
	void Draw();
};