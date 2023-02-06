#pragma once

// Includes
#include "Bullet.h"
#include "Position.h"
#include "Settings.h"

class Grenade
{
private:
	// Fields
	Position position;
	Bullet* shards[SHARDS_IN_GRENADE];
public:
	// Properties
	Position GetPosition();
	void SetPosition(Position* p);

	// Consturctors & Destructors
	Grenade();
	Grenade(Position* p);
	~Grenade();

	// Methods
	bool Explode(int map[MAP_DIMENSION][MAP_DIMENSION]);
	void SimulateExplosion(int map[MAP_DIMENSION][MAP_DIMENSION], double securityMap[MAP_DIMENSION][MAP_DIMENSION]);
	void Draw();
};