#include "PickupItem.h"



HRESULT	PickupItem::initMesh(ID3D11Device* pd3dDevice)
{
	setTextureName(L"Resources\\yellow.dds");

	m_scale = XMFLOAT3(20, 20, 1);
	setTextureName(L"Resources\\pickup.dds");

	HRESULT hr = DrawableGameObject::initMesh(pd3dDevice);

	setPosition(XMFLOAT3(1000, 1000, 0));

	_isPickedUp = true;

	return hr;
}

void PickupItem::CollisionResolution()
{
	_timer = _timeBetweenPickups;
	_isPickedUp = true;

	//Move pickup off screen instead of deleting it
	setPosition(XMFLOAT3(1000.0f, 1000.0f, 0.0f));

	//Notifies observers that a pickup has been collected
	Notify(PICKUP_COLLECTED);
}

void PickupItem::GenerateNewPosition()
{
	//Generates a index between 0 and the length of the vector
	int randomIndex = rand() % _placeablePositions.size();

	_position = _placeablePositions[randomIndex];

	_position.x += SCREEN_WIDTH / 2;
	_position.y += SCREEN_HEIGHT / 2;

	int xPos = _position.x / 51.2;
	int yPos = _position.y / 38.4;

	_tilePosition = Vector2D(xPos, yPos);

	_position.x -= SCREEN_WIDTH / 2;
	_position.y -= SCREEN_HEIGHT / 2;

	setPosition(XMFLOAT3(_position.x, _position.y, 0.0f));
}

void PickupItem::update(const float t)
{
	if (_isPickedUp) {
		_timer -= t;

		if (_timer <= 0.0f) {
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
