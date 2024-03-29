#pragma once
#include "Node.h"
#include <iostream>

class TrackConverter
{
public:
	TrackConverter() = default;

	/// <summary>
	/// Constructor for MazeConverter which takes in the width and height of the map
	/// </summary>
	/// <param name="width">The width of the map</param>
	/// <param name="height">The height of the map</param>
	TrackConverter(int width, int height);

	~TrackConverter();

	/// <summary>
	/// The ProcessMap method is used to convert the map information from characters into usable node information
	/// </summary>
	/// <param name="map">A pointer to a dynamic array of characters</param>
	bool ProcessMap(char* map);

	/// <summary>
	/// This function returns the height of the map
	/// </summary>
	int GetHeight();

	/// <summary>
	/// This function returns the width of the map
	/// </summary>
	int GetWidth();

	/// <summary>
	/// This function returns the entire Node Map
	/// </summary>
	Node* GetNodeMap();


	/// <summary>
	/// This function returns a pointer to the start node
	/// </summary>
	Node* GetStartPoint();

	std::vector<Node*> GetWaypoints();

	void SetAllNodesToUnvisited() {
		for (int i = 0; i < mWidth * mHeight; i++) {
			mNodeMap[i].isVisited = false;
		}
	}

	void DestroyMap();
private:
	//The width of the map
	int mWidth = 0;

	//The height of the map
	int mHeight = 0;

	//The node map in its entirety
	Node* mNodeMap = nullptr;

	//The starting point
	Node* mStartPoint = nullptr;

	

	std::vector<Node*> mWaypoints;
};

