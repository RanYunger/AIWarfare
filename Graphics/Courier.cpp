// Includes
#include "Courier.h"

#include "SearchSupplyState.h"
#include "Settings.h"

// Properties
bool Courier::IsSearchingSupply() { return isSearchingSupply; }
void Courier::SetIsSearchingSupply(bool i) { isSearchingSupply = i; }

bool Courier::IsSearchingAlly() { return isSearchingAlly; }
void Courier::SetIsSearchingAlly(bool i) { isSearchingAlly = i; }

bool Courier::IsSharingSupply() { return isSharingSupply; }
void Courier::SetIsSharingSupply(bool i) { isSharingSupply = i; }

// Constructors & Destructors
Courier::Courier()
	: NPC()
{
	SetIsSharingSupply(false);
	SetIsSearchingAlly(false);
	SetIsSharingSupply(false);

	SetActiveState((State*)new SearchSupplyState());
}
Courier::~Courier() {}

// Methods
/// <summary>
/// Gives arms to a teammate.
/// </summary>
/// <param name="teammate">The teammate who takes the arms</param>
void Courier::GiveArms(Attacker* teammate)
{
	int teammateArms = teammate->GetArms();

	teammate->SetArms(teammateArms + 1 >= MAX_ARMS ? MAX_ARMS : teammateArms + 1);
	arms = arms - 1 <= 0 ? 0 : arms - 1;
}

/// <summary>
/// Take arms from a teammate.
/// </summary>
/// <param name="teammate">The teammate to take arms from</param>
void Courier::TakeArms(Attacker* teammate)
{
	int teammateArms = teammate->GetArms();

	teammate->SetArms(teammateArms - 1 <= 0 ? 0 : teammateArms - 1);
	arms = arms + 1 >= MAX_ARMS ? MAX_ARMS : arms + 1;
}

/// <summary>
/// Gives meds to a teammate.
/// </summary>
/// <param name="teammate">The teammate who takes the meds</param>
void Courier::GiveMeds(Attacker* teammate)
{
	int teammateMeds = teammate->GetMeds();

	teammate->SetMeds(teammateMeds + 1 >= MAX_MEDS ? MAX_MEDS : teammateMeds + 1);
	meds = meds - 1 <= 0 ? 0 : meds - 1;
}

/// <summary>
/// Take meds from a teammate.
/// </summary>
/// <param name="teammate">The teammate to take meds from</param>
void Courier::TakeMeds(Attacker* teammate)
{
	int teammateMeds = teammate->GetMeds();

	teammate->SetMeds(teammateMeds - 1 <= 0 ? 0 : teammateMeds - 1);
	meds = meds + 1 >= MAX_MEDS ? MAX_MEDS : meds + 1;
}

/// <summary>
/// Picks a supply.
/// </summary>
/// <param name="supply">The supply type</param>
void Courier::PickSupply(int supply)
{
	if (supply == ARMS)
		arms++;
	else if (supply == MEDS)
		meds++;
}