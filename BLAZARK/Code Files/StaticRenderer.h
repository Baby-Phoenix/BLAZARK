#pragma once

#include "ECS.h"
#include "Mesh.h"
#include "Vertex.h"

class StaticRenderer {
public:
	StaticRenderer() = default;
	StaticRenderer(GameObject& entity, const Mesh& mesh);
	virtual ~StaticRenderer() = default;

	StaticRenderer(StaticRenderer&&) = default;
	StaticRenderer& operator=(StaticRenderer&&) = default;

	void SetVAO(const Mesh& mesh);

	virtual void Draw();

private:
	std::unique_ptr<VertexArray> m_vao;

	GameObject* m_entity;
};