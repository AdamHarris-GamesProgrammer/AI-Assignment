#include "AIManager.h"
#include "DrawableGameObject.h"
#include "PickupItem.h"
#include "Waypoint.h"

#include "main.h"
#include "Imgui/imgui.h"

#include "Steering.h"
#include "Vehicle.h"

Waypoint* AIManager::GetWaypoint(const unsigned int x, const unsigned int y)
{
	if (x < 0 || y < 0) return nullptr;

	if (x >= WAYPOINT_RESOLUTION || y >= WAYPOINT_RESOLUTION) return nullptr;

	assert(x >= 0 && x < WAYPOINT_RESOLUTION);
	assert(y >= 0 && y < WAYPOINT_RESOLUTION);

	return m_waypoints[y * WAYPOINT_RESOLUTION + x];
}

HRESULT AIManager::initialise(ID3D11Device* pd3dDevice)
{
	// create a pickup item ----------------------------------------------

	PickupItem* pPickup = new PickupItem();
	HRESULT hr = pPickup->initMesh(pd3dDevice);
	m_pickups.push_back(pPickup);


	_pRaceCar = new Vehicle(pd3dDevice, L"Resources\\car_red.dds");
	_pRaceCar->setMaxSpeed(150.0f);

	_pDodgeCar = new Vehicle(pd3dDevice, L"Resources\\car_blue.dds");
	_pDodgeCar->setMaxSpeed(50.0f);

	InitializeWaypoints(pd3dDevice);

	_pRaceCar->SetWaypoints(m_waypoints);
	_pDodgeCar->SetWaypoints(m_waypoints);


	Vector2D position = GetWaypoint(11, 15)->GetVectorPosition();
	_pRaceCar->setPosition(XMFLOAT3(position.x, position.y, 0.0f));
	_pRaceCar->setPositionTo(Vector2D(0, 0));

	Vector2D dodgePosition = GetWaypoint(11, 16)->GetVectorPosition();
	_pDodgeCar->setPosition(XMFLOAT3(dodgePosition.x, dodgePosition.y, 0.0f));
	_pDodgeCar->setPositionTo(Vector2D(0, 0));

	_pRaceCar->InitializeStates();
	_pDodgeCar->InitializeStates();

	_pRaceCar->SetOtherVehicle(_pDodgeCar);
	_pDodgeCar->SetOtherVehicle(_pRaceCar);

	_pDodgeCar->SetActive(false);
	

	return hr;
}

void AIManager::InitializeWaypoints(ID3D11Device* pd3dDevice)
{
	//Calculate shared values for each waypoiny
	float xGap = SCREEN_WIDTH / WAYPOINT_RESOLUTION;
	float yGap = SCREEN_HEIGHT / WAYPOINT_RESOLUTION;
	float xStart = -(SCREEN_WIDTH / 2) + (xGap / 2);
	float yStart = -(SCREEN_HEIGHT / 2) + (yGap / 2);

	//Adds the waypoints to the mesh system
	unsigned int index = 0;
	for (unsigned int j = 0; j < WAYPOINT_RESOLUTION; j++) {
		for (unsigned int i = 0; i < WAYPOINT_RESOLUTION; i++) {
			Waypoint* wp = new Waypoint();
			wp->initMesh(pd3dDevice, index++);
			wp->setPosition(XMFLOAT3(xStart + (xGap * i), yStart + (yGap * j), 0));
			m_waypoints.push_back(wp);
		}
	}
}

void AIManager::update(const float fDeltaTime)
{
	_pRaceCar->update(fDeltaTime);


	//Checks we are active 
	if (_pDodgeCar->GetActive()) {
		_pDodgeCar->update(fDeltaTime);
	}


	checkForCollisions();
	Render(fDeltaTime);

}

void AIManager::DrawUI()
{
	_inMenus = false;

	ImGui::Begin("Car Information");
	if (ImGui::IsWindowFocused()) {
		_inMenus = true;

	}
	ImGui::End();

	ImGui::Begin("AI Control Panel");
	if (ImGui::IsWindowFocused()) {
		_inMenus = true;
	}
	ImGui::End();

}

void AIManager::Render(const float fDeltaTime)
{
	DrawUI();
	_pRaceCar->DrawUI();

	for (unsigned int i = 0; i < m_waypoints.size(); i++) {
		m_waypoints[i]->update(fDeltaTime);
		//AddItemToDrawList(m_waypoints[i]); // if you comment this in, it will display the way points
	}

	for (unsigned int i = 0; i < m_pickups.size(); i++) {
		m_pickups[i]->update(fDeltaTime);
		AddItemToDrawList(m_pickups[i]);
	}

	AddItemToDrawList(_pRaceCar);

	if (_pDodgeCar->GetActive()) {
		AddItemToDrawList(_pDodgeCar);
	}
}

void AIManager::mouseUp(int x, int y)
{
	//If we are not over a menu then set our steering target
	if (!_inMenus) { 
		_pRaceCar->SetSteeringTarget(Vector2D(x, y));
	}
}

void AIManager::keyPress(WPARAM param)
{
	switch (param)
	{
	case VK_NUMPAD0:
	{
		OutputDebugStringA("0 pressed \n");
		break;
	}
	case VK_NUMPAD1:
	{
		OutputDebugStringA("1 pressed \n");
		break;
	}
	case VK_NUMPAD2:
	{
		break;
	}
	default:
		break;
	}
}

bool AIManager::checkForCollisions()
{
	if (m_pickups.size() == 0)
		return false;

	XMVECTOR dummy;

	// the car
	XMVECTOR carPos;
	XMVECTOR carScale;
	XMMatrixDecompose(
		&carScale,
		&dummy,
		&carPos,
		XMLoadFloat4x4(_pRaceCar->getTransform())
	);

	XMFLOAT3 scale;
	XMStoreFloat3(&scale, carScale);
	BoundingSphere boundingSphereCar;
	XMStoreFloat3(&boundingSphereCar.Center, carPos);
	boundingSphereCar.Radius = scale.x;

	// a pickup - !! NOTE it is only referring the first one in the list !!
	XMVECTOR puPos;
	XMVECTOR puScale;
	XMMatrixDecompose(
		&puScale,
		&dummy,
		&puPos,
		XMLoadFloat4x4(m_pickups[0]->getTransform())
	);

	XMStoreFloat3(&scale, puScale);
	BoundingSphere boundingSpherePU;
	XMStoreFloat3(&boundingSpherePU.Center, puPos);
	boundingSpherePU.Radius = scale.x;

	// test
	if (boundingSphereCar.Intersects(boundingSpherePU))
	{
		OutputDebugStringA("Pickup Collision!\n");
		return true;
	}

	return false;
}