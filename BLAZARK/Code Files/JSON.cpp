#include "JSON.h"

nlohmann::json LoadJSON(std::string fileName)
{

	//Opens input file stream 
	std::ifstream i(defaultDir + fileName);
	//Creates a json object
	nlohmann::json j;

	//if file fails to open
		//return empty json
	if (!i.good())
	{
		return j;
	}

	//Inputs the file contents into the json object
	i >> j;

	//Closes the file stream
	i.close();

	//Returns the filled json object
	return j;
}

void CreateJSON(std::string fileName, nlohmann::json j)
{
	std::ofstream o(defaultDir + fileName);

	//outputs the json object to the json file
	o << j;

	//Closes the file stream
	o.close();
}
