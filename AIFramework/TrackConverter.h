#pragma once
#include "Node.h"
#include <iostream>
#include <vector>

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
	std::vector<Node*> GetNodeMap();

	/// <summary>
	/// This function returns a pointer to the start node
	/// </summary>
	Node* GetStartPoint();

	std::vector<Node*> GetWaypoints();


	void ClearParentNodes();

private:
	//The width of the map
	int mWidth = 0;

	//The height of the map
	int mHeight = 0;

	//The node map in its entirety
	//Node* mNodeMap = nullptr;

	std::vector<Node*> mNodeMap;

	//The starting point
	Node* mStartPoint = nullptr;

	//The ending point
	//Node* mEndPoint = nullptr;

	std::vector<Node*> mWaypoints;
};

