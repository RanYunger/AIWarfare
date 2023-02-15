#pragma once

// Includes
#include "NPC.h"
#include "Position.h"
#include "Settings.h"

class Weapon
{
private:
	// Fields
	Position location, previousLocation;
	int team, damage;
	double angle, directionRow, directionColumn;
	bool isCombustable;

public:
	// Properties
	Position GetLocation();
	void SetLocation(Position l);

	Position GetPreviousLocation();
	void SetPreviousLocation(Position p);

	double GetAngle();
	void SetAngle(double a);

	int GetTeam();
	void SetTeam(int t);

	int GetDamage();
	void SetDamage(int d);

	double GetDirectionRow();
	void SetDirectionRow(double dR);

	double GetDirectionColumn();
	void SetDirectionColumn(double dC);

	bool IsCombustable();
	void SetIsCombustable(bool i);

	// Constructors & Destructors
	Weapon();
	Weapon(Position l, double a, int t, bool i);
	~Weapon();

	// Methods
	void Move();
	void Draw();
};