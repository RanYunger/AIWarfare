#pragma once

// Includes
#include "Position.h"
#include "Settings.h"

class Bullet
{
private:
	// Fields
	Position location;
	int team, damage;
	double angle, directionRow, directionColumn;

public:
	// Properties
	Position GetLocation();
	void SetLocation(Position l);

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

	// Constructors & Destructors
	Bullet();
	Bullet(Position l, double a, int t);
	~Bullet();

	// Methods
	bool Move(int map[MAP_DIMENSION][MAP_DIMENSION], double securityMap[MAP_DIMENSION][MAP_DIMENSION]);
	void Draw();
};