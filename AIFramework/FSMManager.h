#pragma once
#include "State.h"

class FSMManager
{
public:
	FSMManager() = default;
	FSMManager(State* startingState);

	virtual void Update(float dt);
	void ChangeState(State* newState);

protected:
	State* pCurrentState = nullptr;

};

