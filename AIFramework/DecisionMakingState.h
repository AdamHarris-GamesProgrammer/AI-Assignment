#pragma once
#include "State.h"
#include "Vector2D.h"
#include "Track.h"
#include "Node.h"
#include "Waypoint.h"
#include "Steering.h"

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

		pOwner->GetOtherVehicle()->SetActive();
		pOwner->GetSteering()->SeekOn();
		pOwner->GetSteering()->ObstacleAvoidanceOn();

		NextTarget();
	}


	void OnExit() override
	{
		_index = 0;
		pCurrentNode = nullptr;
		_currentPath.clear();
	}


	void Update(float dt) override
	{
		DrawUI();


		if (_isFinished) return;

		//Using the Squared distance saves on a square root operation which is computationally expensive
		if (Vec2DDistanceSq(pOwner->GetVectorPosition(), _targetPosition) < _waypointTolerance * _waypointTolerance) {
			NextTarget();
		}
	}

private:
	void DrawUI() {
		std::ostringstream lap;
		lap  << pOwner->GetName() << " " << "Lap " << _lapCounter << "/" << _numOfLaps;

		std::ostringstream index;
		index << pOwner->GetName() << " " << "Index " << _index << "/" << _numOfWaypoints;

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

			_currentPath.clear();
			_currentPath = pTrack->GetNodePath();
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
	int _numOfLaps = 1;

	bool _isFinished = false;

	//This value is how closely the car will follow the path, higher value is less accurate the path but more realistic looking. 
	//higher values also work better when it comes to overtaking
	float _waypointTolerance = 90.0f;

	Node* pCurrentNode = nullptr;
	std::list<Node*> _currentPath;

	Vector2D _targetPosition;

	Track* pTrack = nullptr;
};