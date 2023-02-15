// Includes
#include "Bullet.h"

#include <math.h>
#include "NPC.h"
#include "glut.h"

// Properties
Position Bullet::GetLocation() { return location; }
void Bullet::SetLocation(Position l) { location = l; }

Position Bullet::GetPreviousLocation() { return previousLocation; }
void Bullet::SetPreviousLocation(Position p) { previousLocation = p; }

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
	SetPreviousLocation(EMPTY_POSITION);
	SetAngle(0);
	SetTeam(-1);
	SetDamage(0);
	SetDirectionRow(0);
	SetDirectionColumn(0);
}

Bullet::Bullet(Position l, double a, int t)
{
	SetLocation(l);
	SetPreviousLocation(l);
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
	int previousRow = (int)previousLocation.GetRow(), previousColumn = (int)previousLocation.GetColumn();
	int currentRow = location.GetRow() + (GetDirectionRow() * WEAPON_STEP);
	int currentColumn = location.GetColumn() + (GetDirectionColumn() * WEAPON_STEP);

	if ((currentRow != previousRow) || (currentColumn != previousColumn))
	{
		previousLocation = location;
		damage = damage - 1 <= 0 ? 0 : damage - 1;
	}

	location.SetRow(currentRow);
	location.SetColumn(currentColumn);
}

/// <summary>
/// Draws the bullet.
/// </summary>
void Bullet::Draw()
{
	double row = location.GetRow(), column = location.GetColumn();

	glColor3d(0.75, 0.75, 0.75); // Silver
	glBegin(GL_POLYGON);
	glVertex2d(column - WEAPON_SIZE, row);
	glVertex2d(column, row + WEAPON_SIZE);
	glVertex2d(column + WEAPON_SIZE, row);
	glVertex2d(column, row - WEAPON_SIZE);
	glEnd();
}