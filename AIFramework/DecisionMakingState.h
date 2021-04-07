#pragma once
#include "State.h"
#include "Vector2D.h"
#include "Track.h"
#include "Node.h"
#include "Waypoint.h"
#include "Steering.h"

#include "PickupItem.h"

#include <sstream>

class Vehicle;

class DecisionMakingState : public State {
public:
	DecisionMakingState(Vehicle* owner) : State(owner) {}

	void OnEnter() override
	{
		_pOwner->ResetVehicle();

		_index = 0;
		pTrack = new Track("Resources/waypoints.txt");

		_numOfWaypoints = pTrack->GetConverter()->GetWaypoints().size();


		if (_pOwner->GetName() == "Race Car") {
			_pOwner->GetOtherVehicle()->SetActive();
			_pOwner->GetOtherVehicle()->ResetState();
		}


		_pOwner->GetSteering()->SeekOn();
		_pOwner->GetSteering()->ObstacleAvoidanceOn();

		NextTarget();
	}

	void SetWaypointTolerance(float val) {
		_waypointTolerance = val;
	}

	void OnExit() override
	{
		_index = 0;
		_pCurrentNode = nullptr;
		_currentPath.clear();
		_isFinished = false;
		_lapCounter = 1;
	}


	void Update(float dt) override
	{
		DrawUI();

		if (_isFinished) return;

		//Adjust our waypoint tolerance if we can overtake
		if (_pOwner->GetSteering()->IsWantingToOvertake()) {
			_waypointToleranceFactor = 2.0f;
		}
		else {
			_waypointToleranceFactor = 1.0f;
		}

		float totalTolerance = _waypointTolerance * _waypointToleranceFactor;
		//Using the Squared distance saves on a square root operation which is computationally expensive
		if (Vec2DDistanceSq(_pOwner->GetVectorPosition(), _targetPosition) < totalTolerance * totalTolerance) {
			NextTarget();
		}
	}

	void PickupSpawned() {
		_pickupIndex = _pOwner->GetPickup()->GetIndex();
	}

private:
	void DrawUI() {
		//ImGUI sometimes has a glitch where integers are not correctly displayed on screen,
		//to get around this I use a ostringstream and convert it to a const char* for ImGUI to display
		std::ostringstream lap;
		lap << _pOwner->GetName() << " " << "Lap " << _lapCounter << "/" << _numOfLaps;

		std::ostringstream index;
		index << _pOwner->GetName() << " " << "Index " << _index + 1 << "/" << _numOfWaypoints;

		ImGui::Begin("RACE MODE");
		ImGui::Text(lap.str().c_str());
		ImGui::Text(index.str().c_str());
		ImGui::End();
	}

	void NextTarget()
	{
		if (_currentPath.empty()) {
			_index = (_index + 1) % _numOfWaypoints;

			if (_index == 0) {
				pTrack->SolvePathToNextPoint(_numOfWaypoints - 1, 0);
				_lapCounter++;
			}
			else
			{
				pTrack->SolvePathToNextPoint(_index - 1, _index);
			}

			//if a pickup is on this current waypoint of the map check if it is worth collecting
			if (_index == _pickupIndex) {
				PickupLogic();
			}
			//if it isn't then clear our path and get the new path
			else
			{
				_currentPath.clear();
				_currentPath = pTrack->GetNodePath();
			}
		}

		//Get our next node
		_pCurrentNode = _currentPath.back();

		//Remove the last node from the list
		_currentPath.pop_back();

		//If we have finished the face then perform our finishing logic
		if (_lapCounter == _numOfLaps && _index == _numOfWaypoints - 1) {
			if (_currentPath.size() == 1) {
				FinishedRaceLogic();
			}
		}
		else
		{
			XMFLOAT3* wpPosition = _pOwner->GetWaypoint(_pCurrentNode->pos)->getPosition();

			_targetPosition = Vector2D(wpPosition->x, wpPosition->y);

			_pOwner->SetPositionTo(_targetPosition);
		}
	}

	void FinishedRaceLogic()
	{
		//The ai will no longer seek to the target and instead arrive, this adds deceleration
		_pOwner->GetSteering()->SeekOff();
		_pOwner->GetSteering()->ArriveOn();
		//Sets the finishing flag to true, this stops the race from going on forever
		_isFinished = true;

		//Gets the vehichles starting position
		Vector2D pos = _pOwner->GetStartPosition();
		//Appends 100 pixels to the end of it
		pos.x += 100.0f;

		//This puts the finishing position just after the finishing line (just a cosmetic touch)
		_targetPosition = pos;
		_pOwner->SetPositionTo(_targetPosition);
	}

	void PickupLogic()
	{
		//Calculates the original path effort based on the length of the path
		float pathEffort = pTrack->GetNodePath().size();

		//gets the pickup item
		PickupItem* item = _pOwner->GetPickup();

		//Gets the position of the start waypoint, pickup, and ending position
		Vector2D start = item->GetCurrentWaypoint()->GetTilePosition();
		Vector2D mid = item->GetTilePosition();
		Vector2D end = item->GetNextWaypoint()->GetTilePosition();

		//creates a path from the starting point to the pickup
		pTrack->SolvePathToNextPoint(start, mid);

		//Stores this path in a separate list
		std::list<Node*> path = pTrack->GetNodePath();

		//Clears the path of the track
		pTrack->GetNodePath().clear();
		//Solves the path from the mid to end 
		pTrack->SolvePathToNextPoint(mid, end);

		//Stores this in a list to append to the front of the other list
		std::list<Node*> appendList = pTrack->GetNodePath();

		//Pushes the node from the list to the front of the path as A* pops from the back 
		for (auto& node : appendList) {
			path.push_front(node);
		}


		//calculates the effort of the pickup path based on effort
		float pickupPathEffort = path.size();

		//Calculates if it is worth it by dividing the pickup path effort by a factor and seeing if it
		//is smaller than the path effort. This is because the pickup will give a speed boost
		/*Example
		Default path effort = 12
		Pickup path effort equals 14
		14 / 1.2 = 11.66 (recurring)
		11.66 < 12 (original path effort) therefore it is worth collecting the pickup
		*/
		if (pickupPathEffort / 1.2 < pathEffort) {
			//Sets our current path to the new path
			_currentPath.clear();
			_currentPath = path;
		}
		else
		{
			//Sets our path to the original path
			pTrack->SolvePathToNextPoint(start, end);
			_currentPath.clear();
			_currentPath = pTrack->GetNodePath();
		}
	}

	int _numOfWaypoints;

	int _index = 0;

	int _lapCounter = 1;
	int _numOfLaps = 5;

	bool _isFinished = false;

	int _pickupIndex;

	float _waypointToleranceFactor = 1.0f;

	//This value is how closely the car will follow the path, higher value is less accurate the path but more realistic looking. 
	//higher values also work better when it comes to overtaking
	float _waypointTolerance = 50.0f;

	Node* _pCurrentNode = nullptr;
	std::list<Node*> _currentPath;

	Vector2D _targetPosition;

	Track* pTrack = nullptr;
};