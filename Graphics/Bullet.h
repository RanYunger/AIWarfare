#pragma once
#include"settings.h"

const double STEP = 0.2;
const double MARK = 0.001;

class Bullet
{
private:
	double x, y;
	double dirx, diry;
	double angle;
public:
	Bullet();
	Bullet(double xx, double yy, double alpha);
	bool fire(int maze[MAP_DIMENSION][MAP_DIMENSION]);
	void draw();
	void SimulateFire(int maze[MAP_DIMENSION][MAP_DIMENSION], double security_map[MAP_DIMENSION][MAP_DIMENSION]);
};

