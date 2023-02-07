// Includes
#include "Grenade.h"

// Properties
Shooter Grenade::GetOwner() { return owner; }
void Grenade::SetOwner(Shooter* o) { owner = *o; }

Position Grenade::GetLocation() { return location; }
void Grenade::SetLocation(Position* l) { location = *l; }

// Constructors
Grenade::Grenade() {}
Grenade::Grenade(Shooter* o, Position* l)
{
	double alpha, teta = (360.0 / SHARDS_IN_GRENADE) * PI / 180;

	SetOwner(o);
	SetLocation(l);
	for (int i = 0, alpha = 0; i < SHARDS_IN_GRENADE; i++, alpha += teta)
		shards[i] = new Bullet(o, l, alpha);
}
Grenade::~Grenade() {}

// Methods

/// <summary>
/// Indicates whether the granade or its shards are drawn by their positions.
/// </summary>
/// <param name="map">The map to draw the grenade upon</param>
/// <returns>True if the grenade or its shards are drawn, False otherwise</returns>
bool Grenade::Explode(int map[MAP_DIMENSION][MAP_DIMENSION])
{
	bool finished = true;

	for (int i = 0; i < SHARDS_IN_GRENADE; i++)
		if (shards[i]->Fire(map))
			finished = false;

	return !finished;
}

/// <summary>
/// Simulates a grenade behaviour.
/// </summary>
/// <param name="map">The map to simulate the grenade at</param>
/// <param name="securityMap">The security map</param>
void Grenade::SimulateExplosion(int map[MAP_DIMENSION][MAP_DIMENSION], double securityMap[MAP_DIMENSION][MAP_DIMENSION])
{
	for (int i = 0; i < SHARDS_IN_GRENADE; i++)
		shards[i]->SimulateFire(map, securityMap);
}

/// <summary>
/// Draws the grenade.
/// </summary>
void Grenade::Draw()
{
	for (int i = 0; i < SHARDS_IN_GRENADE; i++)
		shards[i]->Draw();
}