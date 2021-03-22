#pragma once
#include "DrawableGameObject.h"
#include "Subject.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define WAYPOINT_RESOLUTION 20

class PickupItem :
    public DrawableGameObject, public Subject
{
public:
    virtual HRESULT initMesh(ID3D11Device* pd3dDevice);

    void GenerateNewPosition();

    void SetPlaceablePositions(std::vector<Vector2D> positions) {
        _placeablePositions = positions;
    }

    Vector2D GetPosition() const { return _position; }
    Vector2D GetTilePosition() const { return _tilePosition; }
protected:

private:
    std::vector<Vector2D> _placeablePositions;
    Vector2D _position;
    Vector2D _tilePosition;

};

