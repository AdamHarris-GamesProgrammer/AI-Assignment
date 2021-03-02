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

	Steering* GetSteering();

	void SetWaypoints(std::vector<Waypoint*> waypoints);
	Waypoint* GetWaypoint(const int x, const int y);

protected:
	float m_maxSpeed;
	float m_currentSpeed;
	float _mass = 1.0f;

	Vector2D m_currentPosition;
	Vector2D m_startPosition;
	Vector2D m_positionTo;
	Vector2D m_lastPosition;
	Vector2D _velocity;

	VehicleFSM* pFSM = nullptr;

	vector<Waypoint*> _waypoints;



public:
	Vector2D GetTarget();
};

#endif // !VEHICHLE_H




