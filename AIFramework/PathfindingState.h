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
		_index = 0;
		pTrack = new Track("Resources/waypoints.txt");

		pOwner->GetSteering()->ArriveOn();

		NextTarget();
	}

	void OnExit() override {

	}


	void Update(float dt) override {
		if (pOwner->GetVectorPosition().Distance(_targetPosition) < 55.0) {
			NextTarget();
		}
	}

private:

	void NextTarget()
	{
		if (_currentPath.empty()) {
			_index++;

			if (_index >= 15) {
				_index = 0;
			}

			if (_index == 0) {

				pTrack->SolvePathToNextPoint(14, 0);
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


		XMFLOAT3* wpPosition = pOwner->GetWaypoint(pCurrentNode->xPos, pCurrentNode->yPos)->getPosition();

		_targetPosition = Vector2D(wpPosition->x, wpPosition->y);

		pOwner->setPositionTo(_targetPosition);
	}

private:
	int _index = 0;

	Node* pCurrentNode = nullptr;
	std::list<Node*> _currentPath;

	Vector2D _targetPosition;

	Track* pTrack = nullptr;

};
