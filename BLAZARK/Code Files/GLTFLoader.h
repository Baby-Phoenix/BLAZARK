#pragma once

// Project Files
#include "Mesh.h"

// External Libraries
#include "Dependencies/tinygltf/tiny_gltf.h"
#include <glad/glad.h>
#include <iostream>

namespace tinygltf {
	class Model;
	struct Primitive;
}

// Structure to hold data
struct DATAGETTER {
	const unsigned char* data;
	size_t accessorLength;
	int byteStride;
	int componentSize;
};

void loadGLTF(const char* fileName, Mesh& mesh, bool UVYFlip = true) {
	auto model = std::make_unique<tinygltf::Model>();
	std::string warn, err;
	bool status;

	//////////////////////////////
	// Parses data in GLTF file //
	//////////////////////////////
	{
		auto parser = std::make_unique<tinygltf::TinyGLTF>();

		status = parser->LoadASCIIFromFile(&*model, &err, &warn, fileName);
	}

	/////////////////////////////////////////////
	// Prints Errors & Warnings to the Console //
	/////////////////////////////////////////////
	if (!status) {
		printf("Failed to load %s\n", fileName);
		if (!err.empty())
			printf("Error deriving mesh from %s: %s\n", fileName, err.c_str());
		if (!warn.empty())
			printf("Warning(s) deriving mesh from %s: %s\n", fileName, warn.c_str());
	}

	//////////////////////////////////
	// Extracts data from GLTF file //
	//////////////////////////////////
	{
		if (model.get()->meshes.size() == 0) {
			err = "No mesh data found in file.";
			status = false;
		}
		else {
			const tinygltf::Mesh& meshData = model.get()->meshes[0];

			if (meshData.primitives.size() == 0) {
				err = "No geometry data found in mesh.";
				status = false;
			}
			else {
				std::vector<glm::vec3> vertPos;
				std::vector<glm::vec3> vertNorms;
				std::vector<glm::vec2> uvCoords;

				bool firsttime = true;

				for (size_t i = 0; i < meshData.primitives.size(); ++i) {

					//////////////////////////////////////////////
					// Processes the primitive data in the mesh //
					//////////////////////////////////////////////
					{
						const tinygltf::Primitive geo = model.get()->meshes[0].primitives[i];

						if (geo.indices == -1) {
							err = "No indices found in primitive.\nFile must contain indicies for this loader to function.";
							status = false;
						}
						else {
							///// Gets individual vertex data from the indicies /////
							const tinygltf::Accessor& accessor = model.get()->accessors[geo.indices];
							const tinygltf::BufferView& bufferView = model.get()->bufferViews[accessor.bufferView];
							const tinygltf::Buffer& buffer = model.get()->buffers[bufferView.buffer];
							const unsigned char* data = &(buffer.data[bufferView.byteOffset + accessor.byteOffset]);

							size_t accessorLength = accessor.count;
							int byteStride = accessor.ByteStride(bufferView);
							int componentSize = tinygltf::GetComponentSizeInBytes(accessor.componentType) * tinygltf::GetNumComponentsInType(accessor.type);

							DATAGETTER indexer = { data, accessorLength, byteStride, componentSize };
							/////////////////////////////////////////////////////////

							if (indexer.componentSize != sizeof(GLshort)) {
								err = "Unsupported format of indicies in primitive.";
								status = false;
							}
							else {
								///// Finds index of vertex positions /////
								auto iterator = geo.attributes.find("POSITION");
								int vertexID;

								if (iterator == geo.attributes.end())
									vertexID = -1;
								else
									vertexID = iterator->second;
								///////////////////////////////////////////

								if (vertexID == -1) {
									err = "No vertex positions found in mesh.";
									status = false;
								}
								else {
									///// Finds index of vertex normals /////
									iterator = geo.attributes.find("NORMAL");
									int normalID;
									if (iterator == geo.attributes.end())
										normalID = -1;
									else
										normalID = iterator->second;
									/////////////////////////////////////////

									bool NormalsExist = normalID != -1;
									if (!NormalsExist)
										warn += "\nNo vertex normals found in mesh.";

									///// Finds index of UV coordinates /////
									iterator = geo.attributes.find("TEXCOORD_0");
									int uvID;
									if (iterator == geo.attributes.end())
										uvID = -1;
									else
										uvID = iterator->second;
									////////////////////////////////////////

									bool UVsExist = uvID != -1;
									if (!UVsExist)
										warn += "\nNo UV coordinates found in mesh.";

									DATAGETTER VertexGetter, NormalGetter, UVGetter;

									///// Gets vertex position data from the model /////
									const tinygltf::Accessor& accessor = model.get()->accessors[vertexID];
									const tinygltf::BufferView& bufferView = model.get()->bufferViews[accessor.bufferView];
									const tinygltf::Buffer& buffer = model.get()->buffers[bufferView.buffer];
									const unsigned char* data = &(buffer.data[bufferView.byteOffset + accessor.byteOffset]);

									size_t accessorLength = accessor.count;
									int byteStride = accessor.ByteStride(bufferView);
									int componentSize = tinygltf::GetComponentSizeInBytes(accessor.componentType) * tinygltf::GetNumComponentsInType(accessor.type);

									VertexGetter = { data, accessorLength, byteStride, componentSize };
									////////////////////////////////////////////////////

									if (VertexGetter.componentSize != sizeof(glm::vec3)) {
										err = "Unsupported format of vertex position data.";
										status = false;
									}
									else {
										if (NormalsExist) {
											///// Gets vertex normal data from the model /////
											const tinygltf::Accessor& accessor = model.get()->accessors[normalID];
											const tinygltf::BufferView& bufferView = model.get()->bufferViews[accessor.bufferView];
											const tinygltf::Buffer& buffer = model.get()->buffers[bufferView.buffer];
											const unsigned char* data = &(buffer.data[bufferView.byteOffset + accessor.byteOffset]);

											size_t accessorLength = accessor.count;
											int byteStride = accessor.ByteStride(bufferView);
											int componentSize = tinygltf::GetComponentSizeInBytes(accessor.componentType) * tinygltf::GetNumComponentsInType(accessor.type);

											NormalGetter = { data, accessorLength, byteStride, componentSize };
											//////////////////////////////////////////////////

											if (NormalGetter.componentSize != sizeof(glm::vec3)) {
												warn += "Unsupported format of vertex normal data.";
												NormalsExist = false;
											}
										}

										if (UVsExist) {
											///// Gets UV coordinate data from the model /////
											const tinygltf::Accessor& accessor = model.get()->accessors[uvID];
											const tinygltf::BufferView& bufferView = model.get()->bufferViews[accessor.bufferView];
											const tinygltf::Buffer& buffer = model.get()->buffers[bufferView.buffer];
											const unsigned char* data = &(buffer.data[bufferView.byteOffset + accessor.byteOffset]);

											size_t accessorLength = accessor.count;
											int byteStride = accessor.ByteStride(bufferView);
											int componentSize = tinygltf::GetComponentSizeInBytes(accessor.componentType) * tinygltf::GetNumComponentsInType(accessor.type);

											UVGetter = { data, accessorLength, byteStride, componentSize };
											//////////////////////////////////////////////////

											if (UVGetter.componentSize != sizeof(glm::vec2)) {
												warn += "Unsupported format of UV coordinate data.";
												UVsExist = false;
											}
										}

										size_t index = vertPos.size();

										vertPos.resize(vertPos.size() + indexer.accessorLength);

										if (NormalsExist)
											vertNorms.resize(vertNorms.size() + indexer.accessorLength);

										if (UVsExist)
											uvCoords.resize(uvCoords.size() + indexer.accessorLength);

										for (size_t i = index, j = 0; i < vertPos.size() + indexer.accessorLength && j < indexer.accessorLength; ++i, ++j) {
											GLshort vInd;

											// Finds the index of vertex
											memcpy(&vInd, &indexer.data[j * indexer.byteStride], sizeof(GLshort));

											// Gets the vertex position
											memcpy(&vertPos[i], &VertexGetter.data[vInd * VertexGetter.byteStride], sizeof(glm::vec3));

											// Gets the vertex normal
											if (NormalsExist)
												memcpy(&vertNorms[i], &NormalGetter.data[vInd * NormalGetter.byteStride], sizeof(glm::vec3));

											// Gets the UV coordinates
											if (UVsExist) {
												memcpy(&uvCoords[i], &UVGetter.data[vInd * UVGetter.byteStride], sizeof(glm::vec2));

												if (UVYFlip)
													uvCoords[i].y = 1.0f - uvCoords[i].y;
											}
										}

										mesh.SetPositions(vertPos);
										mesh.SetNormals(vertNorms);
										mesh.SetTexCoords(uvCoords);

										//DEBUG
										std::cout << "Nr of vertices: " << vertPos.size() << "\n";
									}
								}
							}
						}
					}
				}
			}
		}
	}

	/////////////////////////////////////////////
	// Prints Errors & Warnings to the Console //
	/////////////////////////////////////////////
	if (!status) {
		printf("Failed to load %s.gltf\n", fileName);
		if (!err.empty())
			printf("Error deriving mesh from %s: %s\n", fileName, err.c_str());
		if (!warn.empty())
			printf("Warning(s) deriving mesh from %s: %s\n", fileName, warn.c_str());
	}

	if (status)
		printf("Successfully loaded file: %s.\n", fileName);
}
