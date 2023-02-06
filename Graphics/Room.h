#pragma once

// Includes
#include "NPC.h"

class Room
{
private:
	// Fields
	Position centerPosition;
	int id, height, width;

public:
	// Properties
	Position GetCenterPosition();
	void SetCenterPosition(Position* p);

	int GetID();
	void SetID(int i);

	int GetHeight();
	void SetHeight(int h);

	int GetWidth();
	void SetWidth(int w);

	// Constructors & Destructors
	Room();
	Room(Position* p, int i, int h, int w);
	~Room();

	// Methods
	bool operator == (const Room& other) { return (centerPosition == other.centerPosition) && (height == other.height) && (width == other.width); }

	bool IsNPCInside(NPC* npc);
};