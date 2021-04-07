#ifndef VEHICHLE_H
#define VEHICHLE_H

#include "DrawableGameObject.h"
#include "Imgui/imgui.h"
#include "Vector2D.h"
#include "Observer.h"

//forward declare our vehicle state machine and steering class
class VehicleFSM;
class Steering;

//the vehicle class is both a DrawableGameObject and a Observer as we need to be seen on screen and observe commands from the pickup
class Vehicle : public DrawableGameObject, public Observer
{
	Steering* pSteering;
public:
	Vehicle(ID3D11Device* device, std::wstring textureName, Vector2D startPos, std::vector<Waypoint*> waypoints, float maxSpeed, std::string name);
	void InitializeStates();

	virtual void update(const float deltaTime);

	void DrawUI();

	//Activates the collision penalty for colliding with another car
	void ActivateCollisionPenalty();

	//Resets the needed data in our vehicle such as position and velocity
	void ResetVehicle();

	//Resets our current state
	void ResetState();

	//Handle memory deallocation
	~Vehicle();

#pragma region Getters
	//Gets our side vector
	Vector2D GetSide() const {
		return _side;
	}
	//Gets if this vehicle is active
	bool GetActive() const {
		return _isActive;
	}

	/// <summary>
	/// Finds a waypoint given it's x and y tile positions
	/// </summary>
	/// <param name="x">X tile postiton</param>
	/// <param name="y">Y tile position</param>
	/// <returns></returns>
	Waypoint* GetWaypoint(const int x, const int y)
	{
		if (x < 0 || y < 0) return nullptr;

		if (x >= 20 || y >= 20) return nullptr;

		assert(x >= 0 && x < 20);
		assert(y >= 0 && y < 20);

		return _waypoints[y * 20 + x];
	}

	/// <summary>
	/// Gets a waypoint based on a vector2D
	/// </summary>
	/// <param name="vec">vector2D for the waypoints tile position</param>
	/// <returns></returns>
	Waypoint* GetWaypoint(const Vector2D vec) {
		return GetWaypoint(vec.x, vec.y);
	}

	//Returns our wander target
	Vector2D GetWanderTarget() const {
		return _wanderTarget;
	}

	//returns a pointer to the vehicles steering behavior
	Steering* GetSteering()
	{
		return pSteering;
	}
	//returns the vehicles forward vector
	Vector2D GetForward() const
	{
		return _forward;
	}
	//returns the vehicles max speed
	float GetMaxSpeed() const
	{
		return _maxSpeed;
	}
	//returns the vehicles velocity
	Vector2D GetVelocity() const
	{
		return _velocity;
	}
	//returns the vehicles current speed
	float GetCurrentSpeed() const
	{
		return _currentSpeed;
	}

	//returns the vehicles name
	std::string GetName() const {
		return _vehicleName;
	}

	//returns the vehicles target
	Vector2D GetTarget()
	{
		return _positionTo;
	}
	//returns a pointer to the vehicles state machine
	VehicleFSM* GetStateMachine() const {
		return pFSM;
	}
	//gets a pointer to the pickup item
	PickupItem* GetPickup() const {
		return _pPickup;
	}

	//Returns the vehicles starting position
	Vector2D GetStartPosition() const
	{
		return _startingPosition;
	}
#pragma endregion

#pragma region Setters
	//sets if the vehicle is active, defaults to true
	void SetActive(bool val = true) {
		_isActive = val;
	}

	//Sets the vehicles wander target
	void SetWanderTarget(Vector2D val) {
		_wanderTarget = val;
	}

	//sets the vehicles waypoints
	void SetWaypoints(std::vector<Waypoint*> waypoints)
	{
		_waypoints = waypoints;
	}

	//sets the vehicles steering target
	void SetSteeringTarget(Vector2D pos);

	//sets the vehicles max speed
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
	//sets the other vehicle on the track
	void SetOtherVehicle(Vehicle* vehicle) {
		_pOtherVehicle = vehicle;
	}

	//returns a pointer to the other vehicle
	Vehicle* GetOtherVehicle() const {
		return _pOtherVehicle;
	}

	//resets our position and velocity
	void Reset() {
		_currentPosition = _startingPosition;
		_velocity = Vector2D(0, 0);
	}

	//sets our pickup item
	void SetPickup(PickupItem* pickup) 
	{
		_pPickup = pickup;
	}


#pragma endregion
	//Overrides the OnNotify method from the Observer class
	void OnNotify(Event event) override;

protected:
	//Holds the vehicle's max and current speed
	float _maxSpeed;
	float _currentSpeed;

	//Holds the mass of the vehicle 
	float _mass = 1.0f;

	//Holds the vehicles current speed factors
	float _defaultSpeedFactor = 1.0f;
	float _pickupSpeedFactor = 1.1f;
	float _collisionSpeedFactor = 0.5f;
	float _currentSpeedFactor = _defaultSpeedFactor;

	//Holds the vehicles collision penalties
	float _collisionPenaltyDuration = 3.0f;
	float _collisionPenaltyTimer = _collisionPenaltyDuration;
	bool _isCollisionPenaltyActive = false;

	//Holds the speed boost data
	float _speedBoostDuration = 3.0f;
	float _speedBoostTimer = _speedBoostDuration;
	bool _isSpeedBoostActive = false;

	//sets the vehicle to active by default
	bool _isActive = true;

	//Holds positional data
	Vector2D _currentPosition;
	Vector2D _startPosition;
	Vector2D _positionTo;
	Vector2D _lastPosition;
	Vector2D _startingPosition;

	//holds the vehicles velocity
	Vector2D _velocity;

	//Holds the vehicles wander target
	Vector2D _wanderTarget;

	//Holds the vehicles forward vector
	Vector2D _forward;

	//Holds the vehicles side vector
	Vector2D _side;

	//Holds the vehicles state machine
	VehicleFSM* pFSM = nullptr;

	//Holds the waypoints in the map
	vector<Waypoint*> _waypoints;

	//Holds a pointer to the other vehicle
	Vehicle* _pOtherVehicle;

	//Holds the vehicles name
	std::string _vehicleName;

	//holds a pointer to the pickup
	PickupItem* _pPickup;
};

#endif // !VEHICHLE_H