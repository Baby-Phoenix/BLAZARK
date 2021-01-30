#pragma once

#include "Mesh.h"
#include "Texture.h"
#include "OBJLoader.h"
#include "Shader.h"
#include "Camera.h"

class Sprite2D
{

public:
	
	Sprite2D(Texture* tex, GameObject* entity,float width = 1, float height = 1, float transparency = 1);
	virtual	~Sprite2D() = default;

	void Draw(Camera* cam);

private:

	glm::vec2 m_bottomLeft = glm::vec2(0.f, 0.f);
	glm::vec2 m_topRight = glm::vec2(1.f, 1.f);

	glm::vec2 m_textureSize = glm::vec2();

	float m_width;
	float m_height;
	float m_transparency;

	static Mesh* m_planeMesh;
	static VertexArray* VAO;
	static Shader* m_Sprite2D_shader;

	Texture* m_texture;
	GameObject* m_entity;
};