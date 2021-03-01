#pragma once
#include "State.h"

class FSMManager
{
public:
	FSMManager() = default;
	FSMManager(State* startingState);

	void Update(float dt);
	void ChangeState(State* newState);

private:

	State* pCurrentState = nullptr;

};

