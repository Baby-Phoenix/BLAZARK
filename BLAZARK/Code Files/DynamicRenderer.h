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
		//SPECULARSHININESS1,
		AMBIENTCOLOUR0,
		//AMBIENTCOLOUR1,
		DIFFUSECOLOUR0,
		//DIFFUSECOLOUR1,
		SPECULARCOLOUR0,
		//SPECULARCOLOUR1,
		DISSOLVE0,
		//DISSOLVE1
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