// Includes
#include "Bullet.h"

#include <math.h>
#include "NPC.h"
#include "glut.h"

// Properties
Position Bullet::GetLocation() { return location; }
void Bullet::SetLocation(Position l) { location = l; }

double Bullet::GetAngle() { return angle; }
void Bullet::SetAngle(double a) { angle = a; }

int Bullet::GetTeam() { return team; }
void Bullet::SetTeam(int t) { team = t; }

int Bullet::GetDamage() { return damage; }
void Bullet::SetDamage(int d) { damage = d; }

double Bullet::GetDirectionRow() { return directionRow; }
void Bullet::SetDirectionRow(double dR) { directionRow = dR; }

double Bullet::GetDirectionColumn() { return directionColumn; }
void Bullet::SetDirectionColumn(double dC) { directionColumn = dC; }

// Constructors & Destructors
Bullet::Bullet()
{
	SetLocation(EMPTY_POSITION);
	SetAngle(0);
	SetTeam(-1);
	SetDamage(0);
	SetDirectionRow(0);
	SetDirectionColumn(0);
}

Bullet::Bullet(Position l, double a, int t)
{
	SetLocation(l);
	SetAngle(a);
	SetTeam(t);
	SetDamage(HEALTH_BOOST);
	SetDirectionRow(sin(a));
	SetDirectionColumn(cos(a));
}

Bullet::~Bullet() {}

/// <summary>
/// Moves the bullet.
/// </summary>
void Bullet::Move()
{
	// Moves the bullet by BULLET_STEP to direction (directionColumn, directionRow) on the map
	location.SetRow(location.GetRow() + GetDirectionRow() * BULLET_STEP);
	location.SetColumn(location.GetColumn() + GetDirectionColumn() * BULLET_STEP);

	damage = damage - 1 <= 0 ? 0 : damage - 1;
}

/// <summary>
/// Draws the bullet.
/// </summary>
void Bullet::Draw()
{
	double row = location.GetRow(), column = location.GetColumn();
	double delta = 0.5;

	glColor3d(0.75, 0.75, 0.75); // Silver
	glBegin(GL_POLYGON);
	glVertex2d(column - delta, row);
	glVertex2d(column, row + delta);
	glVertex2d(column + delta, row);
	glVertex2d(column, row - delta);
	glEnd();
}

/// <summary>
/// Indicates whether the bullet is still active
/// </summary>
/// <param name="map">The map the bullet is drawn on</param>
/// <param name="enemyTeam">The bullet's possible targets</param>
/// <returns>True if the bullet is still active, False otherwise</returns>
bool Bullet::IsActive(int map[MAP_DIMENSION][MAP_DIMENSION], NPC* enemyTeam)
{
	NPC* currentEnemy = enemyTeam;
	Position enemyLocation;

	// Checks if the grenade hits a wall
	if (map[(int)(location.GetRow() - BULLET_SIZE)][(int)location.GetColumn()] == WALL)
		return false;
	if (map[(int)(location.GetRow() + BULLET_SIZE)][(int)location.GetColumn()] == WALL)
		return false;
	if (map[(int)location.GetRow()][(int)(location.GetColumn() - BULLET_SIZE)] == WALL)
		return false;
	if (map[(int)location.GetRow()][(int)(location.GetColumn() + BULLET_SIZE)] == WALL)
		return false;

	// Checks if the grenade hits an enemy
	for (int i = 0; i < TEAM_SIZE; i++, currentEnemy++)
	{
		enemyLocation = currentEnemy->GetLocation();

		if (fabs(location.GetRow() - enemyLocation.GetRow()) <= BULLET_SIZE)
			return false;
		if (fabs(location.GetColumn() - enemyLocation.GetColumn()) <= BULLET_SIZE)
			return false;
	}

	return true;
}