// Includes
#include "Bullet.h"

#include <math.h>

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