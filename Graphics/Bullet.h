#pragma once

// Includes
#include "Position.h"
#include "Settings.h"

class Bullet
{
private:
	// Fields
	Position location;
	int team;
	double angle, directionRow, directionColumn;

public:
	// Properties
	Position GetLocation();
	void SetLocation(Position l);

	int GetTeam();
	void SetTeam(int t);

	double GetAngle();
	void SetAngle(double a);

	double GetDirectionRow();
	void SetDirectionRow(double dR);

	double GetDirectionColumn();
	void SetDirectionColumn(double dC);

	// Constructors & Destructors
	Bullet();
	Bullet(Position l, int t, double a);
	~Bullet();

	// Methods
	bool Move(int map[MAP_DIMENSION][MAP_DIMENSION]);
	void Fire(int map[MAP_DIMENSION][MAP_DIMENSION], double securityMap[MAP_DIMENSION][MAP_DIMENSION]);
	void Draw();
};

