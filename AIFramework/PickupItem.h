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

    void CollisionResolution();

    void GenerateNewPosition();

    void SetPlaceablePositions(std::vector<Vector2D> positions) {
        _placeablePositions = positions;
    }

    void SetWaypoints(std::vector<Waypoint*> waypoints) {
        _waypoints = waypoints;
    }

    void FindClosestWaypoint();

    void update(const float t) override;

    Vector2D GetPosition() const { return _position; }
    Vector2D GetTilePosition() const { return _tilePosition; }
    bool PickedUp() const { return _isPickedUp; }

    Waypoint* GetCurrentWaypoint() const {
        return _closestWaypoint;
    }

    Waypoint* GetNextWaypoint() const {
        return _nextWaypoint;
    }
protected:

private:
    int _xPos;
    int _yPos;

    bool _isPickedUp = false;
    float _timeBetweenPickups = 5.0f;
    float _timer = _timeBetweenPickups;

    Waypoint* _currentWaypoint;
    Waypoint* _nextWaypoint;



    std::vector<Vector2D> _placeablePositions;
    std::vector<Waypoint*> _waypoints;
    

    Vector2D _position;
    Vector2D _tilePosition;

};

