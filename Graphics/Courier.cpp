// Includes
#include "Courier.h"

#include "CollectSuppliesState.h"
#include "Settings.h"

// Properties
int Courier::GetArms() { return arms; }
void Courier::SetArms(int a) { arms = a; }

int Courier::GetMeds() { return meds; }
void Courier::SetMeds(int m) { meds = m; }

bool Courier::IsSelfHealing() { return isSelfHealing; }
void Courier::SetIsSelfHealing(bool i) { isSelfHealing = i; }

bool Courier::IsSearchingShooter() { return isSearchingShooter; }
void Courier::SetIsSearchingShooter(bool i) { isSearchingShooter = i; }

bool Courier::IsCollectingSuuplies() { return isCollectingSupplies; }
void Courier::SetIsCollectingSupplies(bool i) { isCollectingSupplies = i; }

// Constructors & Destructors
Courier::Courier()
	: NPC() {}
Courier::Courier(Position p, int t, int r)
	: NPC(p, t, r)
{
	SetMeds(0);
	SetArms(0);

	SetState((State*)new CollectSuppliesState());
	GetState()->OnEnter(this);
}
Courier::~Courier() {}

// Methods

/// <summary>
/// Increases the arms counter.
/// </summary>
void Courier::TakeArms() { SetArms(GetArms() + 1); }

/// <summary>
/// Increases the meds counter.
/// </summary>
void Courier::TakeMeds() { SetMeds(GetMeds() + 1); }

/// <summary>
/// Shares arms with a teammate.
/// </summary>
void Courier::GiveArms(Shooter* teammate)
{
	int arms = GetArms();

	teammate->TakeArms();
	SetArms(arms - 1 <= 0 ? 0 : arms - 1);
}

/// <summary>
/// Shares meds with a teammate.
/// </summary>
void Courier::GiveMeds(NPC* teammate)
{
	int h = teammate->GetHealth();

	teammate->SetHealth(h + 25 >= MAX_HEALTH ? MAX_HEALTH : h + 25);
	SetMeds(meds - 1 <= 0 ? 0 : meds - 1);
}

/// <summary>
/// Increases health and decreases the meds counter
/// </summary>
void Courier::SelfHeal() { GiveMeds(this); }