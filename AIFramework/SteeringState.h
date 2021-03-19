#pragma once

#include "Vehicle.h"
#include "Steering.h"
#include "State.h"

class SteeringState : public State {

public:
	SteeringState(Vehicle* owner) : State(owner) 
	{
		
	}

	void OnEnter() override
	{
		pOwner->GetSteering()->ClearFlags();
	}


	void OnExit() override
	{

	}


	void Update(float dt) override
	{

	}

	void Clear() {
		pOwner->GetSteering()->ClearFlags();
	}

	void Seek() {
		pOwner->GetSteering()->ClearFlags();
		pOwner->GetSteering()->SeekOn();
	}

	void Arrive() {
		pOwner->GetSteering()->ClearFlags();
		pOwner->GetSteering()->ArriveOn();
	}

	void Flee() {
		pOwner->GetSteering()->ClearFlags();
		pOwner->GetSteering()->FleeOn();
	}

	void Pursuit() {
		pOwner->GetSteering()->ClearFlags();
		pOwner->GetSteering()->PursuitOn();
	}

	void ObstacleAvoidance() {
		pOwner->GetSteering()->ClearFlags();
		pOwner->GetSteering()->SeekOn();
		pOwner->GetSteering()->ObstacleAvoidanceOn();
	}

	void Wandering() {
		pOwner->GetSteering()->ClearFlags();
		pOwner->GetSteering()->WanderOn();
	}


private:

};
