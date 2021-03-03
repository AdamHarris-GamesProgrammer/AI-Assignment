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
		//TODO: Implement flee behavior
	}

	void Pursuit() {
		pOwner->GetSteering()->ClearFlags();
		//TODO: Implement Pursuit behavior
	}

	void ObstacleAvoidance() {
		pOwner->GetSteering()->ClearFlags();
		//TODO: Implement Obstacle behavior
	}

	void Wandering() {
		pOwner->GetSteering()->ClearFlags();
		//TODO: Implement wandering behavior
	}


private:

};
