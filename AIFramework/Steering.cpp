#include "Steering.h"
#include "Vehicle.h"

Steering::Steering(Vehicle* owner) :_flags(0), pOwner(owner) {}

Vector2D Steering::CalculateForce()
{
	//zeroes out the steering force
	_steeringForce.Zero();

	//accumulates the force of all the steering behaviors
	if (IsOn(seek))
	{
		_steeringForce += Seek(pOwner->GetTarget()) * 5.0f;
	}
	if (IsOn(arrive)) {
		_steeringForce += Arrive(pOwner->GetTarget()) * 2.0f;
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
		_steeringForce += ObstacleAvoidance() * 15.0f;
	}

	//limits our steering force based on the owners max speed
	_steeringForce.Truncate(pOwner->GetMaxSpeed());

	//Returns the steering force multiplied by a final weighting
	return _steeringForce * 2.0;
}

Vector2D Steering::Seek(Vector2D target)
{
	//seeking is a simple behaviour that just has the vehicle move to a point and not stop, resulting the owner going back and forth over a point
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
	//calculates the direction from the target to the owner
	Vector2D direction = target - pOwner->GetVectorPosition();

	//gets the distance
	double distance = direction.Length();

	//if our distance is greater than 0 we will need to generate a force
	if (distance > 0.0) {
		//calculate the speed we need to go based on the distance
		double speed = distance / 2;
		speed = min(speed, pOwner->GetMaxSpeed());

		//calculates our desired velocity based on the speed divided by the distance
		Vector2D desiredVelocity = direction * speed / distance;

		//removes the owners current velocity from this velocity
		return (desiredVelocity - pOwner->GetVelocity());
	}

	//if not then we return no force
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
	//Fleeing works liek the opposite of seeking so we just move away from our target
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
	//calculates the direction to our target
	Vector2D directionToTarget = target->GetVectorPosition() - pOwner->GetVectorPosition();

	double relativeForward = pOwner->GetForward().Dot(target->GetForward());

	//if the target is in front of us then we can just seek to them
	if ((directionToTarget.Dot(pOwner->GetForward()) > 0) && (relativeForward < -0.95)) {
		return Seek(target->GetVectorPosition());
	}

	//the target is not infront of us so we have to predict where it will be
	double lookAheadTime = directionToTarget.Length() / (pOwner->GetMaxSpeed() + target->GetCurrentSpeed());

	//seek to the predicted position of the target
	return Seek(target->GetVectorPosition() + target->GetVelocity() * lookAheadTime);
}

Vector2D Steering::Wander()
{
	//Wandering involves seeking to a random position and then generating a new wander target when we get close enough.
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
	//Obstalce avoidance involves converting the obstacles into local position space from the owner, finding the closest obstacle and then calculating a force to avoid it
	XMFLOAT3X3 matTransform;

	//Gets the dot product of the owners forward and side vectors
	double Tx = -pOwner->GetVectorPosition().Dot(pOwner->GetForward());
	double Ty = -pOwner->GetVectorPosition().Dot(pOwner->GetSide());

	//The below code converts the other vehicle into local position space

	//create the transformation matrix
	matTransform._11= pOwner->GetForward().x;
	matTransform._12 = pOwner->GetSide().x;
	matTransform._21= pOwner->GetForward().y;
	matTransform._22= pOwner->GetSide().y;
	matTransform._31= Tx;      
	matTransform._32= Ty;


	//Gets the position of the other vehicle (our obstacle)
	Vector2D otherPos = pOwner->GetOtherVehicle()->GetVectorPosition();

	double tempX = (matTransform._11 * otherPos.x) + (matTransform._21 * otherPos.y) + (matTransform._31);
	double tempY = (matTransform._12 * otherPos.x) + (matTransform._22 * otherPos.y) + (matTransform._32);

	otherPos.x = tempX;
	otherPos.y = tempY;

	//set the local position to the new other position
	Vector2D localPos = otherPos;

	//Holds a pointer to the vehicle if it is a possible collision
	Vehicle* possibleCollision = nullptr;

	//Calculates the length of our avoidance box. A box in front of the owner which is used to check for obstalces 
	float mininumBoxLength = 30.0f + (pOwner->GetCurrentSpeed() / pOwner->GetMaxSpeed()) * 30.0f;

	//Based on scale of cars
	double aRadius = 30.0;
	double bRadius = 30.0;

	//Creates the range between the two vehicles
	double range = aRadius + bRadius;

	//sets wanting to overtake to false
	_wantingToOvertake = false;

	//Target is actually in front of us
	if (localPos.x >= 0.0f) {

		
		if (localPos.LengthSq() < range * range) {
			//the other car is within range of us

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

				//we have a possible collision now
				possibleCollision = pOwner->GetOtherVehicle();
			}
		}

	}
	//create the steering force vector
	Vector2D steeringForce = Vector2D();

	//if we have a collision target
	if (possibleCollision != nullptr) {
		//we now want to overtake
		_wantingToOvertake = true;

		//calculate the steering force needed to get around the obstalce
		double multiplier = 1.0 + (mininumBoxLength - localPos.x) / mininumBoxLength;

		steeringForce.y = (bRadius - localPos.y) * multiplier;

		double breakPower = 0.2;

		steeringForce.x = (bRadius - localPos.x) * breakPower;
	}

	//convert the local positon back to world space
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

	tempX = (transform._11 * vec.x) + (transform._21 * vec.y) + (transform._31);
	tempY = (transform._12 * vec.x) + (transform._22 * vec.y) + (transform._32);
	
	vec.x = tempX;
	vec.y = tempY;

	//returns the world space vector
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
	//Generates a random wander target in the screen bounds
	Vector2D randomTarget = Vector2D(rand() % 1024, rand() % 768);

	//The left bound of the screen is -512 so to get this in the range
	//we subtract half  of our screen width from the random location
	randomTarget.x -= 1024 / 2;
	randomTarget.y -= 768 / 2;

	pOwner->SetWanderTarget(randomTarget);
}
