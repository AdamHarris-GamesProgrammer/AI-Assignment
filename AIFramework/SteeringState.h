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
		_pOwner->ResetVehicle();
		Clear();
	}


	void OnExit() override
	{
		Clear();
	}


	void Update(float dt) override
	{

	}

	void DrawUI() {
		ImGui::Text("Steering Options");
		if (ImGui::RadioButton("Seek", _isSeeking)) {
			Seek();
		}
		else if (ImGui::RadioButton("Flee", _isFleeing)) {
			Flee();
		}
		else if (ImGui::RadioButton("Arrive", _isArriving)) {
			Arrive();
		}
		else if (ImGui::RadioButton("Pursuit", _isPursuing)) {
			Pursuit();
		}
		else if (ImGui::RadioButton("Obstacle Avoidance", _isAvoiding)) {
			ObstacleAvoidance();
		}
		else if (ImGui::RadioButton("Wandering", _isWandering)) {
			Wandering();
		}
	}

	void Clear() {
		_pOwner->GetSteering()->ClearFlags();
	}

private:
	void Seek() {
		DisableBehaviors();
		_isSeeking = true;
		_pOwner->GetSteering()->ClearFlags();
		_pOwner->GetSteering()->SeekOn();
	}

	void Arrive() {
		DisableBehaviors();
		_isArriving = true;
		_pOwner->GetSteering()->ClearFlags();
		_pOwner->GetSteering()->ArriveOn();
	}

	void Flee() {
		DisableBehaviors();
		_isFleeing = true;
		_pOwner->GetSteering()->ClearFlags();
		_pOwner->GetSteering()->FleeOn();
	}

	void Pursuit() {
		DisableBehaviors();
		_isPursuing = true;
		_pOwner->SetMaxSpeed(120.0f);
		_pOwner->GetSteering()->ClearFlags();
		_pOwner->GetSteering()->PursuitOn();
		_pOwner->GetOtherVehicle()->SetActive();
	}

	void ObstacleAvoidance() {
		DisableBehaviors();
		_isAvoiding = true;
		_pOwner->GetSteering()->ClearFlags();
		_pOwner->GetSteering()->ArriveOn();
		_pOwner->GetSteering()->ObstacleAvoidanceOn();
		_pOwner->GetOtherVehicle()->SetActive();
	}

	void Wandering() {
		DisableBehaviors();
		_isWandering = true;
		_pOwner->GetSteering()->ClearFlags();
		_pOwner->GetSteering()->WanderOn();
	}

	void DisableBehaviors() {
		_pOwner->GetOtherVehicle()->SetActive(false);
		_pOwner->SetMaxSpeed(150.0f);
		_pOwner->GetOtherVehicle()->Reset();
		_isSeeking = false;
		_isFleeing = false;
		_isArriving = false;
		_isPursuing = false;
		_isAvoiding = false;
		_isWandering = false;
	}

private:
	//Steering Settings
	bool _isSeeking = false;
	bool _isFleeing = false;
	bool _isArriving = false;
	bool _isPursuing = false;
	bool _isAvoiding = false;
	bool _isWandering = false;
};
