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
		pPathfindingState = new PathfindingState(owner);
		
		pSteeringState = new SteeringState(owner);

		ChangeState(pPathfindingState);

		_section = steering;
	}

	void Section1AI() {
		_section = steering;
		ChangeState(pSteeringState);
	}

	void Section2AI() {
		_section = pathfinding;
		ChangeState(pPathfindingState);
	}

	void Section3AI() {
		_section = pathfinding;
	}

	void SeekOn() {
		if (_section == steering) {
			pSteeringState->Seek();
		}
	}

	void FleeOn() {
		if (_section == steering) {
			pSteeringState->Flee();
		}
	}

	void ArriveOn() {
		if (_section == steering) {
			pSteeringState->Arrive();
		}
	}

	void PursuitOn() {
		if (_section == steering) {
			pSteeringState->Pursuit();
		}
	}

	void ObstacleAvoidanceOn() {
		if (_section == steering) {
			pSteeringState->ObstacleAvoidance();
		}
	}

	void WanderingOn() {
		if (_section == steering) {
			pSteeringState->Wandering();
		}
	}

	Sections GetSection() const {
		return _section;
	}

private:
	PathfindingState* pPathfindingState;
	SteeringState* pSteeringState;



	Sections _section;
};
#endif

