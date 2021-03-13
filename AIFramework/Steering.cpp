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
		_steeringForce += Pursuit(pOwner->GetOtherVehicle());
	}
	if (IsOn(wander)) {
		_steeringForce += Wander();
	}
	if (IsOn(obstacle_avoidance)) {
		_steeringForce += ObstacleAvoidance() * 10.0;
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

	double relativeForward = pOwner->GetForward().Dot(target->GetForward());

	//if the target is in front of us then we can just seek to them
	if ((directionToTarget.Dot(pOwner->GetForward()) > 0) && (relativeForward < -0.95)) {
		return Seek(target->GetVectorPosition());
	}

	//the target is not infront of us so we have to predict where it will be

	double lookAheadTime = directionToTarget.Length() / (pOwner->GetMaxSpeed() + target->GetCurrentSpeed());

	return Seek(target->GetVectorPosition() + target->GetVelocity() * lookAheadTime);
}

Vector2D Steering::Wander()
{
	if (pOwner->GetVectorPosition().Distance(pOwner->GetWanderTarget()) < 3.5f) {
		NewWanderTarget();
	}


	return Seek(pOwner->GetWanderTarget());
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

Vector2D Steering::ObstacleAvoidance()
{
	Vector2D localPos = pOwner->GetOtherVehicle()->GetVectorPosition() - pOwner->GetVectorPosition();
	//localPos.x = -localPos.x;
	//localPos.y = -localPos.y;

	//C2DMatrix matTransform;

	//double Tx = -AgentPosition.Dot(AgentHeading);
	//double Ty = -AgentPosition.Dot(AgentSide);

	////create the transformation matrix
	//matTransform._11(AgentHeading.x); matTransform._12(AgentSide.x);
	//matTransform._21(AgentHeading.y); matTransform._22(AgentSide.y);
	//matTransform._31(Tx);           matTransform._32(Ty);


	Vehicle* possibleCollision = nullptr;

	float mininumBoxLength = 30.0f + (pOwner->GetCurrentSpeed() / pOwner->GetMaxSpeed()) * 30.0f;
	mininumBoxLength *= 1.5f;

	//Based on scale of cars
	

	double aRadius = 45.0;
	double bRadius = 45.0;

	double range = aRadius + bRadius;

	//Target is actually in front of us
	if (localPos.x >= 0.0f) {

		if (localPos.LengthSq() < range * range) {

			double radius = aRadius + bRadius;

			//possible collision
			if (fabsf(localPos.y) < radius) {
				double cX = localPos.x;
				double cY = localPos.y;

				double sqrPart = sqrt(radius * radius - cY * cY);

				double ip = cX - sqrPart;

				if (ip <= 0.0) {
					ip = cX + sqrPart;
				}

				possibleCollision = pOwner->GetOtherVehicle();
			}
		}

	}

	Vector2D steeringForce = Vector2D();

	if (possibleCollision != nullptr) {
		double multiplier = 1.0 + (mininumBoxLength - localPos.x) / mininumBoxLength;

		steeringForce.y = (bRadius - localPos.y) * multiplier;

		double breakPower = 0.2;

		steeringForce.x = (bRadius - localPos.x) * breakPower;
	}


	//Vec: steering force
	//heading: pOwner forward
	//side: pOwner Side

	XMFLOAT3X3 transform;
	
	Vector2D vec = steeringForce;
	Vector2D forward = pOwner->GetForward();
	Vector2D side = pOwner->GetSide();

	transform._11 = forward.x;
	transform._12 = forward.y;
	transform._13 = 0;

	transform._21 = side.x;
	transform._22 = side.y;
	transform._23 = 0;

	transform._31 = 0;
	transform._32 = 0;
	transform._33 = 1;

	double tempX = (transform._11 * vec.x) + (transform._21 * vec.y) + (transform._31);
	double tempY = (transform._12 * vec.x) + (transform._22 * vec.y) + (transform._32);
	
	vec.x = tempX;
	vec.y = tempY;

	return vec;
}

void Steering::ClearFlags()
{
	_flags = 0;
}

bool Steering::IsOn(BehaviorType bt)
{
	return (_flags & bt) == bt;
}

void Steering::NewWanderTarget()
{
	Vector2D randomTarget = Vector2D(rand() % 1024, rand() % 768);

	randomTarget.x -= 1024 / 2;
	randomTarget.y -= 768 / 2;

	pOwner->SetWanderTarget(randomTarget);
}
