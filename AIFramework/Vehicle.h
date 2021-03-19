#ifndef VEHICHLE_H
#define VEHICHLE_H

#include "DrawableGameObject.h"
#include "Imgui/imgui.h"
#include "Vector2D.h"



class VehicleFSM;
class Steering;

class Vehicle : public DrawableGameObject
{
	Steering* pSteering;
public:
	Vehicle(ID3D11Device* device, std::wstring textureName);
	void InitializeStates();

	virtual void update(const float deltaTime);

	void DrawUI();

	void setMaxSpeed(const float maxSpeed);
	void setCurrentSpeed(const float speed); // a ratio: a value between 0 and 1 (1 being max speed)
	void setPositionTo(Vector2D positionTo); // a position to move to
	void setVehiclePosition(Vector2D position); // the current position - this resets positionTo
	float GetMaxSpeed();
	Vector2D GetVelocity();

	float GetCurrentSpeed() const;

	void SetSteeringTarget(Vector2D pos);

	Steering* GetSteering();

	Vector2D GetForward() const;

	void SetWaypoints(std::vector<Waypoint*> waypoints);
	Waypoint* GetWaypoint(const int x, const int y);

	Vector2D GetWanderTarget() const {
		return _wanderTarget;
	}

	void SetWanderTarget(Vector2D val) {
		_wanderTarget = val;
	}

	Vector2D GetSide() const {
		return _side;
	}

	bool GetActive() const {
		return _isActive;
	}

	void SetActive(bool val = true) {
		_isActive = val;
	}

protected:
	float m_maxSpeed;
	float m_currentSpeed;
	float _mass = 1.0f;


	bool _isSteering = false;
	bool _isPathfinding = true;
	bool _isDecisionMaking = false;


	//Steering Settings
	bool _isSeeking = false;
	bool _isFleeing = false;
	bool _isArriving = false;
	bool _isPursuing = false;
	bool _isAvoiding = false;
	bool _isWandering = false;

	bool _isActive = true;

	Vector2D m_currentPosition;
	Vector2D m_startPosition;
	Vector2D m_positionTo;
	Vector2D m_lastPosition;
	Vector2D _velocity;

	Vector2D _wanderTarget;

	Vector2D _forward;

	Vector2D _side;



	VehicleFSM* pFSM = nullptr;

	vector<Waypoint*> _waypoints;

	Vehicle* _pOtherVehicle;

public:
	Vector2D GetTarget();

	void SetOtherVehicle(Vehicle* vehicle) {
		_pOtherVehicle = vehicle;
	}

	Vehicle* GetOtherVehicle() const {
		return _pOtherVehicle;
	}
private:
	void DrawSteeringOptions();
	void DisableBehaviors();
	void DrawPathfindingOptions();
	void DrawDecisionMakingOptions();
};

#endif // !VEHICHLE_H




