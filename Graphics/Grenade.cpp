// Includes
#include "Grenade.h"

// Properties
Position Grenade::GetLocation() { return location; }
void Grenade::SetLocation(Position l) { location = l; }

int Grenade::GetTeam() { return team; }
void Grenade::SetTeam(int t) { team = t; }

// Constructors
Grenade::Grenade() {}
Grenade::Grenade(Position l, int t)
{
	double alpha, teta = (360.0 / SHARDS_IN_GRENADE) * PI / 180;

	SetLocation(l);
	SetTeam(t);

	for (int i = 0, alpha = 0; i < SHARDS_IN_GRENADE; i++, alpha += teta)
		shards[i] = new Bullet(l, t, alpha);
}
Grenade::~Grenade() {}

// Methods

/// <summary>
/// Indicates whether the granade or its shards are drawn by their positions.
/// </summary>
/// <param name="map">The map to draw the grenade upon</param>
/// <returns>True if the grenade or its shards are drawn, False otherwise</returns>
bool Grenade::Move(int map[MAP_DIMENSION][MAP_DIMENSION])
{
	bool finished = true;

	for (int i = 0; i < SHARDS_IN_GRENADE; i++)
		if (shards[i]->Move(map))
			finished = false;

	return !finished;
}

/// <summary>
/// Simulates a grenade behaviour.
/// </summary>
/// <param name="map">The map to simulate the grenade at</param>
/// <param name="securityMap">The security map to simulate the bullet at</param>
void Grenade::Explode(int map[MAP_DIMENSION][MAP_DIMENSION], int securityMap[MAP_DIMENSION][MAP_DIMENSION])
{
	for (int i = 0; i < SHARDS_IN_GRENADE; i++)
		shards[i]->Fire(map, securityMap);
}

/// <summary>
/// Draws the grenade.
/// </summary>
void Grenade::Draw()
{
	for (int i = 0; i < SHARDS_IN_GRENADE; i++)
		shards[i]->Draw();
}