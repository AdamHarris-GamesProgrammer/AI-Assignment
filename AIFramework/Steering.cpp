#include "Steering.h"
#include "Vehicle.h"

Steering::Steering(Vehicle* owner) :_flags(0), pOwner(owner) {}

Vector2D Steering::CalculateForce()
{
	_steeringForce.Zero();

	if (IsOn(seek))
	{
		_steeringForce += Seek(pOwner->GetTarget());
	}
	if (IsOn(arrive)) {
		_steeringForce += Arrive(pOwner->GetTarget());
	}
	if (IsOn(flee)) {
		_steeringForce += Flee(pOwner->GetTarget()) * 15.0f;
	}
	if (IsOn(pursuit)) {

	}
	if (IsOn(wander)) {
		_steeringForce += Wander();
	}
	if (IsOn(obstacle_avoidance)) {

	}



	_steeringForce.Truncate(pOwner->GetMaxSpeed());

	return _steeringForce;
}

Vector2D Steering::Seek(Vector2D target)
{
	Vector2D desiredVelocity = Vec2DNormalize(target - pOwner->GetVectorPosition()) * pOwner->GetMaxSpeed();

	return (desiredVelocity - pOwner->GetVelocity());
}

void Steering::SeekOn()
{
	_flags |= seek;
}

void Steering::SeekOff()
{
	if (IsOn(seek)) _flags ^= seek;
}

Vector2D Steering::Arrive(Vector2D target)
{
	Vector2D direction = target - pOwner->GetVectorPosition();

	double distance = direction.Length();

	if (distance > 20.0) {
		double speed = distance / 2;
		speed = min(speed, pOwner->GetMaxSpeed());


		Vector2D desiredVelocity = direction * speed / distance;

		return (desiredVelocity - pOwner->GetVelocity());
	}

	return Vector2D(0, 0);
}

void Steering::ArriveOn()
{
	_flags |= arrive;
}

void Steering::ArriveOff()
{
	if (IsOn(arrive)) _flags ^= arrive;
}

Vector2D Steering::Flee(Vector2D target)
{
	Vector2D desiredVelocity = Vec2DNormalize(pOwner->GetVectorPosition() - target) * pOwner->GetMaxSpeed();

	return (desiredVelocity - pOwner->GetVelocity());
}

void Steering::FleeOn()
{
	_flags |= flee;
}

void Steering::FleeOff()
{
	if (IsOn(flee)) _flags ^= flee;
}

Vector2D Steering::Pursuit(Vehicle* target)
{
	Vector2D directionToTarget = target->GetVectorPosition() - pOwner->GetVectorPosition();

	//TODO: Create a transform forward system
	return Vector2D(0, 0);
}

Vector2D Steering::Wander()
{

	return Vector2D(0, 0);
}

void Steering::WanderOn()
{
	_flags |= wander;
}

void Steering::WanderOff()
{
	if (IsOn(wander)) _flags ^= wander;
}

void Steering::PursuitOn()
{
	_flags |= pursuit;
}

void Steering::PursuitOff()
{
	if (IsOn(pursuit)) _flags ^= pursuit;
}

void Steering::ObstacleAvoidanceOn()
{
	_flags |= obstacle_avoidance;
}

void Steering::ObstacleAvoidanceOff()
{
	if (IsOn(obstacle_avoidance)) _flags ^= obstacle_avoidance;
}

void Steering::ClearFlags()
{
	_flags = 0;
}

bool Steering::IsOn(BehaviorType bt)
{
	return (_flags & bt) == bt;
}
