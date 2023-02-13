// Includes
#include "Grenade.h"

#include <math.h>

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

Grenade::Grenade(Position l, Position d, int t)
{
	double angle = atan2(location.GetRow() - destination.GetRow(), location.GetColumn() - destination.GetColumn()) * (180 / PI);

	SetLocation(l);
	SetDestination(d);
	SetTeam(t);
	SetAngle(angle);
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