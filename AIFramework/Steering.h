
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
	void SeekOn();;
	void SeekOff();

private:
	enum BehaviorType {
		none = 0x00000,
		seek = 0x00002
	};




	int _flags;

	Vehicle* pOwner;

	Vector2D _steeringForce;

	Vector2D _targetPosition;

private:
	bool IsOn(BehaviorType bt);

};

#endif
