#pragma once

#include "StaticRenderer.h"

class DynamicRenderer : StaticRenderer {
public:
	enum class VertexAttrib {
		POSITION0,
		POSITION1,
		NORMAL0,
		NORMAL1,
		TEXCOORD,
		SPECULARSHININESS0,
		AMBIENTCOLOUR0,
		DIFFUSECOLOUR0,
		SPECULARCOLOUR0,
		DISSOLVE0,
	};

	DynamicRenderer() = default;
	DynamicRenderer(entt::entity camera, entt::entity entity, const Mesh& mesh, Texture* texture = nullptr, bool lightSource = false);
	virtual ~DynamicRenderer() = default;

	DynamicRenderer(DynamicRenderer&&) = default;
	DynamicRenderer& operator=(DynamicRenderer&&) = default;

	void UpdateVAO(const Mesh& frameS, const Mesh& frameE, float t);

	virtual void Draw();

protected:
	float m_t;

	Mesh* m_frameS;
	Mesh* m_frameE;
};