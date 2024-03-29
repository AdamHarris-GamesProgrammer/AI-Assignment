#include "PickupItem.h"
#include "Waypoint.h"



HRESULT	PickupItem::initMesh(ID3D11Device* pd3dDevice)
{
	
	setTextureName(L"Resources\\yellow.dds");

	m_scale = XMFLOAT3(20, 20, 1);
	setTextureName(L"Resources\\pickup.dds");

	HRESULT hr = DrawableGameObject::initMesh(pd3dDevice);

	//Sets our position to be off screen at the start
	setPosition(XMFLOAT3(1000, 1000, 0));

	_isPickedUp = true;

	return hr;
}

void PickupItem::CollisionResolution()
{
	//Resets the timer
	_timer = _timeBetweenPickups;
	_isPickedUp = true;

	//Move pickup off screen instead of deleting it, this is more efficient
	setPosition(XMFLOAT3(1000.0f, 1000.0f, 0.0f));

	//Notifies observers that a pickup has been collected
	Notify(PICKUP_COLLECTED);
}

PickupItem::~PickupItem()
{
	_placeablePositions.clear();
	_waypoints.clear();
}

void PickupItem::GenerateNewPosition()
{
	//Generates a index between 0 and the length of the vector
	int randomIndex = rand() % _placeablePositions.size();

	//Gets a position
	_position = _placeablePositions[randomIndex];

	//Converts the position to tiled position
	_position.x += SCREEN_WIDTH / 2;
	_position.y += SCREEN_HEIGHT / 2;

	_xPos = _position.x / 51.2;
	_yPos = _position.y / 38.4;

	_tilePosition = Vector2D(_xPos, _yPos);

	_position.x -= SCREEN_WIDTH / 2;
	_position.y -= SCREEN_HEIGHT / 2;


	//Sets our position 
	setPosition(XMFLOAT3(_position.x, _position.y, 0.0f));

	FindClosestWaypoint();
}

void PickupItem::FindClosestWaypoint()
{
	//Vector of the checkpoints in our game
	std::vector<int> vecCheckpoint{ 311,239,159,58,54,93,176,209,264,87,47,44,162,240,362 };

	//Finds our starting node 
	Waypoint* startNode = _waypoints[_yPos * WAYPOINT_RESOLUTION + _xPos];

	//Searches this vector for the closest waypoint to us
	float lowestDistance = FLT_MAX;
	int index = 0;

	//Finds the closest waypoint to the pickup
	for (int i = 0; i < vecCheckpoint.size(); i++) {
		float distance = Vec2DDistance(startNode->GetVectorPosition(), _waypoints[vecCheckpoint[i]]->GetVectorPosition());

		if (distance < lowestDistance) {
			lowestDistance = distance;
			index = i;
		}
	}

	//sets the next waypoint after the pickup
	_nextWaypoint = _waypoints[vecCheckpoint[index]];

	//cycles the index if needed
	if (index == 0) {
		index = vecCheckpoint.size() - 1;
	}
	else
	{
		index--;
	}

	//Sets our current waypoint to the one before the pickup
	_currentWaypoint = _waypoints[vecCheckpoint[index]];

	_pickupIndex = index;

	//Notify the observers that we have spawned
	Notify(PICKUP_SPAWNED);
}

void PickupItem::update(const float t)
{
	//If this pickup has been collected
	if (_isPickedUp) {
		//decrement our timer 
		_timer -= t;

		//if our timer is over
		if (_timer <= 0.0f) {
			//Generate our new position
			GenerateNewPosition();

			//Notifies observers that a pickup has spawned
			Notify(PICKUP_SPAWNED);

			//Reset our timer logic
			_isPickedUp = false;
			_timer = _timeBetweenPickups;
		}

	}

	DrawableGameObject::update(t);
}