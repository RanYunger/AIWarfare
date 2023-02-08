// Includes
#include "Bullet.h"
#include <math.h>
#include "glut.h"

// Properties
Position Bullet::GetLocation() { return location; }
void Bullet::SetLocation(Position l) { location = l; }

int Bullet::GetTeam() { return team; }
void Bullet::SetTeam(int t) { team = t; }

double Bullet::GetAngle() { return angle; }
void Bullet::SetAngle(double a) { angle = a; }

double Bullet::GetDirectionRow() { return directionRow; }
void Bullet::SetDirectionRow(double dR) { directionRow = dR; }

double Bullet::GetDirectionColumn() { return directionColumn; }
void Bullet::SetDirectionColumn(double dC) { directionColumn = dC; }

// Constructors & Destructors
Bullet::Bullet() {}

Bullet::Bullet(Position l, int t, double a)
{
	SetLocation(l);
	SetTeam(t);
	SetAngle(a);
	SetDirectionRow(sin(a));
	SetDirectionColumn(cos(a));
}

/// <summary>
/// Indicates whether the bullet is drawn by its position.
/// </summary>
/// <param name="map">The map to draw the bullet upon</param>
/// <returns>True if the bullet was drawn, False otherwise</returns>
bool Bullet::Move(int map[MAP_DIMENSION][MAP_DIMENSION])
{
	double row = location.GetRow(), column = location.GetColumn();
	Position* newLocation;

	// Validation
	if (map[(int)row][(int)column] == WALL)
		return false;

	// Moves the bullet by BULLET_STEP to direction (directionColumn, directionRow)
	column += GetDirectionColumn() * BULLET_STEP;
	row += GetDirectionRow() * BULLET_STEP;
	newLocation = new Position(row, column);
	SetLocation(*newLocation);

	return true;
}

/// <summary>
/// Simulates a bullet behaviour.
/// </summary>
/// <param name="map">The map to simulate the bullet at</param>
/// <param name="securityMap">The security map to simulate the bullet at</param>
void Bullet::Fire(int map[MAP_DIMENSION][MAP_DIMENSION], int securityMap[MAP_DIMENSION][MAP_DIMENSION])
{
	int row = (int)location.GetRow(), column = (int)location.GetColumn();

	while (map[row][column] != WALL)
	{
		securityMap[row][column] += BULLET_SIZE;
		Move(map);
	}
}

/// <summary>
/// Draws the bullet.
/// </summary>
void Bullet::Draw()
{
	int row = (int)location.GetRow(), column = (int)location.GetColumn();
	double delta = 0.5;

	glColor3d(0, 0, 0); // black
	glBegin(GL_POLYGON);
	glVertex2d(column - delta, row);
	glVertex2d(column, row + delta);
	glVertex2d(column + delta, row);
	glVertex2d(column, row - delta);
	glEnd();
}