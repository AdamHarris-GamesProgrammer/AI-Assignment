#include "Pathfinder.h"

//Using a initializer list to initialize the mStartNode and mEndNode variables
Pathfinder::Pathfinder(Node* startNode, Node* endNode) : mStartNode(startNode), mEndNode(endNode) {}

Pathfinder::~Pathfinder()
{
	//Clears the directions list
	mDirectionsToGoal.clear();

	//clears the nodes list
	mPathOfNodes.clear();
}

bool Pathfinder::FindPath()
{
	/// <summary>
	/// Lambda expression to get the distance between two nodes on the map. 
	/// </summary>
	/// <returns>The distance between two nodes</returns>
	auto distance = [](Node* a, Node* b)
	{
		//Returns the distance between the two nodes
		return sqrtf((a->xPos - b->xPos) * (a->xPos - b->xPos) + (a->yPos - b->yPos) * (a->yPos - b->yPos));
	};

	/// <summary>
	/// Lambda expression used to calculate the heuristic value between two nodes.
	/// </summary>
	/// <returns>The heuristic value between nodes on the map</returns>
	auto heuristic = [distance](Node* a, Node* b) {
		/*Currently the heuristic value is based upon the overall distance between two nodes,
		however this could be changed to take further variables into mind such as the players speed
		on a particular type of tile. e.g. the player would move slower through water than on solid stone*/
		return distance(a, b);
	};

	//Sets the current node to the starting node
	Node* currentNode = mStartNode;

	//sets the local goal to 0 as this is needed to compare the viability of the path
	mStartNode->localGoal = 0.0f;
	//sets the heuristic value between this tile and the end tile
	mStartNode->globalGoal = heuristic(currentNode, mEndNode);

	//This list will contain the untested nodes that will be used to cycle through each node in the map 
	std::list<Node*> untestedNodes;
	//Adds the starting node to the untested node list
	untestedNodes.push_back(mStartNode);

	bool pathFound = false;

	while (!untestedNodes.empty())
	{
		/*Sorts the list by order of distance in ascending order.
		This is done as lists do not have iterative access meaning it is harder to grab an
		element at a specified index.
		*/
		untestedNodes.sort(
			[](const Node* nodeA, const Node* nodeB) {
				return nodeA->globalGoal < nodeB->globalGoal;
			});

		//The elements at the start of the list may have already been visited, meaning that testing them would use unnecessary resources.
		while (!untestedNodes.empty() && untestedNodes.front()->isVisited)
		{
			//remove the front node if it has been visited
			untestedNodes.pop_front();
		}

		//if there are no nodes available then break out of the loop
		if (untestedNodes.empty())
			break;

		//sets the current node to the front of the list
		currentNode = untestedNodes.front();
		currentNode->isVisited = true; //Nodes only need to be calculated once

		//Explore this node's neighbors
		for (auto nodeNeighbor : currentNode->neighborNodes) {
			//if the neighbor node is not visited and is not an obstacle then add it to the untestedNode list
			if (!nodeNeighbor->isVisited && !nodeNeighbor->isObstacle) {
				//Adds the neighboring node to the list
				untestedNodes.push_back(nodeNeighbor);
			}


			//Calculates the neighbors 'potentially' lower distance 
			float potentiallyLowerGoal = currentNode->localGoal + distance(currentNode, nodeNeighbor);

			//If the distance between this node and the neighbors is lower than the local score of the neighbor is then this should be the new parent node of the neighbor 
			if (potentiallyLowerGoal < nodeNeighbor->localGoal) {
				//Sets the parent node to the new closest node
				nodeNeighbor->parentNode = currentNode;

				//sets the local goal to the new lowest goal
				nodeNeighbor->localGoal = potentiallyLowerGoal;

				//Calculate the heuristic value of the neighbor based on there local goal and the distance to the end point
				nodeNeighbor->globalGoal = nodeNeighbor->localGoal + heuristic(nodeNeighbor, mEndNode);
			}

			/*if the distance between the neighbor node and the end node is 0 then it means a path has been found.
			However it may not be the shortest path*/
			if (distance(nodeNeighbor, mEndNode) == 0) pathFound = true;
		}
	}

	//returns if the path has been found
	return pathFound;
}

void Pathfinder::ProcessDirections()
{
	if (mEndNode != nullptr) {
		//Sets the previousNode to the endPoint as the A* algorithm works backwards
		Node* previousNode = mEndNode;


		//keep looping until the previousNode no longer has a parent this can only be the starting node
		while (previousNode->parentNode != nullptr)
		{
			//adds the node to the path of nodes
			mPathOfNodes.push_back(previousNode);

			//Adds the directions to the direction list
			//Adds eastern direction
			if (previousNode->parentNode->xPos < previousNode->xPos) mDirectionsToGoal.push_front('E');

			//Adds western direction
			else if (previousNode->parentNode->xPos > previousNode->xPos) mDirectionsToGoal.push_front('W');

			//Adds northern direction
			else if (previousNode->parentNode->yPos > previousNode->yPos) mDirectionsToGoal.push_front('N');

			//Adds southern direction
			else if (previousNode->parentNode->yPos < previousNode->yPos) mDirectionsToGoal.push_front('S');

			//Sets the previous node to the parent of this node
			previousNode = previousNode->parentNode;
		}
	}
}


#pragma region Getters
std::list<char> Pathfinder::GetDirections()
{
	return mDirectionsToGoal;
}

std::list<Node*> Pathfinder::GetPathOfNodes()
{
	return mPathOfNodes;
}
#pragma endregion 
