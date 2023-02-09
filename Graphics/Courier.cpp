// Includes
#include "Courier.h"

#include "SearchSupplyState.h"
#include "Settings.h"

// Properties
int Courier::GetSupply() { return supply; }
void Courier::SetSupply(int s) { supply = s; }

int Courier::GetTransaction() { return transaction; }
void Courier::SetTransaction(int t) { transaction = t; }

bool Courier::IsSearchingSupply() { return isSearchingSupply; }
void Courier::SetIsSearchingSupply(bool i) { isSearchingSupply = i; }

bool Courier::IsSearchingAlly() { return isSearchingAlly; }
void Courier::SetIsSearchingAlly(bool i) { isSearchingAlly = i; }

bool Courier::IsSharingSupply() { return isSharingSupply; }
void Courier::SetIsSharingSupply(bool i) { isSharingSupply = i; }

bool Courier::IsCalled() { return isCalled; }
void Courier::SetIsCalled(bool i) { isCalled = i; }

// Constructors & Destructors
Courier::Courier()
	: NPC()
{
	SetSupply(-1);
	SetTransaction(-1);
	SetIsSharingSupply(false);
	SetIsSearchingAlly(false);
	SetIsSharingSupply(false);
	SetIsCalled(false);

	SetActiveState((State*)new SearchSupplyState());
	activeState->OnEnter(this);
}
Courier::~Courier() {}

// Methods
/// <summary>
/// Gives arms to an ally.
/// </summary>
/// <param name="ally">The ally who takes the arms</param>
void Courier::GiveArms(Attacker* ally)
{
	int allyArms = ally->GetArms();

	ally->SetArms(allyArms + 1 >= MAX_ARMS ? MAX_ARMS : allyArms + 1);
	arms = arms - 1 <= 0 ? 0 : arms - 1;
}

/// <summary>
/// Take arms from an ally.
/// </summary>
/// <param name="ally">The ally to take arms from</param>
void Courier::TakeArms(Attacker* ally)
{
	int allyArms = ally->GetArms();

	ally->SetArms(allyArms - 1 <= 0 ? 0 : allyArms - 1);
	arms = arms + 1 >= MAX_ARMS ? MAX_ARMS : arms + 1;
}

/// <summary>
/// Gives meds to an ally.
/// </summary>
/// <param name="ally">The ally who takes the meds</param>
void Courier::GiveMeds(Attacker* ally)
{
	int allyMeds = ally->GetMeds();

	ally->SetMeds(allyMeds + 1 >= MAX_MEDS ? MAX_MEDS : allyMeds + 1);
	meds = meds - 1 <= 0 ? 0 : meds - 1;
}

/// <summary>
/// Take meds from an ally.
/// </summary>
/// <param name="ally">The ally to take meds from</param>
void Courier::TakeMeds(Attacker* ally)
{
	int allyMeds = ally->GetMeds();

	ally->SetMeds(allyMeds - 1 <= 0 ? 0 : allyMeds - 1);
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

/// <summary>
/// Trades a supply with an ally
/// </summary>
/// <param name="ally">The ally to trade with</param>
void Courier::TradeSupply(Attacker* ally)
{
	isCalled = false;

	if (transaction == GIVE)
	{
		if (supply == ARMS)
			GiveArms(ally);
		else
			GiveMeds(ally);
	}
	else if (transaction == TAKE)
	{
		if (supply == ARMS)
			TakeArms(ally);
		else
			TakeMeds(ally);
	}
}