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
	/// <summary>
	/// Gets a waypoint based on it's X and Y tile position
	/// </summary>
	/// <param name="x">X Tile of the waypoint</param>
	/// <param name="y">Y Tile of the waypoint</param>
	/// <returns>Returns a pointer to the Waypoint at this position</returns>
	Waypoint* GetWaypoint(const unsigned int x, const unsigned int y);

	HRESULT initialise(ID3D11Device* pd3dDevice);

	void InitializeWaypoints(ID3D11Device* pd3dDevice);

	void	update(const float fDeltaTime);

	void DrawUI();

	void Render(const float fDeltaTime);

	void	mouseUp(int x, int y);
	void	keyPress(WPARAM param);

	~AIManager();

protected:
	/// <summary>
	/// Collision Detection and Resolution
	/// </summary>
	void	CollisionDetection();

private:
	vecWaypoints            _waypoints;
	//Single Pickup pointer
	PickupItem* _pPickup;
	Vehicle*				_pRaceCar = nullptr;
	Vehicle* _pDodgeCar = nullptr;

	bool _inMenus = false;
	bool _displayWaypoints = false;


};

