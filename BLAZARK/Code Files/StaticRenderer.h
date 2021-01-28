#pragma once

#include "ECS.h"
#include "OBJLoader.h"
#include "Vertex.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"

class StaticRenderer {
public:
	StaticRenderer() = default;
	StaticRenderer(GameObject* camera, GameObject* entity, const Mesh& mesh, Shader* shader,Texture* texture = nullptr);
	virtual ~StaticRenderer() = default;

	StaticRenderer(StaticRenderer&&) = default;
	StaticRenderer& operator=(StaticRenderer&&) = default;

	void SetVAO(const Mesh& mesh, Shader* shader);

	virtual void Draw(Shader* shader);

private:
	std::unique_ptr<VertexArray> m_vao;

	GameObject* m_camera;
	GameObject* m_entity;

	Texture* m_tex;
};