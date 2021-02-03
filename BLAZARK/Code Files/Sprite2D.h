#pragma once

#include "GLTFLoader.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"
#include "Application.h"

class Sprite2D
{

public:
	
	Sprite2D(Texture* tex, GameObject* entity,float width = 1, float height = 1, float transparency = 1);
	virtual	~Sprite2D() = default;

	void Draw(Camera* cam);

private:

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