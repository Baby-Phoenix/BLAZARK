#include "Camera.h"

Camera::Camera(GameObject* entity) {
	m_entity = entity;
}

void Camera::PerspectiveProj(float near, float far, float aspect, float FoVY) {
	m_proj = glm::perspective(glm::radians(FoVY), aspect, near, far);
	Update();
}

void Camera::OrthographicProj(float near, float far, float left, float right, float bottom, float top) {
	m_proj = glm::ortho(left, right, bottom, top, near, far);
	Update();
}

void Camera::Update() {
	m_view = glm::inverse(m_entity->GetComponent<Transform>().UpdateGlobal());
	m_viewProj = m_proj * m_view;
}

const glm::mat4& Camera::GetViewProj()
{
	return m_viewProj;
}

const glm::mat4& Camera::GetView()
{
	return m_view;
}
