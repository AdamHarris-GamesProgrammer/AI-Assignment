#pragma once
#include "State.h"
#include "Vector2D.h"
#include "Track.h"
#include "Node.h"
#include "Waypoint.h"
#include "Steering.h"

class Vehicle;

class PathfindingState : public State {

public:
	PathfindingState(Vehicle* owner) : State(owner) {}

	void OnEnter() override
	{
		_pOwner->ResetVehicle();
		_index = 0;
		pTrack = new Track("Resources/waypoints.txt");

		_numOfWaypoints = pTrack->GetConverter()->GetWaypoints().size();

		_pOwner->GetSteering()->SeekOn();
		
		_pOwner->GetOtherVehicle()->SetActive(false);

		NextTarget();
	}

	void OnExit() override {
		_index = 0;
		pCurrentNode = nullptr;
		_currentPath.clear();
	}


	void Update(float dt) override {
		if (GetAsyncKeyState('Q')) {
			_index = 0;
			pTrack->SolvePathToNextPoint(pCurrentNode->pos, Vector2D(11,15));
			_currentPath = pTrack->GetNodePath();

			XMFLOAT3* wpPosition = _pOwner->GetWaypoint(pCurrentNode->pos)->getPosition();

			_targetPosition = Vector2D(wpPosition->x, wpPosition->y);

			_pOwner->SetPositionTo(_targetPosition);
		}


		//Using the Squared distance saves on a square root operation which is computationally expensive
		if (Vec2DDistanceSq(_pOwner->GetVectorPosition(), _targetPosition) <_waypointTolerance * _waypointTolerance) {
			NextTarget();
		}
	}

	void DrawUI() {
		ImGui::Text("Waypoint tolerance \nadjusts how closely the \nAI will stick \nto the path");
		ImGui::SliderFloat("Waypoint Tolerance: ", &_waypointTolerance, 1.0f, 150.0f, "%.2f", 1.0f);
	}

private:

	void NextTarget()
	{
		if (_currentPath.empty()) {
			_index = (_index + 1) % _numOfWaypoints;

			if (_index == 0) {
				pTrack->SolvePathToNextPoint(_numOfWaypoints - 1, 0);
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


		XMFLOAT3* wpPosition = _pOwner->GetWaypoint(pCurrentNode->pos)->getPosition();

		_targetPosition = Vector2D(wpPosition->x, wpPosition->y);

		_pOwner->SetPositionTo(_targetPosition);
	}

private:
	int _numOfWaypoints;

	int _index = 0;

	//This value is how closely the car will follow the path, higher value is less accurate the path but more realistic looking. 
	//higher values also work better when it comes to overtaking
	float _waypointTolerance = 50.0f;

	Node* pCurrentNode = nullptr;
	std::list<Node*> _currentPath;

	Vector2D _targetPosition;

	Track* pTrack = nullptr;

};
