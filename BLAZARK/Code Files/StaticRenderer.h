#pragma once

#include "ECS.h"
#include "Vertex.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "GLTFLoader.h"

class StaticRenderer {
public:
	StaticRenderer() = default;
	StaticRenderer(GameObject* camera, GameObject* entity, const Mesh& mesh,Texture* texture = nullptr);
	virtual ~StaticRenderer() = default;

	StaticRenderer(StaticRenderer&&) = default;
	StaticRenderer& operator=(StaticRenderer&&) = default;

	void SetVAO(const Mesh& mesh);

	virtual void Draw();

private:
	std::unique_ptr<VertexArray> m_vao;
	static Shader* m_static_shader;
	GameObject* m_camera;
	GameObject* m_entity;

	Texture* m_tex;
};