#pragma once

// External Libraries
#include "Dependencies/tinygltf/tiny_gltf.h"
#include <glad/glad.h>
#include <iostream>

// Project Files
#include "OBJLoader.h"

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

void loadGLTF(const char* fileName, Mesh& mesh, bool UVYFlip = true);