#include "Track.h"

Track::Track(std::string filepath) : mFilePath(filepath) { 
	Initialize(); 
}

Track::~Track()
{
	//deletes the pointers used
	delete mPathfinder;
	delete mConverter;
	delete mReader;

	//Sets the pointers to  nullptr
	mPathfinder = nullptr;
	mConverter = nullptr;
	mReader = nullptr;

	//clears the direction and node path lists
	mDirections.clear();
	mNodePath.clear();
}

void Track::Solve()
{
	//if a path can be found
	if (mPathfinder->FindPath()) {
		//process the directions
		mPathfinder->ProcessDirections();

		//sets the node path list equal to the pathfinders node path list
		mNodePath = mPathfinder->GetPathOfNodes();
	}
}

bool Track::Initialize()
{
	//Creates the mReader object
	mReader = new TrackReader();

	//Checks that the file can be loaded
	if (!mReader->LoadMap(mFilePath)) {
		//Outputs error if the file cannot be read
		std::cerr << "ERROR: Maze::Initialize(): Map Reader Failed" << std::endl;
	}

	//Creates the mConverter object
	mConverter = new TrackConverter(mReader->GetWidth(), mReader->GetHeight());

	//Checks that the map can be converted
	if (!mConverter->ProcessMap(mReader->GetMapArray())) {
		//Outputs error if map cannot be converted for any reason
		std::cerr << "ERROR: Maze::Initialize(): Map Converter for " << mFilePath << " failed" << std::endl;
	}
	//Creates the pathfinder object 


	//Returns true as if the program gets to here there has been no problem initializing the map
	return true;
}

std::list<Node*> Track::GetNodePath()
{
	return mNodePath;
}

std::list<char> Track::GetDirections()
{
	return mDirections;
}

void Track::SolvePathToNextPoint(int current, int next)
{
	ResetMap();
	mPathfinder = new Pathfinder(mConverter->GetWaypoints()[current], mConverter->GetWaypoints()[next]);

	mPathfinder->FindPath();
	mPathfinder->ProcessDirections();
	mNodePath = mPathfinder->GetPathOfNodes();
}

void Track::ResetMap()
{
	delete mPathfinder;
	mPathfinder = nullptr;
	mConverter->DestroyMap();
	mConverter->ProcessMap(mReader->GetMapArray());
}
