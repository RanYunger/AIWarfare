// Includes
#include "Room.h"

#include <math.h>

// Properties
Position Room::GetCenterPosition() { return centerPosition; }
void Room::SetCenterPosition(Position* p) { centerPosition = *p; }

int Room::GetID() { return id; }
void Room::SetID(int i) { id = i; }

int Room::GetHeight() { return height; }
void Room::SetHeight(int h) { height = h; }

int Room::GetWidth() { return width; }
void Room::SetWidth(int w) { width = w; }

// Constructors & Destructors
Room::Room() {}
Room::Room(Position* p, int i, int h, int w)
{
	SetCenterPosition(p);
	SetID(i);
	SetHeight(h);
	SetWidth(w);
}
Room::~Room() {}

// Methods

/// <summary>
/// Indicates whether an NPC is within the Room.
/// </summary>
/// <param name="npc">The NPC to check</param>
/// <returns>True if the NPC is within the Room, False otherwise</returns>
bool Room::IsNPCInside(NPC* npc)
{
	Position position = npc->GetPosition();

	return (fabs(position.GetRow() - centerPosition.GetRow()) <= height / 2) && (fabs(position.GetColumn() - centerPosition.GetColumn()) <= width / 2);
}