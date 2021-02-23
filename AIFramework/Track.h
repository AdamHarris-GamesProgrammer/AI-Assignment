#pragma once

#include "Node.h"
#include "TrackConverter.h"
#include "TrackReader.h"
#include "Pathfinder.h"

#include <string>
#include <iostream>

class Track
{
public:
	//Default constructor
	Track() = default;

	/// <summary>
	/// The constructor for the maze object which takes a filepath 
	/// </summary>
	/// <param name="filepath">The filepath for the Maze</param>
	Track(std::string filepath);

	//Overrides the default destructor in order to delete the memory associated with this class
	~Track();

	/// <summary>
	/// This function calls the pathfinders solve method and will process the directions
	/// </summary>
	void Solve();



	/// <summary>
	/// Initializes the reader and converter with the map provided
	/// </summary>
	/// <returns>If this method returns true then it means initializing was successful</returns>
	bool Initialize();

	/// <summary>
	/// Gets the Node Path for the user
	/// </summary>
	std::list<Node*> GetNodePath();

	/// <summary>
	/// Gets the Directions for the user
	/// </summary>
	std::list<char> GetDirections();

	void SolvePathToNextPoint(int current, int next);

private:
	//Holds the MazeReader object which is used for reading the maze from the file
	TrackReader* mReader = nullptr;

	//Holds the MazeConverter object which is used for converting the maze file into usable nodes
	TrackConverter* mConverter = nullptr;

	//Holds the Pathfinder object which is used to solve the maze
	Pathfinder* mPathfinder = nullptr;

	//The filepath to the maze file
	std::string mFilePath;

	//This list holds the directions to the end point in the form of NSEW directions
	std::list<char> mDirections;

	//This list holds the Node path to the end point in pure Node form
	std::list<Node*> mNodePath;

	Node* mCopyOfMap;
	std::vector <Node*> mCopyOfWaypoints;
};

