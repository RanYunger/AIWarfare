#include "Bullet.h"
#include <math.h>
#include "glut.h"

Bullet::Bullet()
{
}

Bullet::Bullet(double xx, double yy, double alpha)
{
	x = xx;
	y = yy;
	angle = alpha;
	dirx = cos(alpha);
	diry = sin(alpha);
}

bool Bullet::fire(int maze[MAP_DIMENSION][MAP_DIMENSION])
{
	if (maze[(int)y][(int)x] == WALL) return false;
	
	x += dirx * STEP; // move bullet by STEP to direction (dirx,dity)
	y += diry * STEP;

	return true;
}

void Bullet::draw()
{
	double delta = 0.5;
	glColor3d(0, 0, 0); // black
	glBegin(GL_POLYGON);
	glVertex2d(x - delta, y);
	glVertex2d(x , y+delta);
	glVertex2d(x + delta, y);
	glVertex2d(x, y - delta);
	glEnd();
}

void Bullet::SimulateFire(int maze[MAP_DIMENSION][MAP_DIMENSION], double security_map[MAP_DIMENSION][MAP_DIMENSION])
{
	while (maze[(int)y][(int)x]!=WALL)
	{
		security_map[(int)y][(int)x] += MARK;
		x += dirx * STEP; // move bullet by STEP to direction (dirx,dity)
		y += diry * STEP;

	}
}


