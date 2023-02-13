// Includes
#include "Grenade.h"

#include <math.h>
#include "NPC.h"
#include "glut.h"

// Properties
Position Grenade::GetLocation() { return location; }
void Grenade::SetLocation(Position l) { location = l; }

Position Grenade::GetDestination() { return destination; }
void Grenade::SetDestination(Position d) { destination = d; }

int Grenade::GetTeam() { return team; }
void Grenade::SetTeam(int t) { team = t; }

double Grenade::GetAngle() { return angle; }
void Grenade::SetAngle(double a) { angle = a; }

double Grenade::GetDirectionRow() { return directionRow; }
void Grenade::SetDirectionRow(double dR) { directionRow = dR; }

double Grenade::GetDirectionColumn() { return directionColumn; }
void Grenade::SetDirectionColumn(double dC) { directionColumn = dC; }

// Constructors
Grenade::Grenade() 
{
	SetLocation(EMPTY_POSITION);
	SetDestination(EMPTY_POSITION);
	SetTeam(-1);
	SetAngle(0);
	SetDirectionRow(0);
	SetDirectionColumn(0);
}

Grenade::Grenade(Position l, Position d, double a, int t)
{
	SetLocation(l);
	SetDestination(d);
	SetTeam(t);
	SetAngle(a);
	SetDirectionRow(sin(angle));
	SetDirectionColumn(cos(angle));
}

Grenade::~Grenade() {}

// Methods

/// <summary>
/// Moves the grenade.
/// </summary>
void Grenade::Move()
{
	// Moves the grenade by BULLET_STEP to direction (directionColumn, directionRow)
	location.SetRow(location.GetRow() + (directionRow * BULLET_STEP));
	location.SetColumn(location.GetColumn() + (directionColumn * BULLET_STEP));
}

/// <summary>
/// Draws the Grenade.
/// </summary>
void Grenade::Draw()
{
	double row = location.GetRow(), column = location.GetColumn();
	double delta = 0.5;

	glColor3d(0, 0.39, 0); // Dark Green
	glBegin(GL_POLYGON);
	glVertex2d(column - delta, row);
	glVertex2d(column, row + delta);
	glVertex2d(column + delta, row);
	glVertex2d(column, row - delta);
	glEnd();
}

/// <summary>
/// Indicates whether the grenade is still active
/// </summary>
/// <param name="map">The map the grenade is drawn on</param>
/// <param name="enemyTeam">The grenade's possible targets</param>
/// <returns>True if the grenade is still active, False otherwise</returns>
bool Grenade::IsActive(int map[MAP_DIMENSION][MAP_DIMENSION], NPC* enemyTeam)
{
	NPC* currentEnemy = enemyTeam;
	Position enemyLocation;

	// TODO: FIX
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