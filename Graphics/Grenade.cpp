// Includes
#include "Grenade.h"

#include <math.h>
#include "glut.h"

// Properties
Position Grenade::GetLocation() { return location; }
void Grenade::SetLocation(Position l) { location = l; }

Position Grenade::GetDestination() { return destination; }
void Grenade::SetDestination(Position d) { destination = d; }

Bullet** Grenade::GetShards() { return shards; }

int Grenade::GetTeam() { return team; }
void Grenade::SetTeam(int t) { team = t; }

double Grenade::GetAngle() { return angle; }
void Grenade::SetAngle(double a) { angle = a; }

double Grenade::GetDirectionRow() { return directionRow; }
void Grenade::SetDirectionRow(double dR) { directionRow = dR; }

double Grenade::GetDirectionColumn() { return directionColumn; }
void Grenade::SetDirectionColumn(double dC) { directionColumn = dC; }

// Constructors
Grenade::Grenade() {}
Grenade::Grenade(Position l, Position d, int t)
{
	double dRow = destination.GetRow() - location.GetRow(), dColumn = destination.GetColumn() - location.GetColumn();
	double angle = atan(dRow / dColumn);

	SetLocation(l);
	SetDestination(d);
	SetTeam(t);
	SetAngle(angle);
	SetDirectionRow(sin(angle));
	SetDirectionColumn(cos(angle));

	InitShards();
}
Grenade::~Grenade() {}

// Methods

/// <summary>
/// Initiates the grenade shards.
/// </summary>
void Grenade::InitShards()
{
	double alpha = 0, teta = (360.0 / MAX_SHARDS_IN_GRENADE) * PI / 180;

	for (int i = 0; i < MAX_SHARDS_IN_GRENADE; i++, alpha += teta)
		shards[i] = new Bullet(location, team, alpha);
}

/// <summary>
/// Indicates whether the granade can move.
/// </summary>
/// <param name="map">The map to draw the grenade upon</param>
/// <param name="securityMap">The security map to simulate the grenade at</param>
/// <returns>True if the grenade can move, False otherwise</returns>
bool Grenade::Move(int map[MAP_DIMENSION][MAP_DIMENSION], double securityMap[MAP_DIMENSION][MAP_DIMENSION])
{
	double row, column;
	Position* newLocation;

	// Validation
	if (location == destination)
		return false;

	// Moves the grenade by BULLET_STEP to direction (directionColumn, directionRow)
	row = location.GetRow() + (directionRow * BULLET_STEP);
	column = location.GetColumn() + (directionColumn * BULLET_STEP);
	newLocation = new Position(row, column);
	SetLocation(*newLocation);

	for (int i = 0; i < MAX_SHARDS_IN_GRENADE; i++)
		shards[i]->SetLocation(*newLocation);

	// Updates the security factor at (row, column)
	securityMap[(int)row][(int)column] += BULLET_SIZE * MAX_SHARDS_IN_GRENADE;

	return true;
}

/// <summary>
/// Draws the grenade.
/// </summary>
void Grenade::Draw()
{
	int row = (int)location.GetRow(), column = (int)location.GetColumn();
	double delta = 0.5;

	glColor3d(0, 0.39, 0);	// Dark Green
	glBegin(GL_POLYGON);
	glVertex2d(column - delta, row);
	glVertex2d(column, row + delta);
	glVertex2d(column + delta, row);
	glVertex2d(column, row - delta);
	glEnd();
}