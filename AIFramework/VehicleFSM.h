#pragma once
#ifndef VEHICLEFSM_H
#define VEHICLEFSM_H

#include "FSMManager.h"
#include "Vehicle.h"
#include "PathfindingState.h"
#include "SteeringState.h"




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
		//initializes pathfinding state
		_pOwner = owner;


		pPathfindingState = new PathfindingState(_pOwner);
		
		pSteeringState = new SteeringState(_pOwner);

		ChangeState(pPathfindingState);

		_section = pathfinding;
	}

	void DrawUI() {
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

			_pOwner->GetOtherVehicle()->SetActive(true);

			Section3AI();
		}
		
		switch (_section)
		{
		case steering:
			DrawSteeringOptions();
			break;
		case pathfinding:
			DrawPathfindingOptions();
			break;
		case decisionMaking:
			DrawDecisionMakingOptions();
			break;
		default:
			break;
		}

		ImGui::End();
	}

	void Section1AI() {
		_section = steering;
		pSteeringState->Clear();
		DisableBehaviors();
		ChangeState(pSteeringState);
	}

	void Section2AI() {
		_section = pathfinding; 
		pSteeringState->Clear();
		ChangeState(pPathfindingState);
	}

	void Section3AI() {
		_section = pathfinding;
		pSteeringState->Clear();
	}

	Sections GetSection() const {
		return _section;
	}

private:
		void DrawPathfindingOptions()
		{

		}

		void DrawDecisionMakingOptions()
		{

		}

		void DrawSteeringOptions()
		{
			ImGui::Text("Steering Options");
			if (ImGui::RadioButton("Seek", _isSeeking)) {
				DisableBehaviors();
				_isSeeking = true;
				pSteeringState->Seek();
			}
			else if (ImGui::RadioButton("Flee", _isFleeing)) {
				DisableBehaviors();
				_isFleeing = true;
				pSteeringState->Flee();
			}
			else if (ImGui::RadioButton("Arrive", _isArriving)) {
				DisableBehaviors();
				_isArriving = true;
				pSteeringState->Arrive();
			}
			else if (ImGui::RadioButton("Pursuit", _isPursuing)) {
				DisableBehaviors();
				_isPursuing = true;
				pSteeringState->Pursuit();
				_pOwner->GetOtherVehicle()->SetActive();
			}
			else if (ImGui::RadioButton("Obstacle Avoidance", _isAvoiding)) {
				DisableBehaviors();
				_isAvoiding = true;
				pSteeringState->ObstacleAvoidance();
				_pOwner->GetOtherVehicle()->SetActive();
			}
			else if (ImGui::RadioButton("Wandering", _isWandering)) {
				DisableBehaviors();
				_isWandering = true;
				pSteeringState->Wandering();
			}
		}

		void DisableBehaviors()
		{
			_isSeeking = false;
			_isFleeing = false;
			_isArriving = false;
			_isPursuing = false;
			_isAvoiding = false;
			_isWandering = false;
			_pOwner->GetOtherVehicle()->SetActive(false);
		}

private:
	bool _isSteering = false;
	bool _isPathfinding = true;
	bool _isDecisionMaking = false;

	//Steering Settings
	bool _isSeeking = false;
	bool _isFleeing = false;
	bool _isArriving = false;
	bool _isPursuing = false;
	bool _isAvoiding = false;
	bool _isWandering = false;

	Vehicle* _pOwner;

	PathfindingState* pPathfindingState;
	SteeringState* pSteeringState;

	Sections _section;
};
#endif

