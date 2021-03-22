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

	void InitializeWaypoints(ID3D11Device* pd3dDevice);

	void	update(const float fDeltaTime);

	void DrawUI();

	void Render(const float fDeltaTime);

	void	mouseUp(int x, int y);
	void	keyPress(WPARAM param);

protected:
	void	CollisionDetection();

private:
	vecWaypoints            m_waypoints;
	//vecPickups              m_pickups;
	PickupItem* _pPickup;
	Vehicle*				_pRaceCar = nullptr;
	Vehicle* _pDodgeCar = nullptr;

	bool _inMenus = false;


};

