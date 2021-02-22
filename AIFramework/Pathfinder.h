#pragma once
#include "Node.h"
#include "List"
#include <iostream>

class Pathfinder
{
public:
	//The default constructor for the class
	Pathfinder() = default;

	/// <summary>
	/// The constructor for the class which initializes the start and end nodes 
	/// </summary>
	/// <param name="startNode">The node that the player starts on</param>
	/// <param name="endNode">The node that the player ends on</param>
	Pathfinder(Node* startNode, Node* endNode);

	//Overriding the default destructor to delete the memory associated with the direction and path lists
	~Pathfinder();

	/// <summary>
	/// This function will calculate the shortest path for the maze. Using the A* Pathfinding Algorithm
	/// </summary>
	/// <returns>This function returns true if a path is found and false if a path is not found</returns>
	bool FindPath();

	/// <summary>
	/// This function will process the node list to find the directions the player needs to go in to get to the end
	/// </summary>
	void ProcessDirections();

	/// <summary>
	/// This function returns the node path list to the user
	/// </summary>
	std::list<Node*> GetPathOfNodes();

private:
	//The starting node
	Node* mStartNode = nullptr;

	//The ending node
	Node* mEndNode = nullptr;

	//This list contains the directions the player needs to take in the form of Nodes. 
	std::list<Node*> mPathOfNodes;
};

