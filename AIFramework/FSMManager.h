#pragma once
#include "State.h"

//A basic state machine implementation
class FSMManager
{
public:
	FSMManager() = default;
	//Sets a starting state if we have one
	FSMManager(State* startingState);

	//Updates the state machine
	virtual void Update(float dt);
	//Changes state
	void ChangeState(State* newState);

protected:
	//Holds our current state
	State* pCurrentState = nullptr;

};

