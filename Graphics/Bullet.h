#pragma once

// Includes
#include "Attacker.h"
#include "Position.h"
#include "Settings.h"

class Bullet
{
private:
	// Fields
	Attacker owner;
	Position location;
	double angle, directionRow, directionColumn;
public:
	// Properties
	Attacker GetOwner();
	void SetOwner(Attacker* o);

	Position GetLocation();
	void SetLocation(Position* l);

	double GetAngle();
	void SetAngle(double a);

	double GetDirectionRow();
	void SetDirectionRow(double dR);

	double GetDirectionColumn();
	void SetDirectionColumn(double dC);

	// Constructors & Destructors
	Bullet();
	Bullet(Attacker* o, Position* l, double angle);
	~Bullet();

	// Methods
	bool Fire(int map[MAP_DIMENSION][MAP_DIMENSION]);
	void SimulateFire(int map[MAP_DIMENSION][MAP_DIMENSION], double securityMap[MAP_DIMENSION][MAP_DIMENSION]);
	void Draw();
};

