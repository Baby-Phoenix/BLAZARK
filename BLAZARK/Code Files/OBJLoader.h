#pragma once

#include "Mesh.h"

#include<iostream>
#include<fstream>
#include<sstream>

static void loadOBJ(const char* filename, Mesh& mesh) {
	// Opens file
	std::ifstream obj_file(filename);
	std::ifstream mtl_file;

	// Throws error if file not open
	if (!obj_file.is_open()) {
		throw "ERROR::OBJLOADER::Could not open file.";
	}

	std::stringstream ss;
	std::string obj_line = "";
	std::string prefix = "";

	std::string mtl_filename;
	std::string mtl_line = "";

	// Vertex Data
	std::vector<glm::fvec3> raw_vertex_positions;
	std::vector<glm::fvec3> raw_vertex_normals;
	std::vector<glm::fvec2> raw_vertex_texcoords;
	std::vector<Material> raw_vertex_materials;
	std::vector<GLint> vertex_position_indicies;
	std::vector<GLint> vertex_normal_indicies;
	std::vector<GLint> vertex_texcoord_indicies;
	std::vector<glm::vec3> vertex_positions;
	std::vector<glm::vec3> vertex_normals;
	std::vector<glm::vec2> vertex_texcoords;
	std::vector<Material> vertex_materials;

	std::map<std::string, int> materialDict;
	std::string mat_name = "";
	int mat_id = -1;

	glm::vec3 temp_vec3;
	glm::vec2 temp_vec2;
	Material temp_mat;
	std::string temp_matName = "";
	GLint matCounter = 0;
	GLint index = 0;

	// Iterates through each line of the .obj file
	while (std::getline(obj_file, obj_line)) {
		// Gets the prefix of the line
		ss.clear();
		ss.str(obj_line);
		ss >> prefix;

		if (prefix == "mtllib") {
			ss >> mtl_filename;
			std::string mtl_filepath = filename;
			mtl_filepath.erase(mtl_filepath.end() - mtl_filename.size(), mtl_filepath.end());
			mtl_filepath += mtl_filename;
			
			mtl_file.open(mtl_filepath.c_str());

			// Throws error if file not open
			if (!obj_file.is_open()) {
				throw "ERROR::OBJLOADER::Could not open file.";
			}

			while (std::getline(mtl_file, mtl_line)) {
				ss.clear();
				ss.str(mtl_line);
				ss >> prefix;

				if (prefix == "newmtl") {
					ss >> mat_name;
					mat_id++;
					materialDict.insert(std::pair<std::string, int>(mat_name, mat_id));
					temp_mat.mat_id = mat_id;
				}
				else if (prefix == "Ns") {
					ss >> temp_mat.specularShininess;
				}
				else if (prefix == "Ka") {
					ss >> temp_mat.ambientColour.x >> temp_mat.ambientColour.y >> temp_mat.ambientColour.z;
				}
				else if (prefix == "Kd") {
					ss >> temp_mat.diffuseColour.x >> temp_mat.diffuseColour.y >> temp_mat.diffuseColour.z;
				}
				else if (prefix == "Ks") {
					ss >> temp_mat.specularColour.x >> temp_mat.specularColour.y >> temp_mat.specularColour.z;
				}
				else if (prefix == "d") {
					ss >> temp_mat.dissolve;
					raw_vertex_materials.push_back(temp_mat);
				}
			}
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
		else if (prefix == "usemtl" || prefix == "end") {
			if (matCounter != 0) {
				for (int i = 0; i < raw_vertex_materials.size(); i++) {
					auto it = materialDict.find(temp_matName);
					if (it != materialDict.end()) {
						if (it->second == raw_vertex_materials[i].mat_id) {
							matCounter *= 3;
							while (matCounter > 0) {
								vertex_materials.push_back(raw_vertex_materials[i]);
								matCounter--;
							}
						}
					}
				}
			}
			ss >> temp_matName;
		}
		else if (prefix == "f") {
			int counter = 0;
			matCounter++;

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
	mesh.SetMaterials(vertex_materials);

	//DEBUG
	std::cout << "Nr of vertices: " << vertex_positions.size() << "\n";

	//Loaded success
	std::cout << filename << " file loaded!" << "\n";
}