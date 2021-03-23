#pragma once
#include <vector>
#include "Vector2D.h"

/// <summary>
/// The Node struct is used to represent the information of a cell in the maze
/// </summary>
struct Node {
	//Is this node an obstacle i.e a wall
	bool isObstacle = false;
	//Has this node already been visited 
	bool isVisited = false;

	//Distance to the end node
	float globalGoal = FLT_MAX;

	//Distance to the end node from here
	float localGoal = FLT_MAX;

	//X and Y position of the node in the map
	/*int xPos = 0;
	int yPos = 0;*/

	Vector2D pos = Vector2D(0, 0);

	//A vector containing all of the nodes in the maze level
	std::vector<Node*> neighborNodes;
	//A pointer to the neighbor node that is closest to the start node
	Node* parentNode = nullptr;
};