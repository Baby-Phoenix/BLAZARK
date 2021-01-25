#pragma once

#include "Dependencies/json/json.hpp"
#include <fstream>


	//The default directory for json files
	static std::string defaultDir = "BLAZARK/Resource Files/JSONFiles/";

	//Loads in the file and places it in a json object and returns the object
	nlohmann::json LoadJSON(std::string fileName);

	//Takes a json and a file name and outputs the dat within the json object to the file
	void CreateJSON(std::string fileName, nlohmann::json j);

