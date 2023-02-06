#pragma once
#include "NPC.h"
//#include "Shooter.h"
//#include "Courier.h"

class NPC;
//class Shooter;
//class Courier;

class State
{
public:
	virtual void Transform(NPC* pn) = 0;	// to the next state
	virtual void OnEnter(NPC* pn) = 0;		// to the current state
	virtual void OnExit(NPC* pn) = 0;		// from the current state
};