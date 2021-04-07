#include "AIManager.h"
#include "DrawableGameObject.h"
#include "PickupItem.h"
#include "Waypoint.h"

#include "main.h"
#include "Imgui/imgui.h"

#include "Steering.h"
#include "Vehicle.h"

#include "VehicleFSM.h"

Waypoint* AIManager::GetWaypoint(const unsigned int x, const unsigned int y)
{
	if (x < 0 || y < 0) return nullptr;

	if (x >= WAYPOINT_RESOLUTION || y >= WAYPOINT_RESOLUTION) return nullptr;

	assert(x >= 0 && x < WAYPOINT_RESOLUTION);
	assert(y >= 0 && y < WAYPOINT_RESOLUTION);
	return _waypoints[y * WAYPOINT_RESOLUTION + x];
}

HRESULT AIManager::initialise(ID3D11Device* pd3dDevice)
{
	InitializeWaypoints(pd3dDevice);
	
	//Initialize car objects with the device, texture path, starting position, waypoints and there max speed
	_pRaceCar = new Vehicle(pd3dDevice, L"Resources\\car_red.dds",
		GetWaypoint(10, 16)->GetVectorPosition(), _waypoints, 150.0f, "Race Car");

	_pDodgeCar = new Vehicle(pd3dDevice, L"Resources\\car_blue.dds",
		GetWaypoint(11, 14)->GetVectorPosition(), _waypoints, 130.0f, "Dodge Car");

	//Initialize the pickup item
	_pPickup = new PickupItem();
	HRESULT hr = _pPickup->initMesh(pd3dDevice);

	//Builds a vector of Vector2D objects to find which nodes are on the track, this is used so the
	//pickup item will always spawn on the track itself
	std::vector<Vector2D> placeablePoints;
	for (int i = 0; i < _waypoints.size(); i++) {
		if (_waypoints[i]->isOnTrack()) {
			placeablePoints.push_back(_waypoints[i]->GetVectorPosition());
		}
	}

	_pPickup->SetWaypoints(_waypoints);

	//Adds our race car as an observer to the pickup, uses the observer pattern
	_pPickup->SetPlaceablePositions(placeablePoints);
	_pPickup->AddObserver(_pRaceCar);

	//Initialize all the required data for the race car and dodge car
	_pRaceCar->SetOtherVehicle(_pDodgeCar);
	_pRaceCar->InitializeStates();

	_pDodgeCar->SetOtherVehicle(_pRaceCar);
	_pDodgeCar->InitializeStates();

	_pDodgeCar->SetPickup(_pPickup);
	_pRaceCar->SetPickup(_pPickup);

	//Changes the dodge car to the decision making state
	_pDodgeCar->GetStateMachine()->Section3AI();

	return hr;
}

void AIManager::InitializeWaypoints(ID3D11Device* pd3dDevice)
{
	//Calculate shared values for each waypoints
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
			_waypoints.push_back(wp);
		}
	}
}

void AIManager::update(const float fDeltaTime)
{
	//Updates our race car and pickup
	_pRaceCar->update(fDeltaTime);
	_pPickup->update(fDeltaTime);

	//Checks if the dodge car is active 
	if (_pDodgeCar->GetActive()) {
		//Update the dodge car
		_pDodgeCar->update(fDeltaTime);
	}

	//Checks for collisions this frame
	CollisionDetection();

	//Renders the frame
	Render(fDeltaTime);
}

void AIManager::DrawUI()
{
	_inMenus = false;

	//Checks if the windows are focused, meaning we are in the UI
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

	ImGui::Begin("Waypoints");
	ImGui::Checkbox("Display Waypoints", &_displayWaypoints);
	if (ImGui::IsWindowFocused()) {
		_inMenus = true;
	}
	ImGui::End();
}

void AIManager::Render(const float fDeltaTime)
{
	//Draws relevant UI
	DrawUI();
	_pRaceCar->DrawUI();

	//Updates and draws (if wanted) the waypoints
	for (unsigned int i = 0; i < _waypoints.size(); i++) {
		_waypoints[i]->update(fDeltaTime);
		if (_displayWaypoints) {
			AddItemToDrawList(_waypoints[i]); // if you comment this in, it will display the way points
		}
	}

	//Draws the pickup and racecar
	AddItemToDrawList(_pPickup);
	AddItemToDrawList(_pRaceCar);

	//Draws the dodge car if it's active
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

AIManager::~AIManager()
{
	//Deletes memory associated with AIManager variables
	delete _pPickup;
	_pPickup = nullptr;

	delete _pRaceCar;
	_pRaceCar = nullptr;

	delete _pDodgeCar;
	_pDodgeCar = nullptr;

	_waypoints.clear();
}

void AIManager::CollisionDetection()
{
	//Dummy vector for getting the transform of the world objects
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

	//Scale seemed "off" when it's at its full value, dividing by two fixed this
	raceCarScale.m128_f32[0] /= 2.0f;
	//Uses a oriented bounding box for collision checks
	BoundingOrientedBox orientedRaceCarBox;
	XMStoreFloat3(&orientedRaceCarBox.Center, raceCarPos);
	XMStoreFloat3(&orientedRaceCarBox.Extents, raceCarScale);
	XMStoreFloat4(&orientedRaceCarBox.Orientation, dummy);

	// the dodge car
	XMVECTOR dodgeCarDummy;
	XMVECTOR dodgeCarPos;
	XMVECTOR dodgeCarScale;
	XMMatrixDecompose(
		&dodgeCarScale,
		&dodgeCarDummy,
		&dodgeCarPos,
		XMLoadFloat4x4(_pDodgeCar->getTransform())
	);

	dodgeCarScale.m128_f32[0] /= 2.0f;
	BoundingOrientedBox orientedDodgeCarBox;
	XMStoreFloat3(&orientedDodgeCarBox.Center, dodgeCarPos);
	XMStoreFloat3(&orientedDodgeCarBox.Extents, dodgeCarScale);
	XMStoreFloat4(&orientedDodgeCarBox.Orientation, dodgeCarDummy);

	//Pickup system
	//generates transform for the pickup
	XMVECTOR puPos;
	XMVECTOR puScale;
	XMMatrixDecompose(
		&puScale,
		&dummy,
		&puPos,
		XMLoadFloat4x4(_pPickup->getTransform())
	);

	//Creates the bounding sphere for a pickup item
	BoundingSphere boundingSpherePU;
	XMStoreFloat3(&boundingSpherePU.Center, puPos);
	boundingSpherePU.Radius = puScale.m128_f32[0];

	//Checks if the race car collides with the pickup
	if (orientedRaceCarBox.Intersects(boundingSpherePU))
	{
		//Checks that the pickup has not already been picked up (shouldn't be, this is just for exceptions)
		if (!_pPickup->PickedUp()) {
			//Resolves the collision
			_pPickup->CollisionResolution();
		}
	}

	//Dont check for car collisions if the dodge car is not active
	if (_pDodgeCar->GetActive()) {
		if (orientedRaceCarBox.Intersects(orientedDodgeCarBox)) {
			_pRaceCar->ActivateCollisionPenalty();
		}
	}
	
}