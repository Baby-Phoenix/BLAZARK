#pragma once
#include "OBJLoader.h"

void cubeLoader(CubeFile& cube, std::string file);

class CubeFile {

public:
	CubeFile(){}
	CubeFile(std::vector<glm::vec3> values) {
		m_RGB = values;
	}

	CubeFile(glm::vec3 values[])
	{
		int size = sizeof(values);
		size /= sizeof(values[0]);

		for (int i = 0; i < size; i++) {
			m_RGB.push_back(values[i]);
		}
	}


	std::vector<glm::vec3>::iterator begin() { return m_RGB.begin(); }

	std::vector<glm::vec3>::iterator end() { return m_RGB.end(); }

private:

	friend void cubeLoader(CubeFile&, std::string);
	std::vector<glm::vec3> m_RGB;

};


void cubeLoader(CubeFile& cube, std::string file){

	std::ifstream cubeInfo;

	cubeInfo.open(file);

	if (cubeInfo.is_open()) {

		while (cubeInfo.good()) {
		

		}
	}

	else
	{
		std::cout << "The .cube file could not be read \n";
	}
}