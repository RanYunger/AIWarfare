#pragma once
// Includes
#include "Position.h"
#include "Settings.h"

class Bullet
{
private:
	// Fields
	Position position;
	double angle, directionRow, directionColumn;
public:
	// Properties
	Position GetPosition();
	void SetPosition(Position* p);

	double GetAngle();
	void SetAngle(double a);

	double GetDirectionRow();
	void SetDirectionRow(double dR);

	double GetDirectionColumn();
	void SetDirectionColumn(double dC);

	// Constructors & Destructors
	Bullet();
	Bullet(Position* p, double angle);
	~Bullet();

	// Methods
	bool Fire(int map[MAP_DIMENSION][MAP_DIMENSION]);
	void SimulateFire(int map[MAP_DIMENSION][MAP_DIMENSION], double securityMap[MAP_DIMENSION][MAP_DIMENSION]);
	void Draw();
};

