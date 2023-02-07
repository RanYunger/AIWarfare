// Includes
#include "Courier.h"

#include "CollectSuppliesState.h"
#include "Settings.h"

// Properties
bool Courier::IsSearchingShooter() { return isSearchingShooter; }
void Courier::SetIsSearchingShooter(bool i) { isSearchingShooter = i; }

bool Courier::IsCollectingSuuplies() { return isCollectingSupplies; }
void Courier::SetIsCollectingSupplies(bool i) { isCollectingSupplies = i; }

// Constructors & Destructors
Courier::Courier()
	: NPC() {}
Courier::Courier(Position l, int t, int r)
	: NPC(l, t, r)
{
	SetMeds(0);
	SetArms(0);

	SetState((State*)new CollectSuppliesState());
	GetState()->OnEnter(this);
}
Courier::~Courier() {}

// Methods