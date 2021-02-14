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
	StaticRenderer(entt::entity camera, entt::entity entity, const Mesh& mesh,Texture* texture = nullptr);
	virtual ~StaticRenderer() = default;

	StaticRenderer(StaticRenderer&&) = default;
	StaticRenderer& operator=(StaticRenderer&&) = default;

	void SetVAO(const Mesh& mesh);

	void toggleTexture();

	virtual void Draw();

private:
	std::unique_ptr<VertexArray> m_vao;
	static std::vector<Shader*> m_shader;
	int currShader;
	entt::entity m_camera;
	entt::entity m_entity;
	Texture* m_tex;
	bool textureToggle = true;
};