#include "Vehicle.h"

#include "Steering.h"

#include "VehicleFSM.h"

#define MAX_SPEED 150

Vehicle::Vehicle(ID3D11Device* device, std::wstring textureName)
{
	m_scale = XMFLOAT3(30, 20, 1);
	SetMaxSpeed(MAX_SPEED);
	_currentSpeed = _maxSpeed;

	SetVehiclePosition(Vector2D(74.5, 205));

	setTextureName(textureName);
	DrawableGameObject::initMesh(device);
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
			_steerSpeedFactor = 1.0f;
		}
	}

	if (_isCollisionPenaltyActive) {
		_collisionPenaltyTimer -= deltaTime;
		if (_collisionPenaltyTimer <= 0.0f) {
			_isCollisionPenaltyActive = false;
			_steerSpeedFactor = 1.0f;
		}
	}

	Vector2D acceleration = (steeringForce * _steerSpeedFactor) / _mass;

	_velocity += acceleration * deltaTime;

	//Stops the velocity going beyond our maximum speed
	_velocity.Truncate(_maxSpeed);

	//Adds the velocity to our current position
	_currentPosition += _velocity * deltaTime;

	//Calculates our current speed (used in steering behaviors) 
	_currentSpeed = _velocity.Length();

	// rotate the object based on its last & current position
	Vector2D diff = _currentPosition - _lastPosition;
	if (diff.Length() > 0) { // if zero then don't update rotation
		diff.Normalize();
		m_targetRotation = atan2f(diff.y, diff.x); // this is used by DrawableGameObject to set the rotation
	}

	_lastPosition = _currentPosition;

	//Calculate the forward vector
	_forward = Vector2D(cosf(m_radianRotation), sinf(m_radianRotation));

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
	ImGui::Text("Current Position: %f, %f", _currentPosition.x, _currentPosition.y);
	ImGui::Text("Radian Rotation: %f", m_radianRotation);
	ImGui::Text("Target Rotation: %f", m_targetRotation);
	ImGui::Text("Forward Vector: %f, %f", _forward.x, _forward.y);
	ImGui::Text("Wander Target: %f, %f", _wanderTarget.x, _wanderTarget.y);

	if (_isSpeedBoostActive) {
		ImGui::Text("Speed boost active, time left: %f", _speedBoostTimer);
	}
	else
	{
		ImGui::Text("Speed boost inactive");
	}

	if (_isCollisionPenaltyActive) {
		ImGui::Text("Collision Penalty active, time left: %f", _collisionPenaltyTimer);
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
	_steerSpeedFactor = _collisionSpeedFactor;

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
		//TODO Add in pickup to vehicle AI system


		break;
	case PICKUP_COLLECTED:
		_isSpeedBoostActive = true;
		_speedBoostTimer = _speedBoostDuration;
		_steerSpeedFactor = _pickupSpeedFactor;
		;
		break;
	default:
		break;
	}
}
