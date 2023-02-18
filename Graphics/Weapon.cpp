// Includes
#include "Weapon.h"

#include <math.h>
#include "NPC.h"
#include "glut.h"

// Properties
Position Weapon::GetLocation() { return location; }
void Weapon::SetLocation(Position l) { location = l; }

Position Weapon::GetPreviousLocation() { return previousLocation; }
void Weapon::SetPreviousLocation(Position p) { previousLocation = p; }

double Weapon::GetAngle() { return angle; }
void Weapon::SetAngle(double a) { angle = a; }

int Weapon::GetTeam() { return team; }
void Weapon::SetTeam(int t) { team = t; }

int Weapon::GetDamage() { return damage; }
void Weapon::SetDamage(int d) { damage = d; }

double Weapon::GetDirectionRow() { return directionRow; }
void Weapon::SetDirectionRow(double dR) { directionRow = dR; }

double Weapon::GetDirectionColumn() { return directionColumn; }
void Weapon::SetDirectionColumn(double dC) { directionColumn = dC; }

bool Weapon::IsCombustable() { return isCombustable; }
void Weapon::SetIsCombustable(bool i) { isCombustable = i; }

// Constructors & Destructors
Weapon::Weapon()
{
	SetLocation(EMPTY_POSITION);
	SetPreviousLocation(EMPTY_POSITION);
	SetAngle(0);
	SetTeam(-1);
	SetIsCombustable(false);
	SetDamage(0);
	SetDirectionRow(0);
	SetDirectionColumn(0);
}

Weapon::Weapon(Position l, double a, int t, bool i)
{
	SetLocation(l);
	SetPreviousLocation(l);
	SetAngle(a);
	SetTeam(t);
	SetIsCombustable(i);

	SetDamage(i ? GRENADE_DAMAGE : BULLET_DAMAGE);
	SetDirectionRow(sin(a));
	SetDirectionColumn(cos(a));
}

Weapon::~Weapon() {}

/// <summary>
/// Moves the bullet.
/// </summary>
void Weapon::Move()
{
	int previousRow = (int)previousLocation.GetRow(), previousColumn = (int)previousLocation.GetColumn();
	int currentRow = (int)(location.GetRow() + (GetDirectionRow() * WEAPON_STEP));
	int currentColumn = (int)(location.GetColumn() + (GetDirectionColumn() * WEAPON_STEP));

	if ((currentRow != previousRow) || (currentColumn != previousColumn))
	{
		previousLocation = location;
		damage = damage - BULLET_STEP_DAMAGE_REDUCTION <= 0 ? 0 : damage - BULLET_STEP_DAMAGE_REDUCTION;
	}

	location.SetRow(currentRow);
	location.SetColumn(currentColumn);
}

/// <summary>
/// Draws the bullet.
/// </summary>
void Weapon::Draw()
{
	double row = location.GetRow(), column = location.GetColumn();

	if (isCombustable)
		glColor3d(1, 0.84, 0);		 // Gold
	else
		glColor3d(0.75, 0.75, 0.75); // Silver

	glBegin(GL_POLYGON);
	glVertex2d(column - WEAPON_SIZE, row);
	glVertex2d(column, row + WEAPON_SIZE);
	glVertex2d(column + WEAPON_SIZE, row);
	glVertex2d(column, row - WEAPON_SIZE);
	glEnd();
}