#include "Vehicle.h"

#include "Imgui/imgui.h"

#include "Steering.h"

#define MAX_SPEED 250

Vehicle::Vehicle(ID3D11Device* device, std::wstring textureName)
{
	m_scale = XMFLOAT3(30, 20, 1);
	setMaxSpeed(MAX_SPEED);
	m_currentSpeed = m_maxSpeed;

	setVehiclePosition(Vector2D(74.5, 205));
	

	setTextureName(textureName);
	DrawableGameObject::initMesh(device);

	pSteering = new Steering(this);
}

void Vehicle::update(const float deltaTime)
{
	Vector2D steeringForce;
	steeringForce = pSteering->CalculateForce();

	Vector2D acceleration = steeringForce / _mass;

	_velocity += acceleration * deltaTime;
	_velocity.Truncate(m_maxSpeed);

	m_currentPosition += _velocity * deltaTime;





	// rotate the object based on its last & current position
	Vector2D diff = m_currentPosition - m_lastPosition;
	if (diff.Length() > 0) { // if zero then don't update rotation
		diff.Normalize();
		m_targetRotation = atan2f(diff.y, diff.x); // this is used by DrawableGameObject to set the rotation
	}
	m_lastPosition = m_currentPosition;

	// set the current position for the drawable gameobject
	setPosition(XMFLOAT3((float)m_currentPosition.x, (float)m_currentPosition.y, 0));

	DrawableGameObject::update(deltaTime);

	ImGui::Begin("Car Information");
	ImGui::Text("Radian Rotation: %f", m_radianRotation);
	ImGui::Text("Target Rotation: %f", m_targetRotation);
	ImGui::End();

}

void Vehicle::setMaxSpeed(const float maxSpeed)
{
	m_maxSpeed = maxSpeed;
}

// a ratio: a value between 0 and 1 (1 being max speed)
void Vehicle::setCurrentSpeed(const float speed)
{
	m_currentSpeed = m_maxSpeed * speed;
	m_currentSpeed = max(0, m_currentSpeed);
	m_currentSpeed = min(1, m_currentSpeed);
}

// a position to move to
void Vehicle::setPositionTo(Vector2D position)
{
	m_startPosition = m_currentPosition;
	m_positionTo = position;
}

// the current position
void Vehicle::setVehiclePosition(Vector2D position)
{
	m_currentPosition = position;
	m_positionTo = position;
	m_startPosition = position;
	m_lastPosition = position;
	setPosition(XMFLOAT3((float)position.x, (float)position.y, 0));
}

float Vehicle::GetMaxSpeed()
{
	return m_maxSpeed;
}

Vector2D Vehicle::GetVelocity()
{
	return _velocity;
}

Steering* Vehicle::GetSteering()
{
	return pSteering;
}

Vector2D Vehicle::GetTarget()
{
	return m_positionTo;
}
