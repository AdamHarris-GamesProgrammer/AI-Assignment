#include "PathfindingState.h"

#include "Vehicle.h"



PathfindingState::PathfindingState(Vehicle* owner) : State(owner)
{

}

void PathfindingState::OnEnter()
{
	_index = 0;
	pTrack = new Track("Resources/waypoints.txt");

	pOwner->GetSteering()->SeekOff();
	pOwner->GetSteering()->ArriveOn();

	NextTarget();
}

void PathfindingState::OnExit()
{

}

void PathfindingState::Update(float dt)
{
	//pOwner->GetSteering()->Arrive(_targetPosition);

	if (pOwner->GetVectorPosition().Distance(_targetPosition) < 15.0) {
		NextTarget();
	}
}

void PathfindingState::NextTarget()
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
