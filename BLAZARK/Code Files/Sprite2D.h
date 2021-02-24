#pragma once
#include "Shader.h"
#include "Camera.h"
#include "Application.h"
#include "Sprite2DAnimation.h"

class Sprite2D
{

public:
	
	Sprite2D(Texture* tex, entt::entity entity, float width = 1, float height = 1, bool isAnim = false, AnimationHandler* anim = nullptr, float transparency = 1);
	virtual	~Sprite2D() = default;

	void SetAnimationHandler(AnimationHandler* anim);

	void Draw(Camera* cam);

private:

	float m_width;
	float m_height;
	float m_transparency;

	static Mesh* m_planeMesh;
	VertexArray* VAO = nullptr;
	static Shader* m_Sprite2D_shader[2];

	bool m_isAnimated = false;

	AnimationHandler* Animation = nullptr;

	Texture* m_texture;
	entt::entity m_entity;
};
