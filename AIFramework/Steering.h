
#ifndef STEERING_H
#define STEERING_H
#pragma warning (disable:4786)

#include "Vector2D.h"


class Vehicle;


class Steering
{
public:
	Steering(Vehicle* owner);

	Vector2D CalculateForce();

	Vector2D Seek(Vector2D target);
	void SeekOn();
	void SeekOff();

	Vector2D Arrive(Vector2D target);
	void ArriveOn();
	void ArriveOff();

	Vector2D Flee(Vector2D target);
	void FleeOn();
	void FleeOff();

	Vector2D Pursuit(Vehicle* target);
	void PursuitOn();
	void PursuitOff();

	Vector2D Wander();
	void WanderOn();
	void WanderOff();


	void ObstacleAvoidanceOn();
	void ObstacleAvoidanceOff();
	Vector2D ObstacleAvoidance();

	void ClearFlags();

	void NewWanderTarget();

	bool IsWantingToOvertake() const {
		return _wantingToOvertake;
	}

private:
	enum BehaviorType {
		none = 0x00000,
		seek = 0x00002,
		flee = 0x00004,
		arrive = 0x00008,
		wander = 0x00010,
		obstacle_avoidance = 0x00020,
		pursuit = 0x00040
	};




	int _flags;

	Vehicle* pOwner;

	Vector2D _steeringForce;

	Vector2D _targetPosition;

private:
	bool IsOn(BehaviorType bt);

	bool _wantingToOvertake = false;
};

#endif
