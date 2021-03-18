#include "Camera.h"

Camera::Camera(unsigned int entity) {
	m_entity = entt::entity(entity);
}

void Camera::PerspectiveProj(float near, float far, float aspect, float FoVY) {
	m_proj = glm::perspective(glm::radians(FoVY), aspect, near, far);
	Update();
}

void Camera::OrthographicProj(float near, float far, float left, float right, float bottom, float top) {
	m_proj = glm::ortho(left, right, bottom, top, near, far);
	Update();
}

Transform& Camera::GetTransform()
{
	return GameObject::GetComponent<Transform>(m_entity);
}

void Camera::Update() {
	m_view = glm::inverse(GameObject::GetComponent<Transform>(m_entity).UpdateGlobal());
	m_viewProj = m_proj * m_view;
}

void Camera::Update(entt::entity mainPlayer) {
	m_view = glm::lookAt(GetTransform().GetLocalPos(), GameObject::GetComponent<Transform>(mainPlayer).GetLocalPos() + glm::vec3(0, 0, -10), glm::vec3(0, 1, 0));
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

const glm::mat4& Camera::GetProj()
{
	return m_proj;
}

void Camera::SetViewProj(glm::mat4 viewProj)
{
	m_viewProj = viewProj;
}

void Camera::SetView(glm::mat4 view)
{
	m_view = view;
}

void Camera::SetProj(glm::mat4 proj)
{
	m_proj = proj;
}
