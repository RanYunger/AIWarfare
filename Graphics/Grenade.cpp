// Includes
#include "Grenade.h"

#include <math.h>
#include "glut.h"

// Properties
Position Grenade::GetLocation() { return location; }
void Grenade::SetLocation(Position l) { location = l; }

int Grenade::GetTeam() { return team; }
void Grenade::SetTeam(int t) { team = t; }

double Grenade::GetAngle() { return angle; }
void Grenade::SetAngle(double a) { angle = a; }

double Grenade::GetDirectionRow() { return directionRow; }
void Grenade::SetDirectionRow(double dR) { directionRow = dR; }

double Grenade::GetDirectionColumn() { return directionColumn; }
void Grenade::SetDirectionColumn(double dC) { directionColumn = dC; }

bool Grenade::IsExploding() { return isExploding; }
void Grenade::SetIsExploding(bool i) { isExploding = i; }

// Constructors
Grenade::Grenade() {}
Grenade::Grenade(Position l, int t, double a)
{
	SetLocation(l);
	SetTeam(t);
	SetAngle(a);
	SetDirectionRow(sin(a));
	SetDirectionColumn(cos(a));
	SetIsExploding(false);

	InitShards();
}
Grenade::~Grenade() {}

// Methods

/// <summary>
/// Initiates the grenade shards.
/// </summary>
void Grenade::InitShards()
{
	double alpha = 0, teta = (360.0 / SHARDS_IN_GRENADE) * PI / 180;

	for (int i = 0; i < SHARDS_IN_GRENADE; i++, alpha += teta)
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
	if (isExploding)
	{
		// Checks whether any of the shards is still moving
		for (int i = 0; i < SHARDS_IN_GRENADE; i++)
			if (shards[i]->Move(map))
				return true;

		return false;
	}

	row = location.GetRow(); column = location.GetColumn();
	if (map[(int)row][(int)column] == WALL)
	{
		isExploding = true;
		Explode(map, securityMap);

		return true;
	}

	// Moves the grenade by BULLET_STEP to direction (directionColumn, directionRow)
	column += GetDirectionColumn() * BULLET_STEP;
	row += GetDirectionRow() * BULLET_STEP;
	newLocation = new Position(row, column);
	SetLocation(*newLocation);

	return true;
}

/// <summary>
/// Simulates a grenade behaviour.
/// </summary>
/// <param name="map">The map to simulate the grenade at</param>
/// <param name="securityMap">The security map to simulate the bullet at</param>
void Grenade::Explode(int map[MAP_DIMENSION][MAP_DIMENSION], double securityMap[MAP_DIMENSION][MAP_DIMENSION])
{
	for (int i = 0; i < SHARDS_IN_GRENADE; i++)
		shards[i]->Fire(map, securityMap);
}

/// <summary>
/// Draws the grenade.
/// </summary>
void Grenade::Draw()
{
	int row = (int)location.GetRow(), column = (int)location.GetColumn();
	double delta = 0.5;

	if (isExploding)
		for (int i = 0; i < SHARDS_IN_GRENADE; i++)
			shards[i]->Draw();
	else
	{
		glColor3d(0, 0.39, 0);	// Dark Green
		glBegin(GL_POLYGON);
		glVertex2d(column - delta, row);
		glVertex2d(column, row + delta);
		glVertex2d(column + delta, row);
		glVertex2d(column, row - delta);
		glEnd();
	}
}