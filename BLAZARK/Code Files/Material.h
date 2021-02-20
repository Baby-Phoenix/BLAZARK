#pragma once

#include <string>
#include <glm.hpp>

struct Material {
	int mat_id; // 1

	float specularShininess; // 1

	glm::vec3 ambientColour; // 3
	glm::vec3 diffuseColour; // 3
	glm::vec3 specularColour; // 3

	float dissolve; // 1
};
