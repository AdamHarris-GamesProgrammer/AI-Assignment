#pragma once
#include <string>
#include <iostream>
#include <fstream>

class TrackReader
{
public:
	//Default constructor
	TrackReader() = default;

	//Overriding the default destructor to delete the character map
	~TrackReader();

	/// <summary>
	/// This method loads a map from the file and adds it to the char array
	/// </summary>
	/// <param name="filepath">The filepath to the maze text file</param>
	bool LoadMap(std::string filepath);

	/// <summary>
	/// Returns the array of chars to the user
	/// </summary>
	char* GetMapArray();

	/// <summary>
	/// Returns the width of the map to the user
	/// </summary>
	int GetWidth();

	/// <summary>
	/// Returns the height of the map to the user
	/// </summary>
	int GetHeight();

private:
	//A dynamic array holding the map data
	char* mMap;

	//The width of the map
	int mMapWidth;

	//The height of the map
	int mMapHeight;
};

