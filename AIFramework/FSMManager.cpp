#include "FSMManager.h"

FSMManager::FSMManager(State* startingState)
{
	pCurrentState = startingState;
}

void FSMManager::Update(float dt)
{
	if (pCurrentState != nullptr) {
		pCurrentState->Update(dt);
	}
}

void FSMManager::ChangeState(State* newState)
{
	//Check we have an existing state
	if (pCurrentState != nullptr) {
		//if the passed in state is the same as the previous state then dont change 
		if (newState == pCurrentState) return;

		pCurrentState->OnExit();
	}

	pCurrentState = newState;
	pCurrentState->OnEnter();
}
