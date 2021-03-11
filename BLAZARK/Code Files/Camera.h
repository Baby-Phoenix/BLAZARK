#pragma once

#include "ECS.h"

class Camera {
public:
	Camera() = default;
	Camera(entt::entity entity);
	virtual ~Camera() = default;

	void PerspectiveProj(float near, float far, float aspect, float FoVY);
	void OrthographicProj(float near, float far, float left, float right, float bottom, float top);
	Transform& GetTransform();

	void Update();
	void Update(entt::entity);

	// Getters //
	const glm::mat4& GetViewProj();
	const glm::mat4& GetView();
	const glm::mat4& GetProj();

	// Setters //
	void SetViewProj(glm::mat4);
	void SetView(glm::mat4);
	void SetProj(glm::mat4);

private:
	glm::mat4 m_view;
	glm::mat4 m_proj;
	glm::mat4 m_viewProj;

	entt::entity m_entity;
};