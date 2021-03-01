#pragma once
#ifndef VEHICLEFSM_H
#define VEHICLEFSM_H

#include "FSMManager.h"
#include "Vehicle.h"
#include "PathfindingState.h"



class VehicleFSM : public FSMManager
{
public:
	VehicleFSM(Vehicle* owner) {
		//initializes pathfinding state
		pPathfindingState = new PathfindingState(owner);

		ChangeState(pPathfindingState);
	}

private:
	PathfindingState* pPathfindingState;

};
#endif

