// Includes
#include "Bullet.h"
#include <math.h>
#include "glut.h"

// Properties
Position Bullet::GetPosition() { return position; }
void Bullet::SetPosition(Position* p) { position = *p; }

double Bullet::GetAngle() { return angle; }
void Bullet::SetAngle(double a) { angle = a; }

double Bullet::GetDirectionRow() { return directionRow; }
void Bullet::SetDirectionRow(double dR) { directionRow = dR; }

double Bullet::GetDirectionColumn() { return directionColumn; }
void Bullet::SetDirectionColumn(double dC) { directionColumn = dC; }

// Constructors & Destructors
Bullet::Bullet() {}

Bullet::Bullet(Position* p, double angle)
{
	SetPosition(p);
	SetAngle(angle);
	SetDirectionRow(sin(angle));
	SetDirectionColumn(cos(angle));
}

/// <summary>
/// Indicates whether the bullet is drawn by its position.
/// </summary>
/// <param name="map">The map to draw the bullet upon</param>
/// <returns>True if the bullet was drawn, False otherwise</returns>
bool Bullet::Fire(int map[MAP_DIMENSION][MAP_DIMENSION])
{
	int row = (int)position.GetRow(), column = (int)position.GetColumn();

	// Validation
	if (map[row][column] == WALL)
		return false;

	// Moves the bullet by BULLET_STEP to direction (directionColumn, directionRow)
	column += GetDirectionColumn() * BULLET_STEP;
	row += GetDirectionRow() * BULLET_STEP;
	SetPosition(new Position(row, column));

	return true;
}

/// <summary>
/// Simulates a bullet behaviour.
/// </summary>
/// <param name="map">The map to simulate the bullet at</param>
/// <param name="securityMap">The security map</param>
void Bullet::SimulateFire(int map[MAP_DIMENSION][MAP_DIMENSION], double securityMap[MAP_DIMENSION][MAP_DIMENSION])
{
	int row = (int)position.GetRow(), column = (int)position.GetColumn();

	while (map[row][column] != WALL)
	{
		// Moves the bullet by BULLET_STEP to direction (directionColumn, directionRow)
		securityMap[row][column] += BULLET_SIZE;
		column += GetDirectionColumn() * BULLET_STEP;
		row += GetDirectionRow() * BULLET_STEP;
		SetPosition(new Position(row, column));
	}
}

/// <summary>
/// Draws the bullet.
/// </summary>
void Bullet::Draw()
{
	int row = position.GetRow(), column = position.GetColumn();
	double delta = 0.5;

	glColor3d(0, 0, 0); // black
	glBegin(GL_POLYGON);
	glVertex2d(column - delta, row);
	glVertex2d(column, row + delta);
	glVertex2d(column + delta, row);
	glVertex2d(column, row - delta);
	glEnd();
}