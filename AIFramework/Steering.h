
#ifndef STEERING_H
#define STEERING_H
#pragma warning (disable:4786)

#include "Vector2D.h"


class Vehicle;


class Steering
{
public:
	//Sets the owner of this steering behavior
	Steering(Vehicle* owner);

	//Calculates the steering force based on our current enabled behaviors
	Vector2D CalculateForce();

	//Handles the seeking logic
	Vector2D Seek(Vector2D target);
	void SeekOn();
	void SeekOff();

	//Handles the arriving logic
	Vector2D Arrive(Vector2D target);
	void ArriveOn();
	void ArriveOff();

	//Handles the fleeing logic
	Vector2D Flee(Vector2D target);
	void FleeOn();
	void FleeOff();

	//Handles the pursuing logic
	Vector2D Pursuit(Vehicle* target);
	void PursuitOn();
	void PursuitOff();

	//Handles the wandering logic
	Vector2D Wander();
	void WanderOn();
	void WanderOff();

	//Handles the obstacle avoidance logic
	void ObstacleAvoidanceOn();
	void ObstacleAvoidanceOff();
	Vector2D ObstacleAvoidance();

	//clear the behavior flags
	void ClearFlags();

	//generates a new wander target which is on screen
	void NewWanderTarget();

	//returns if we want to overtake, this is decided in the ObstacleAvoidance logic
	bool IsWantingToOvertake() const {
		return _wantingToOvertake;
	}

private:
	//Holds all the steering behaviors we have, uses bit flags
	enum BehaviorType {
		none = 0x00000,
		seek = 0x00002,
		flee = 0x00004,
		arrive = 0x00008,
		wander = 0x00010,
		obstacle_avoidance = 0x00020,
		pursuit = 0x00040
	};



	//holds our current flags
	int _flags;

	//Holds the vehicle that owns this steering behavior 
	Vehicle* pOwner;

	Vector2D _steeringForce;

	Vector2D _targetPosition;

private:
	/// <summary>
	/// Checks if a certain behavior type is on or off
	/// </summary>
	/// <param name="bt"></param>
	/// <returns></returns>
	bool IsOn(BehaviorType bt);

	bool _wantingToOvertake = false;
};

#endif
