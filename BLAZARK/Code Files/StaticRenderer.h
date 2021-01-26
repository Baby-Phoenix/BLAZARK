#pragma once

#include "ECS.h"
#include "OBJLoader.h"
#include "Vertex.h"
#include "Shader.h"
#include "Texture.h"

class StaticRenderer {
public:
	StaticRenderer() = default;
	StaticRenderer(GameObject* entity, const Mesh& mesh, Texture* texture = nullptr);
	virtual ~StaticRenderer() = default;

	StaticRenderer(StaticRenderer&&) = default;
	StaticRenderer& operator=(StaticRenderer&&) = default;

	void SetVAO(const Mesh& mesh);

	virtual void Draw(Shader* shader);

private:
	std::unique_ptr<VertexArray> m_vao;

	std::unique_ptr<GameObject> m_entity;

	Texture* m_tex;
};