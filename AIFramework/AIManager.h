#pragma once

#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxcolors.h>
#include <DirectXCollision.h>
#include <vector>
#include "Track.h"

#include <list>
#include "Node.h"
#include "Vector2D.h"

using namespace std;

class Vehicle;
class DrawableGameObject;
class PickupItem;
class Waypoint;

typedef vector<DrawableGameObject*> vecDrawables;
typedef vector<Waypoint*> vecWaypoints;
typedef vector<PickupItem*> vecPickups;

class AIManager
{
public:

	Waypoint* GetWaypoint(const unsigned int x, const unsigned int y);

	HRESULT initialise(ID3D11Device* pd3dDevice);
	void	update(const float fDeltaTime);
	void	mouseUp(int x, int y);
	void	keyPress(WPARAM param);

protected:
	bool	checkForCollisions();

private:
	Vector2D ConvertPosition(Vector2D pos);
	void NextTarget();

	bool _isFollowingTrack = true;

	vecWaypoints            m_waypoints;
	vecPickups              m_pickups;
	Vehicle*				m_pCar = nullptr;

	Node* _currentNode;
	std::list<Node*> _currentPath;
	Vector2D _targetPosition;

	Track* mTrack = nullptr;
	int _index = 0;
};

