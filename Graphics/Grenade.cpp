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
	location.SetRow(location.GetRow() + (directionRow * WEAPON_STEP));
	location.SetColumn(location.GetColumn() + (directionColumn * WEAPON_STEP));
}

/// <summary>
/// Draws the Grenade.
/// </summary>
void Grenade::Draw()
{
	double row = location.GetRow(), column = location.GetColumn();

	glColor3d(0, 0.39, 0); // Dark Green
	glBegin(GL_POLYGON);
	glVertex2d(column - WEAPON_SIZE, row);
	glVertex2d(column, row + WEAPON_SIZE);
	glVertex2d(column + WEAPON_SIZE, row);
	glVertex2d(column, row - WEAPON_SIZE);
	glEnd();
}