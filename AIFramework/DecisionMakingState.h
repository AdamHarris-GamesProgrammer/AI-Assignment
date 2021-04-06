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
		pOwner->ResetVehicle();

		_index = 0;
		pTrack = new Track("Resources/waypoints.txt");

		_numOfWaypoints = pTrack->GetConverter()->GetWaypoints().size();


		if (pOwner->GetName() == "Race Car") {
			pOwner->GetOtherVehicle()->SetActive();
			pOwner->GetOtherVehicle()->ResetState();
		}
		

		pOwner->GetSteering()->SeekOn();
		pOwner->GetSteering()->ObstacleAvoidanceOn();

		NextTarget();
	}

	void SetWaypointTolerance(float val) {
		_waypointTolerance = val;
	}

	void OnExit() override
	{
		_index = 0;
		pCurrentNode = nullptr;
		_currentPath.clear();
		_isFinished = false;
		_lapCounter = 1;
	}


	void Update(float dt) override
	{
		DrawUI();


		if (_isFinished) return;

		if (pOwner->GetSteering()->IsWantingToOvertake()) {
			_waypointToleranceFactor = 2.0f;
		}
		else {
			_waypointToleranceFactor = 1.0f;
		}

		//Using the Squared distance saves on a square root operation which is computationally expensive
		if (Vec2DDistanceSq(pOwner->GetVectorPosition(), _targetPosition) < (_waypointTolerance * _waypointToleranceFactor) * (_waypointTolerance * _waypointToleranceFactor)) {
			NextTarget();
		}
	}
	
	void PickupSpawned() {
		_pickupIndex = pOwner->GetPickup()->GetIndex();
	}

private:
	void DrawUI() {
		std::ostringstream lap;
		lap  << pOwner->GetName() << " " << "Lap " << _lapCounter << "/" << _numOfLaps;

		std::ostringstream index;
		index << pOwner->GetName() << " " << "Index " << _index + 1 << "/" << _numOfWaypoints;

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


			if (_index == _pickupIndex) {
				float pathEffort = pTrack->GetNodePath().size();
				
				PickupItem* item = pOwner->GetPickup();
				Vector2D start = item->GetCurrentWaypoint()->GetTilePosition();
				Vector2D mid = item->GetTilePosition();
				Vector2D end = item->GetNextWaypoint()->GetTilePosition();

				pTrack->SolvePathToNextPoint(start, mid);
				std::list<Node*> path = pTrack->GetNodePath();

				float pickupPathEffort = path.size();
				if (pickupPathEffort / 1.75 < pathEffort) {
					_currentPath.clear();
					_currentPath = path;
				}
				else
				{
					pTrack->SolvePathToNextPoint(start, end);
					_currentPath.clear();
					_currentPath = pTrack->GetNodePath();
				}
			}
			else
			{
				_currentPath.clear();
				_currentPath = pTrack->GetNodePath();
			}
			
		}

		pCurrentNode = _currentPath.back();
		_currentPath.pop_back();

		
		if (_lapCounter == _numOfLaps && _index == _numOfWaypoints - 1) {
			if (_currentPath.size() == 1) {
				pOwner->GetSteering()->SeekOff();
				pOwner->GetSteering()->ArriveOn();
				_isFinished = true;

				Vector2D pos = pOwner->GetStartPosition();

				pos.x += 60.0f;

				_targetPosition = pos;
				pOwner->SetPositionTo(_targetPosition);
			}

		}
		else
		{
			XMFLOAT3* wpPosition = pOwner->GetWaypoint(pCurrentNode->pos)->getPosition();

			_targetPosition = Vector2D(wpPosition->x, wpPosition->y);

			pOwner->SetPositionTo(_targetPosition);
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

	Node* pCurrentNode = nullptr;
	std::list<Node*> _currentPath;

	Vector2D _targetPosition;

	Track* pTrack = nullptr;
};