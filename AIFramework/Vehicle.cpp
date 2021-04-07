#include "Vehicle.h"

#include "Steering.h"

#include "VehicleFSM.h"

#define MAX_SPEED 500

Vehicle::Vehicle(ID3D11Device* device, std::wstring textureName, Vector2D startPos, std::vector<Waypoint*> waypoints, float maxSpeed, std::string name)
	: _maxSpeed(maxSpeed), _waypoints(waypoints), _startingPosition(startPos), _vehicleName(name)
{
	_currentSpeed = _maxSpeed;

	_velocity = Vector2D(0, 0);

	m_scale = XMFLOAT3(30, 20, 1);

	SetVehiclePosition(startPos);

	setTextureName(textureName);
	DrawableGameObject::initMesh(device);

	_isSpeedBoostActive = false;
	_currentSpeedFactor = _defaultSpeedFactor;
}

void Vehicle::InitializeStates()
{
	pSteering = new Steering(this);

	pFSM = new VehicleFSM(this);
}

void Vehicle::update(const float deltaTime)
{
	pFSM->Update(deltaTime);

	Vector2D steeringForce;
	steeringForce = pSteering->CalculateForce();



	if (_isSpeedBoostActive)
	{
		_speedBoostTimer -= deltaTime;
		if (_speedBoostTimer <= 0.0f) {
			_isSpeedBoostActive = false;
			_currentSpeedFactor = _defaultSpeedFactor;
		}
	}

	if (_isCollisionPenaltyActive) {
		_collisionPenaltyTimer -= deltaTime;
		if (_collisionPenaltyTimer <= 0.0f) {
			_isCollisionPenaltyActive = false;
			_currentSpeedFactor = _defaultSpeedFactor;
		}
	}

	Vector2D acceleration = (steeringForce * _currentSpeedFactor * 3.0) / _mass;

	_velocity += acceleration * deltaTime;


	//Stops the velocity going beyond our maximum speed
	_velocity.Truncate(_maxSpeed);

	//Multiplies our velocity by the speed factor (1.1 for speed boost, 0.5 for collision penalty)
	_velocity *= _currentSpeedFactor;

	//Adds the velocity to our current position
	_currentPosition += _velocity * deltaTime;

	//Calculates our current speed (used in steering behaviors) 
	_currentSpeed = _velocity.Length();

	// rotate the object based on its last & current position
	Vector2D diff = _currentPosition - _lastPosition;
	if (diff.Length() > 0) { // if zero then don't update rotation
		diff.Normalize();
		m_radianRotation = atan2f(diff.y, diff.x); // this is used by DrawableGameObject to set the rotation
	}

	_lastPosition = _currentPosition;

	//Calculate the forward vector
	_forward = Vector2D(cosf(m_radianRotation), sinf(m_radianRotation));

	setDirection(XMFLOAT3(_forward.x, _forward.y, 0.0f));

	//Calculate the right vector
	_side = _forward.Perp();

	// set the current position for the drawable Gameobject
	setPosition(XMFLOAT3((float)_currentPosition.x, (float)_currentPosition.y, 0));

	DrawableGameObject::update(deltaTime);
}

void Vehicle::DrawUI()
{
	ImGui::Begin("Car Information");
	ImGui::Text("Car Details");
	ImGui::Text("Wander Target: %.2f, %.2f", _wanderTarget.x, _wanderTarget.y);

	if (_isSpeedBoostActive) {
		ImGui::Text("Speed boost active, time left: %.2f", _speedBoostTimer);
	}
	else
	{
		ImGui::Text("Speed boost inactive");
	}

	if (_isCollisionPenaltyActive) {
		ImGui::Text("Collision Penalty active, time left: %.2f", _collisionPenaltyTimer);
	}
	else {
		ImGui::Text("No Collision Penalty");
	}
	ImGui::End();


	pFSM->DrawUI();
}

void Vehicle::ActivateCollisionPenalty()
{
	_isCollisionPenaltyActive = true;
	_collisionPenaltyTimer = _collisionPenaltyDuration;
	_currentSpeedFactor = _collisionSpeedFactor;

}

void Vehicle::ResetVehicle()
{
	SetVehiclePosition(_startingPosition);
	_currentSpeedFactor = _defaultSpeedFactor;
	pSteering->ClearFlags();
	_velocity = Vector2D(0, 0);
}

void Vehicle::ResetState()
{
	pFSM->SetWaypointTolerance(50.0f);
	pFSM->Section3AI();
}

void Vehicle::SetSteeringTarget(Vector2D pos)
{
	if (pFSM->GetSection() == VehicleFSM::steering) {
		SetPositionTo(pos);
	}
}

void Vehicle::OnNotify(const Entity& entity, Event event)
{

}

void Vehicle::OnNotify(Event event)
{
	switch (event)
	{
	case PICKUP_SPAWNED:
		pFSM->Pickup();

		break;
	case PICKUP_COLLECTED:
		_isSpeedBoostActive = true;
		_speedBoostTimer = _speedBoostDuration;
		_currentSpeedFactor = _pickupSpeedFactor;

		break;
	default:
		break;
	}
}
