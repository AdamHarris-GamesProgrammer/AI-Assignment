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
	InitializeWaypoints(pd3dDevice);

	//Initialize car objects with the device, texture path, starting position, waypoints and there max speed
	_pRaceCar = new Vehicle(pd3dDevice, L"Resources\\car_red.dds",
		GetWaypoint(11, 14)->GetVectorPosition(), m_waypoints, 150.0f);

	_pDodgeCar = new Vehicle(pd3dDevice, L"Resources\\car_blue.dds",
		GetWaypoint(10, 16)->GetVectorPosition(), m_waypoints, 120.0f);

	//Initialize the pickup item
	_pPickup = new PickupItem();
	HRESULT hr = _pPickup->initMesh(pd3dDevice);

	//Builds a vector of Vector2D objects to find which nodes are on the track, this is used so the
	//pickup item will always spawn on the track itself
	std::vector<Vector2D> placeablePoints;
	for (int i = 0; i < m_waypoints.size(); i++) {
		if (m_waypoints[i]->isOnTrack()) {
			placeablePoints.push_back(m_waypoints[i]->GetVectorPosition());
		}
	}

	_pPickup->SetPlaceablePositions(placeablePoints);
	//_pPickup->GenerateNewPosition();
	_pPickup->AddObserver(_pRaceCar);


	_pRaceCar->SetOtherVehicle(_pDodgeCar);
	_pRaceCar->InitializeStates();

	_pDodgeCar->SetOtherVehicle(_pRaceCar);
	_pDodgeCar->InitializeStates();


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
	_pPickup->update(fDeltaTime);

	//Checks we are active 
	if (_pDodgeCar->GetActive()) {
		_pDodgeCar->update(fDeltaTime);
	}

	CollisionDetection();


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

	AddItemToDrawList(_pPickup);

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
		break;
	}
	}
}

void AIManager::CollisionDetection()
{
	XMVECTOR dummy;

	// the race car
	XMVECTOR raceCarPos;
	XMVECTOR raceCarScale;
	XMMatrixDecompose(
		&raceCarScale,
		&dummy,
		&raceCarPos,
		XMLoadFloat4x4(_pRaceCar->getTransform())
	);

	XMFLOAT3 raceScale;
	XMStoreFloat3(&raceScale, raceCarScale);
	BoundingSphere bsRaceCar;
	XMStoreFloat3(&bsRaceCar.Center, raceCarPos);
	bsRaceCar.Radius = raceScale.x;

	// the dodge car
	XMVECTOR dodgeCarPos;
	XMVECTOR dodgeCarScale;
	XMMatrixDecompose(
		&dodgeCarScale,
		&dummy,
		&dodgeCarPos,
		XMLoadFloat4x4(_pDodgeCar->getTransform())
	);

	XMFLOAT3 dodgeScale;
	XMStoreFloat3(&dodgeScale, dodgeCarScale);
	BoundingSphere bsDodgeCar;
	XMStoreFloat3(&bsDodgeCar.Center, dodgeCarPos);
	bsDodgeCar.Radius = dodgeScale.x;

	BoundingBox raceCarBox;
	XMStoreFloat3(&raceCarBox.Center, raceCarPos);
	XMStoreFloat3(&raceCarBox.Extents, raceCarScale);

	BoundingBox dodgeCarBox;
	XMStoreFloat3(&dodgeCarBox.Center, dodgeCarPos);
	XMStoreFloat3(&dodgeCarBox.Extents, dodgeCarScale);

	//Pickup system
	XMVECTOR puPos;
	XMVECTOR puScale;
	XMMatrixDecompose(
		&puScale,
		&dummy,
		&puPos,
		XMLoadFloat4x4(_pPickup->getTransform())
	);

	XMStoreFloat3(&raceScale, puScale);
	BoundingSphere boundingSpherePU;
	XMStoreFloat3(&boundingSpherePU.Center, puPos);
	boundingSpherePU.Radius = raceScale.x;

	if (bsRaceCar.Intersects(boundingSpherePU))
	{
		if (!_pPickup->PickedUp()) {
			_pPickup->CollisionResolution();
		}
	}

	//Dont check for car collisions if they are not active
	if (_pDodgeCar->GetActive()) {
		if (raceCarBox.Intersects(dodgeCarBox)) {
			_pRaceCar->ActivateCollisionPenalty();
		}
	}
	
}