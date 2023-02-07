#pragma once

// Includes
#include "Shooter.h"
#include "Position.h"
#include "Settings.h"

class Bullet
{
private:
	// Fields
	Shooter owner;
	Position location;
	double angle, directionRow, directionColumn;
public:
	// Properties
	Shooter GetOwner();
	void SetOwner(Shooter* o);

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
	Bullet(Shooter* o, Position* l, double angle);
	~Bullet();

	// Methods
	bool Fire(int map[MAP_DIMENSION][MAP_DIMENSION]);
	void SimulateFire(int map[MAP_DIMENSION][MAP_DIMENSION], double securityMap[MAP_DIMENSION][MAP_DIMENSION]);
	void Draw();
};

