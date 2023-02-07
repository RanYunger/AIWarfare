#pragma once

// Includes
#include "Attacker.h"
#include "Position.h"
#include "Bullet.h"
#include "Settings.h"

class Grenade
{
private:
	// Fields
	Attacker owner;
	Position location;
	Bullet* shards[SHARDS_IN_GRENADE];
public:
	// Properties
	Attacker GetOwner();
	void SetOwner(Attacker* o);

	Position GetLocation();
	void SetLocation(Position* l);

	// Consturctors & Destructors
	Grenade();
	Grenade(Attacker* o, Position* l);
	~Grenade();

	// Methods
	bool Explode(int map[MAP_DIMENSION][MAP_DIMENSION]);
	void SimulateExplosion(int map[MAP_DIMENSION][MAP_DIMENSION], double securityMap[MAP_DIMENSION][MAP_DIMENSION]);
	void Draw();
};