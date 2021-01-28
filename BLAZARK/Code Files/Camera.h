#pragma once

#include "ECS.h"

class Camera {
public:
	Camera(GameObject* entity);
	virtual ~Camera() = default;

	void PerspectiveProj(float near, float far, float aspect, float FoVY);
	void OrthographicProj(float near, float far, float left, float right, float bottom, float top);

	void Update();

	// Getters //
	const glm::mat4& GetViewProj();
	const glm::mat4& GetView();
	const glm::mat4& GetProj();


private:
	glm::mat4 m_view;
	glm::mat4 m_proj;
	glm::mat4 m_viewProj;

	GameObject* m_entity;
};