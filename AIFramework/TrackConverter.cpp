#include "TrackConverter.h"

TrackConverter::TrackConverter(int width, int height) : mWidth(width), mHeight(height) {
	mWaypoints = std::vector<Node*>();
}

TrackConverter::~TrackConverter()
{
	//Deletes the node map for this maze
	delete[] mNodeMap;
	mNodeMap = nullptr;
	//No need to delete mStartNode and mEndNode here as they are contained in the mNodeMap
}

bool TrackConverter::ProcessMap(char* map)
{
	bool successful = true;

	mNodeMap = new Node[mWidth * mHeight];
	if (!mNodeMap) {
		std::cerr << "[ERROR: MazeConverter::ProcessMap(): Failed to allocate memory to map" << std::endl;
		return false;
	}

	bool hasStartPoint = false;
	bool hasEndPoint = false;

	for (int x = 0; x < mWidth; x++) {
		for (int y = 0; y < mHeight; y++) {

			//This is the current index we want to get the character from
			int index = y * mWidth + x;

			//Gets the current character
			char currentCharacter = map[index];

			//Sets the current node's properties based on what the current character is. 
			switch (currentCharacter) {
			case '0':
				//'x' is used to denote impassable nodes such as walls
				mNodeMap[index].isObstacle = true;
				break;
			case '1':
				//'.' is used to represent a empty space
				mNodeMap[index].isObstacle = false;
				break;
			case 'A':
				//'A' is used to represent the starting point
				mStartPoint = &mNodeMap[index];

				//if has start point has not been set to true then set it
				if (!hasStartPoint) hasStartPoint = true;
				else
				{
					//if this code is called again then it means there are two starting points on the map
					std::cerr << "[ERROR: MazeConverter::ProcessMap(): Current Map has more than one start point" << std::endl;
					successful = false;
				}

				break;
			case '*':

				mWaypoints.push_back(&mNodeMap[index]);

				//if has end point has not been set to true then set it
				if (!hasEndPoint) hasEndPoint = true;
				else
				{
					//if this code is called again then it means there are two end points on the map 
					std::cerr << "[ERROR: MazeConverter::ProcessMap(): Current Map has more than one end point" << std::endl;
					successful = false;
				}
				break;
			default:
				//Invalid character has been found
				std::cerr << "[ERROR: MazeConverter::ProcessMap(): Current Map has a invalid character: " << currentCharacter << std::endl;
				successful = false;
			}

			//Initializes all default values for each node
			mNodeMap[index].xPos = x;
			mNodeMap[index].yPos = y;


			//Sets neighbor nodes
			//if the node is not on the left most side then add the left hand node to the nodes neighbors
			if (x > 0) {
				mNodeMap[index].neighborNodes.push_back(&mNodeMap[y * mWidth + (x - 1)]);
			}

			//if the node is not on the right most side then add the right hand node to the nodes neighbors
			if (x < mWidth - 1) {
				mNodeMap[index].neighborNodes.push_back(&mNodeMap[y * mWidth + (x + 1)]);
			}

			//if the node is not on top of the map then add the node above to the nodes neighbors
			if (y > 0) {
				mNodeMap[index].neighborNodes.push_back(&mNodeMap[(y - 1) * mWidth + x]);
			}

			//if the node is not at the bottom of the map then add the node below it to the nodes neighbors
			if (y < mHeight - 1) {
				mNodeMap[index].neighborNodes.push_back(&mNodeMap[(y + 1) * mWidth + x]);
			}

			//Diagonal neighbors
			if (y > 0 && x > 0)
				mNodeMap[y * mWidth + x].neighborNodes.push_back(&mNodeMap[(y - 1) * mWidth + (x - 1)]);
			if (y < mHeight - 1 && x>0)
				mNodeMap[y * mWidth + x].neighborNodes.push_back(&mNodeMap[(y + 1) * mWidth + (x - 1)]);
			if (y > 0 && x < mWidth - 1)
				mNodeMap[y * mWidth + x].neighborNodes.push_back(&mNodeMap[(y - 1) * mWidth + (x + 1)]);
			if (y < mWidth - 1 && x < mWidth - 1)
				mNodeMap[y * mWidth + x].neighborNodes.push_back(&mNodeMap[(y + 1) * mWidth + (x + 1)]);

		}
	}
	//If the maze does not have a start point
	if (!hasStartPoint) {
		//Output error
		successful = false;
		std::cerr << "ERROR: MazeConverter::ProcessMap(): No start point on map" << std::endl;
	}

	//If the maze does not have a end point
	if (!hasEndPoint) {
		//Output error
		successful = false;
		std::cerr << "ERROR: MazeConverter::ProcessMap(): No end point on map" << std::endl;
	}

	//Returns if there was any errors during the conversion process
	return successful;
}

int TrackConverter::GetHeight()
{
	return mHeight;
}

int TrackConverter::GetWidth()
{
	return mWidth;
}

Node* TrackConverter::GetNodeMap()
{
	return mNodeMap;
}

Node* TrackConverter::GetStartPoint()
{
	return mStartPoint;
}

std::vector<Node*> TrackConverter::GetWaypoints()
{
	return mWaypoints;
}
