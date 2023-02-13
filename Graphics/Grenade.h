#pragma once

// Includes
#include "Bullet.h"
#include "Position.h"
#include "Settings.h"

class Grenade
{
private:
	// Fields
	Position location, destination;
	int team;
	double angle, directionRow, directionColumn;

public:
	// Properties
	Position GetLocation();
	void SetLocation(Position l);

	Position GetDestination();
	void SetDestination(Position d);

	int GetTeam();
	void SetTeam(int t);

	double GetAngle();
	void SetAngle(double a);

	double GetDirectionRow();
	void SetDirectionRow(double dR);

	double GetDirectionColumn();
	void SetDirectionColumn(double dC);

	// Consturctors & Destructors
	Grenade();
	Grenade(Position l, Position d, double a, int t);
	~Grenade();

	// Methods
	void Move();
	void Draw();

	bool IsActive(int map[MAP_DIMENSION][MAP_DIMENSION], NPC* enemyTeam);
};