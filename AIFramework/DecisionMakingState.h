#pragma once
#include "State.h"
#include "Vector2D.h"
#include "Track.h"
#include "Node.h"
#include "Waypoint.h"
#include "Steering.h"

class Vehicle;

class DecisionMakingState : public State {
public:
	DecisionMakingState(Vehicle* owner) : State(owner) {

	}



	void OnEnter() override
	{

	}


	void OnExit() override
	{

	}


	void Update(float dt) override
	{

	}

};