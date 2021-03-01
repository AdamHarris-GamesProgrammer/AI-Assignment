#pragma once
#include "Vehicle.h"

class State {
public:
	State(Vehicle* owner) {
		pOwner = owner;
	}

	virtual void OnEnter() {}
	virtual void OnExit() {}
	virtual void Update(float dt) {}

private:
	Vehicle* pOwner = nullptr;

};
