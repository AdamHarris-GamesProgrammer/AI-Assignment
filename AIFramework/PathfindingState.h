#ifndef PATHFINDINGSTATE_H
#define PATHFINDINGSTATE_H

#include "State.h"
#include "Vector2D.h"
#include "Track.h"
#include "Node.h"
#include "Waypoint.h"
#include "Steering.h"

class Vehicle;

class PathfindingState : public State {

public:
	PathfindingState(Vehicle* owner);

	void OnEnter() override;


	void OnExit() override;


	void Update(float dt) override;

private:
	void NextTarget();


private:
	int _index = 0;

	Node* pCurrentNode = nullptr;
	std::list<Node*> _currentPath;

	Vector2D _targetPosition;

	Track* pTrack = nullptr;

};
#endif