#include "TrackReader.h"
#include <list>

TrackReader::~TrackReader()
{
	//Deletes the character map for this maze
	delete[] mMap;
	mMap = nullptr;
}

bool TrackReader::LoadMap(std::string filepath)
{
	//Opens the file 
	std::ifstream inFile(filepath, std::ios::in | std::ios::binary);

	//Checks that the file is open
	if (inFile.is_open()) {
		std::string line;

		//Creates a list of lines that will be used to add to the map array later
		std::list<std::string> lines;

		//row counter
		int rows = 0;

		//Gets the next line in the file
		while (getline(inFile, line))
		{
			//adds the line to the lines list
			lines.push_back(line);

			//increments the row counter
			rows++;
		}

		//Sets the width and height of the map
		mMapWidth = line.length();
		mMapHeight = rows;

		//Creates the dynamic array
		mMap = new char[mMapWidth * mMapHeight];

		//y is used to access the row index 
		int y = 19;
		//cycles through each string element in the list of lines
		for (std::string c : lines) {
			//cycles through all the characters in the line
			for (int x = 0; x < mMapWidth; x++) {
				//assigns the value of the map at this index to the current character in the string
				mMap[y * mMapWidth + x] = c[x];
			}
			//increments the y variable
			y--;
		}

		//Close the file now we have finished reading from it
		inFile.close();

		return true;
	}
	else
	{
		//if the file was not opened then output a error
		std::cerr << "File: " << filepath << " could not be opened." << std::endl;

		return false;
	}
}

char* TrackReader::GetMapArray()
{
	return mMap;
}

int TrackReader::GetWidth()
{
	return mMapWidth;
}

int TrackReader::GetHeight()
{
	return mMapHeight;
}
