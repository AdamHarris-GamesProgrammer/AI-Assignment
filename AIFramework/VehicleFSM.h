#pragma once
#ifndef VEHICLEFSM_H
#define VEHICLEFSM_H

#include "FSMManager.h"
#include "Vehicle.h"
#include "PathfindingState.h"
#include "SteeringState.h"
#include "DecisionMakingState.h"

//A extension of the base state machine, this is to create a state machine specific to this assignment rather than clog up a reusable state machine with project specific data
class VehicleFSM : public FSMManager
{
private:


public:
	//Enum created to help control which section of the assignment I am showcasing
	enum Sections {
		steering,
		pathfinding,
		decisionMaking
	};

	VehicleFSM(Vehicle* owner) {
		//initializes the owner
		_pOwner = owner;

		//Initializes all our states
		pPathfindingState = new PathfindingState(_pOwner);

		pSteeringState = new SteeringState(_pOwner);

		pDecisionMakingState = new DecisionMakingState(_pOwner);

		//Changes to the pathfinding state which I chose for our default state
		ChangeState(pPathfindingState);

		_section = pathfinding;
	}

	void DrawUI() {
		//Draws the UI for controlling what state the race car is in
		ImGui::Begin("AI Control Panel");
		if (ImGui::RadioButton("Steering", _isSteering)) {
			_isSteering = true;
			_isPathfinding = false;
			_isDecisionMaking = false;

			Section1AI();
		}
		else if (ImGui::RadioButton("Pathfinding", _isPathfinding)) {
			_isPathfinding = true;
			_isSteering = false;
			_isDecisionMaking = false;

			Section2AI();
		}
		else if (ImGui::RadioButton("Decision Making", _isDecisionMaking)) {
			_isDecisionMaking = true;
			_isSteering = false;
			_isPathfinding = false;

			Section3AI();
		}
		//Draws UI for the current state
		pCurrentState->DrawUI();

		ImGui::End();
	}

	//Sets us on the SectionA steering behaviors AI
	void Section1AI() {
		_section = steering;
		DisableBehaviors();
		ChangeState(pSteeringState);
	}

	//Sets us on the SectionB steering behaviors AI
	void Section2AI() {
		_section = pathfinding;
		DisableBehaviors();
		ChangeState(pPathfindingState);
	}

	//Sets us on the SectionC steering behaviors AI
	void Section3AI() {
		_section = decisionMaking;
		DisableBehaviors();
		ChangeState(pDecisionMakingState);
	}

	void SetWaypointTolerance(float val) {
		pDecisionMakingState->SetWaypointTolerance(val);
	}

	void Pickup() {
		pDecisionMakingState->PickupSpawned();
	}

	Sections GetSection() const {
		return _section;
	}

private:
	void DisableBehaviors()
	{
		_pOwner->GetOtherVehicle()->SetActive(false);
		_pOwner->GetOtherVehicle()->Reset();
	}

private:
	bool _isSteering = false;
	bool _isPathfinding = true;
	bool _isDecisionMaking = false;

	Vehicle* _pOwner;

	PathfindingState* pPathfindingState;
	SteeringState* pSteeringState;
	DecisionMakingState* pDecisionMakingState;

	Sections _section;
};
#endif

