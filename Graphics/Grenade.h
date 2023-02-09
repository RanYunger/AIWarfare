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
	Bullet* shards[SHARDS_IN_GRENADE];
	int team;
	double angle, directionRow, directionColumn;
	bool isExploding;

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

	bool IsExploding();
	void SetIsExploding(bool i);

	// Consturctors & Destructors
	Grenade();
	Grenade(Position l, Position d, int t);
	~Grenade();

	// Methods
	void InitShards();
	bool Move(int map[MAP_DIMENSION][MAP_DIMENSION], double securityMap[MAP_DIMENSION][MAP_DIMENSION]);
	void Explode(int map[MAP_DIMENSION][MAP_DIMENSION], double securityMap[MAP_DIMENSION][MAP_DIMENSION]);
	void Draw();
};