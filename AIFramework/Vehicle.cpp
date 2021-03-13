#include "Vehicle.h"

#include "Steering.h"

#include "VehicleFSM.h"

#define MAX_SPEED 150

Vehicle::Vehicle(ID3D11Device* device, std::wstring textureName)
{
	m_scale = XMFLOAT3(30, 20, 1);
	setMaxSpeed(MAX_SPEED);
	m_currentSpeed = m_maxSpeed;

	setVehiclePosition(Vector2D(74.5, 205));


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


	//Calculate the forward vector
	_forward = Vector2D(cosf(m_radianRotation), sinf(m_radianRotation));

	//Calculate the right vector
	_side = _forward.Perp();

	// set the current position for the drawable gameobject
	setPosition(XMFLOAT3((float)m_currentPosition.x, (float)m_currentPosition.y, 0));

	DrawableGameObject::update(deltaTime);
}

void Vehicle::DrawUI()
{
	ImGui::Begin("Car Information");
	ImGui::Text("Car Details");
	ImGui::Text("Current Position: %f, %f", m_currentPosition.x, m_currentPosition.y);
	ImGui::Text("Radian Rotation: %f", m_radianRotation);
	ImGui::Text("Target Rotation: %f", m_targetRotation);
	ImGui::Text("Forward Vector: %f, %f", _forward.x, _forward.y);
	ImGui::Text("Wander Target: %f, %f", _wanderTarget.x, _wanderTarget.y);
	ImGui::End();


	//Checks to see if the user is pressing any of the radio buttons
	ImGui::Begin("AI Control Panel");
	if (ImGui::RadioButton("Steering", _isSteering)) {
		_isSteering = true;
		_isPathfinding = false;
		_isDecisionMaking = false;
		_isSeeking = false;
		_isFleeing = false;
		_isArriving = false;
		_isPursuing = false;
		_isAvoiding = false;
		_isWandering = false;
		pFSM->Section1AI();
	}
	else if (ImGui::RadioButton("Pathfinding", _isPathfinding)) {
		_isPathfinding = true;
		_isSteering = false;
		_isDecisionMaking = false;
		_isSeeking = false;
		_isFleeing = false;
		_isArriving = false;
		_isPursuing = false;
		_isAvoiding = false;
		_isWandering = false;
		pFSM->Section2AI();
	}
	else if (ImGui::RadioButton("Decision Making", _isDecisionMaking)) {
		_isDecisionMaking = true;
		_isSteering = false;
		_isPathfinding = false;
		_isSeeking = false;
		_isFleeing = false;
		_isArriving = false;
		_isPursuing = false;
		_isAvoiding = false;
		_isWandering = false;
		pFSM->Section3AI();
	}

	if (_isSteering) {
		DrawSteeringOptions();

	}
	else if (_isPathfinding) {
		DrawPathfindingOptions();
	}
	else if (_isDecisionMaking) {
		DrawDecisionMakingOptions();
	}
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

float Vehicle::GetCurrentSpeed() const
{
	return m_currentSpeed;
}

void Vehicle::SetSteeringTarget(Vector2D pos)
{
	if (pFSM->GetSection() == VehicleFSM::steering) {
		setPositionTo(pos);
	}
}

Steering* Vehicle::GetSteering()
{
	return pSteering;
}

Vector2D Vehicle::GetForward() const
{
	return _forward;
}

void Vehicle::SetWaypoints(std::vector<Waypoint*> waypoints)
{
	_waypoints = waypoints;
}

Waypoint* Vehicle::GetWaypoint(const int x, const int y)
{
	if (x < 0 || y < 0) return nullptr;

	if (x >= 20 || y >= 20) return nullptr;

	assert(x >= 0 && x < 20);
	assert(y >= 0 && y < 20);

	return _waypoints[y * 20 + x];
}

Vector2D Vehicle::GetTarget()
{
	return m_positionTo;
}

void Vehicle::DrawSteeringOptions()
{
	ImGui::Text("Steering Options");
	if (ImGui::RadioButton("Seek", _isSeeking)) {
		_isSeeking = true;
		_isFleeing = false;
		_isArriving = false;
		_isPursuing = false;
		_isAvoiding = false;
		_isWandering = false;
		pFSM->SeekOn();
	}
	else if (ImGui::RadioButton("Flee", _isFleeing)) {
		_isSeeking = false;
		_isFleeing = true;
		_isArriving = false;
		_isPursuing = false;
		_isAvoiding = false;
		_isWandering = false;
		pFSM->FleeOn();
	}
	else if (ImGui::RadioButton("Arrive", _isArriving)) {
		_isSeeking = false;
		_isFleeing = false;
		_isArriving = true;
		_isPursuing = false;
		_isAvoiding = false;
		_isWandering = false;
		pFSM->ArriveOn();
	}
	else if (ImGui::RadioButton("Pursuit", _isPursuing)) {
		_isSeeking = false;
		_isFleeing = false;
		_isArriving = false;
		_isPursuing = true;
		_isAvoiding = false;
		_isWandering = false;
		pFSM->PursuitOn();
	}
	else if (ImGui::RadioButton("Obstacle Avoidance", _isAvoiding)) {
		_isSeeking = false;
		_isFleeing = false;
		_isArriving = false;
		_isPursuing = false;
		_isAvoiding = true;
		_isWandering = false;
		pFSM->ObstacleAvoidanceOn();
	}
	else if (ImGui::RadioButton("Wandering", _isWandering)) {
		_isSeeking = false;
		_isFleeing = false;
		_isArriving = false;
		_isPursuing = false;
		_isAvoiding = false;
		_isWandering = true;
		pSteering->NewWanderTarget();
		pFSM->WanderingOn();
	}
}

void Vehicle::DrawPathfindingOptions()
{

}

void Vehicle::DrawDecisionMakingOptions()
{
	
}
