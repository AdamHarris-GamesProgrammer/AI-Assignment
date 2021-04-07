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

	mWaypoints.reserve(15);
	mWaypoints.resize(15);

	for (int x = 0; x < mWidth; x++) {
		for (int y = 0; y < mHeight; y++) {

			//This is the current index we want to get the character from
			int index = y * mWidth + x;

			//Gets the current character
			char currentCharacter = map[index];

			//Sets the current node's properties based on what the current character is. 
			switch (currentCharacter) {
			case 'x':
				//'x' is used to denote impassable nodes such as walls
				mNodeMap[index].isObstacle = true;
				break;
			case '.':
				//'.' is used to represent a empty space
				mNodeMap[index].isObstacle = false;
				break;
			case '1':
				//'A' is used to represent the starting point
				mStartPoint = &mNodeMap[index];

				break;
				//All the rest of our switch statement checks where the waypoints are and adds them to the waypoints vector
			case 'A':
				mWaypoints[0] = &mNodeMap[index];
				break;
			case 'B':
				mWaypoints[1] = &mNodeMap[index];
				break;
			case 'C':
				mWaypoints[2] = &mNodeMap[index];
				break;
			case 'D':
				mWaypoints[3] = &mNodeMap[index];
				break;
			case 'E':
				mWaypoints[4] = &mNodeMap[index];
				break;
			case 'F':
				mWaypoints[5] = &mNodeMap[index];
				break;
			case 'G':
				mWaypoints[6] = &mNodeMap[index];
				break;
			case 'H':
				mWaypoints[7] = &mNodeMap[index];
				break;
			case 'I':
				mWaypoints[8] = &mNodeMap[index];
				break;
			case 'J':
				mWaypoints[9] = &mNodeMap[index];
				break;
			case 'K':
				mWaypoints[10] = &mNodeMap[index];
				break;
			case 'L':
				mWaypoints[11] = &mNodeMap[index];
				break;
			case 'M':
				mWaypoints[12] = &mNodeMap[index];
				break;
			case 'N':
				mWaypoints[13] = &mNodeMap[index];
				break;
			case 'O':
				mWaypoints[14] = &mNodeMap[index];
				break;
			default:
				//Invalid character has been found
				std::cerr << "[ERROR: MazeConverter::ProcessMap(): Current Map has a invalid character: " << currentCharacter << std::endl;
				successful = false;
			}

			//Initializes all default values for each node
			mNodeMap[index].pos = Vector2D(x,y);


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

void TrackConverter::DestroyMap()
{
	delete[] mNodeMap;
	mNodeMap = nullptr;
}

