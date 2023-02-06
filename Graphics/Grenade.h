#pragma once
#include "Bullet.h"

const int NUM_BULLETS = 20;
class Grenade
{
private:
	double x, y;
	Bullet* bullets[NUM_BULLETS];
public:
	Grenade(double xx, double yy);
	void draw();
	bool explode(int maze[MAP_DIMENSION][MAP_DIMENSION]);
	void SimulateExplosion(int maze[MAP_DIMENSION][MAP_DIMENSION], double security_map[MAP_DIMENSION][MAP_DIMENSION]);
};

