#pragma once

#include "Mesh.h"

#include<iostream>
#include<fstream>
#include<sstream>

static void loadOBJ(const char* file_name, Mesh& mesh) {
	// Opens file
	std::ifstream obj_file(file_name);

	// Throws error if file not open
	if (!obj_file.is_open()) {
		throw "ERROR::OBJLOADER::Could not open file.";
	}

	std::stringstream ss;
	std::string obj_line = "";
	std::string prefix = "";

	// Vertex Data
	std::vector<glm::fvec3> raw_vertex_positions;
	std::vector<glm::fvec3> raw_vertex_normals;
	std::vector<glm::fvec2> raw_vertex_texcoords;
	std::vector<GLint> vertex_position_indicies;
	std::vector<GLint> vertex_normal_indicies;
	std::vector<GLint> vertex_texcoord_indicies;
	std::vector<glm::vec3> vertex_positions;
	std::vector<glm::vec3> vertex_normals;
	std::vector<glm::vec2> vertex_texcoords;

	glm::vec3 temp_vec3;
	glm::vec2 temp_vec2;
	GLint index = 0;

	// Iterates through each line of the .obj file
	while (std::getline(obj_file, obj_line)) {
		// Gets the prefix of the line
		ss.clear();
		ss.str(obj_line);
		ss >> prefix;

		if (prefix == "mtllib") {

		}
		else if (prefix == "v") {
			ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
			raw_vertex_positions.push_back(temp_vec3);
		}
		else if (prefix == "vt") {
			ss >> temp_vec2.x >> temp_vec2.y;
			raw_vertex_texcoords.push_back(temp_vec2);
		}
		else if (prefix == "vn") {
			ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
			raw_vertex_normals.push_back(temp_vec3);
		}
		else if (prefix == "use_mtl") {

		}
		else if (prefix == "f") {
			int counter = 0;

			while (ss >> index) {
				// Pushes the indices into the correct arrays
				if (counter == 0)
					vertex_position_indicies.push_back(index);
				else if (counter == 1)
					vertex_texcoord_indicies.push_back(index);
				else if (counter == 2)
					vertex_normal_indicies.push_back(index);

				// Handles characters to be ignored
				if (ss.peek() == '/') {
					++counter;
					ss.ignore(1, '/');
				}
				else if (ss.peek() == ' ') {
					++counter;
					ss.ignore(1, ' ');
				}

				// Resets the counter
				if (counter > 2)
					counter = 0;
			}
		}

	}

	// Arrange raw data as per indicies
	for (size_t i = 0; i < vertex_position_indicies.size(); ++i) {
		vertex_positions.push_back(raw_vertex_positions[vertex_position_indicies[i] - 1]);
		vertex_normals.push_back(raw_vertex_normals[vertex_normal_indicies[i] - 1]);
		vertex_texcoords.push_back(raw_vertex_texcoords[vertex_texcoord_indicies[i] - 1]);
	}
	
	mesh.SetPositions(vertex_positions);
	mesh.SetNormals(vertex_normals);
	mesh.SetTexCoords(vertex_texcoords);

	//DEBUG
	std::cout << "Nr of vertices: " << vertex_positions.size() << "\n";

	//Loaded success
	std::cout << "OBJ file loaded!" << "\n";
}