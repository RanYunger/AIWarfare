#pragma once

// Includes
#include "Shooter.h"
#include "Position.h"
#include "Bullet.h"
#include "Settings.h"

class Grenade
{
private:
	// Fields
	Shooter owner;
	Position location;
	Bullet* shards[SHARDS_IN_GRENADE];
public:
	// Properties
	Shooter GetOwner();
	void SetOwner(Shooter* o);

	Position GetLocation();
	void SetLocation(Position* l);

	// Consturctors & Destructors
	Grenade();
	Grenade(Shooter* o, Position* l);
	~Grenade();

	// Methods
	bool Explode(int map[MAP_DIMENSION][MAP_DIMENSION]);
	void SimulateExplosion(int map[MAP_DIMENSION][MAP_DIMENSION], double securityMap[MAP_DIMENSION][MAP_DIMENSION]);
	void Draw();
};