#ifndef VEHICHLE_H
#define VEHICHLE_H

#include "DrawableGameObject.h"
#include "Imgui/imgui.h"
#include "Vector2D.h"
#include "Observer.h"



class VehicleFSM;
class Steering;

class Vehicle : public DrawableGameObject, public Observer
{
	Steering* pSteering;
public:
	Vehicle(ID3D11Device* device, std::wstring textureName, Vector2D startPos, std::vector<Waypoint*> waypoints, float maxSpeed);
	void InitializeStates();

	virtual void update(const float deltaTime);

	void DrawUI();

	void ActivateCollisionPenalty();

#pragma region Getters
	Vector2D GetSide() const {
		return _side;
	}
	bool GetActive() const {
		return _isActive;
	}
	Waypoint* GetWaypoint(const int x, const int y)
	{
		if (x < 0 || y < 0) return nullptr;

		if (x >= 20 || y >= 20) return nullptr;

		assert(x >= 0 && x < 20);
		assert(y >= 0 && y < 20);

		return _waypoints[y * 20 + x];
	}
	Waypoint* GetWaypoint(const Vector2D vec) {
		return GetWaypoint(vec.x, vec.y);
	}
	Vector2D GetWanderTarget() const {
		return _wanderTarget;
	}
	Steering* GetSteering()
	{
		return pSteering;
	}
	Vector2D GetForward() const
	{
		return _forward;
	}
	float GetMaxSpeed() const
	{
		return _maxSpeed;
	}
	Vector2D GetVelocity() const
	{
		return _velocity;
	}
	float GetCurrentSpeed() const
	{
		return _currentSpeed;
	}
	Vector2D GetTarget()
	{
		return _positionTo;
	}
#pragma endregion

#pragma region Setters
	void SetActive(bool val = true) {
		_isActive = val;
	}
	void SetWanderTarget(Vector2D val) {
		_wanderTarget = val;
	}
	void SetWaypoints(std::vector<Waypoint*> waypoints)
	{
		_waypoints = waypoints;
	}
	void SetSteeringTarget(Vector2D pos);
	void SetMaxSpeed(const float maxSpeed)
	{
		_maxSpeed = maxSpeed;
	}
	// a ratio: a value between 0 and 1 (1 being max speed)
	void SetCurrentSpeed(const float speed)
	{
		_currentSpeed = _maxSpeed * speed;
		_currentSpeed = max(0, _currentSpeed);
		_currentSpeed = min(1, _currentSpeed);
	} // a ratio: a value between 0 and 1 (1 being max speed)
											 // a position to move to
	void SetPositionTo(Vector2D positionTo)
	{
		_startPosition = _currentPosition;
		_positionTo = positionTo;

	} // a position to move to

	// the current position
	void SetVehiclePosition(Vector2D position) // the current position - this resets positionTo
	{
		_currentPosition = position;
		_positionTo = position;
		_startPosition = position;
		_lastPosition = position;
		setPosition(XMFLOAT3((float)position.x, (float)position.y, 0));
	}
	void SetOtherVehicle(Vehicle* vehicle) {
		_pOtherVehicle = vehicle;
	}
	Vehicle* GetOtherVehicle() const {
		return _pOtherVehicle;
	}
#pragma endregion


	void OnNotify(const Entity& entity, Event event) override;


	void OnNotify(Event event) override;

protected:
	float _maxSpeed;
	float _currentSpeed;
	float _mass = 1.0f;

	float _steerSpeedFactor = 1.0f;
	float _pickupSpeedFactor = 4.5f;
	float _collisionSpeedFactor = 0.1f;

	float _currentSpeedFactor = _steerSpeedFactor;

	float _collisionPenaltyDuration = 3.0f;
	float _collisionPenaltyTimer = _collisionPenaltyDuration;
	bool _isCollisionPenaltyActive = false;

	float _speedBoostDuration = 3.0f;
	float _speedBoostTimer = _speedBoostDuration;
	bool _isSpeedBoostActive = false;

	bool _isActive = true;

	Vector2D _currentPosition;
	Vector2D _startPosition;
	Vector2D _positionTo;
	Vector2D _lastPosition;
	Vector2D _velocity;

	Vector2D _wanderTarget;

	Vector2D _forward;

	Vector2D _side;

	VehicleFSM* pFSM = nullptr;

	vector<Waypoint*> _waypoints;

	Vehicle* _pOtherVehicle;
};

#endif // !VEHICHLE_H