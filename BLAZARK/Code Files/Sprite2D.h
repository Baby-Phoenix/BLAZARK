#pragma once

#include "glm.hpp"
#include <string>
#include "Mesh.h"


class Sprite2D
{

public:



private:

	glm::vec2 m_bottomLeft = glm::vec2(0.f, 0.f);
	glm::vec2 m_topRight = glm::vec2(1.f, 1.f);

	glm::vec2 m_textureSize = glm::vec2();

	float m_width = 0;
	float m_height = 0;
	float m_transparency = 0;

	VertexBuffer VBO;
	VertexArray VAO;

};