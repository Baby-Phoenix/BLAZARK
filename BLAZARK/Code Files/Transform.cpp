#include "Transform.h"

#include "gtx/transform.hpp"

Transform::Transform()
{
	m_parent = nullptr;
	m_isLocalChanged = true;

	m_pos = glm::vec3(0.0f);
	m_scale = glm::vec3(1.0f);
	m_rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	m_rotationEulerDeg = glm::vec3(0.0f);

	m_global = glm::mat4(1.0f);
	m_local = glm::mat4(1.0f);
	m_normalMatrix = glm::mat3(1.0f);
}

Transform::~Transform()
{
	SetParent(nullptr);
}

glm::mat4& Transform::UpdateGlobal()
{

	glm::mat4 local = glm::translate(glm::mat4(1.0f), m_pos) *
		glm::toMat4(m_rotation) *
		glm::scale(glm::mat4(1.0f), m_scale);

	if (m_parent != nullptr)
		m_global = GameObject::GetComponent<Transform>(*m_parent).UpdateGlobal() * local;
	else
		m_global = local;

	return m_global;
}

glm::mat4& Transform::GetGlobal()
{
	return m_global;
}

glm::mat3 Transform::GetNormal()
{
	//If Uniform scale
	if (m_scale.x == m_scale.y && m_scale.x == m_scale.z)
		return glm::mat3(m_global);

	//If Non-Uniform scale
	return glm::inverse(glm::transpose(glm::mat3(m_global)));
}

glm::mat4& Transform::GetLocal()
{
	UpdateLocalTransform();
	return m_local;
}

void Transform::SetParent(entt::entity* parent)
{
	m_parent = parent;
}

Transform* Transform::MoveLocalPos(glm::vec3& localMovement)
{
	m_pos += m_rotation * localMovement;
	m_isLocalChanged = true;
	return this;
}

Transform* Transform::MoveLocalPosFixed(glm::vec3& localMovement)
{
	m_pos += localMovement;
	m_isLocalChanged = true;
	return this;
}

Transform* Transform::RotateLocal(glm::vec3& rotationDeg)
{
	m_rotation = m_rotation * glm::quat(glm::radians(rotationDeg));
	m_rotationEulerDeg = glm::degrees(glm::eulerAngles(m_rotation));
	m_isLocalChanged = true;
	return this;
}

Transform* Transform::RotateLocalFixed(glm::vec3& rotationDeg)
{
	m_rotation = glm::quat(glm::radians(rotationDeg)) * m_rotation;
	m_rotationEulerDeg = glm::degrees(glm::eulerAngles(m_rotation));
	m_isLocalChanged = true;
	return this;
}

Transform* Transform::SetLocalScale(glm::vec3 scale)
{
	m_scale = scale;
	m_isLocalChanged = true;
	return this;
}

Transform* Transform::SetLocalScale(float scaleX, float scaleY, float scaleZ)
{
	m_scale = glm::vec3(scaleX, scaleY, scaleZ);
	m_isLocalChanged = true;
	return this;
}

Transform* Transform::SetLocalPos(glm::vec3 pos)
{
	m_pos = pos;
	m_isLocalChanged = true;
	return this;
}

Transform* Transform::SetLocalPos(float posX, float posY, float posZ)
{
	m_pos = glm::vec3(posX, posY, posZ);
	m_isLocalChanged = true;
	return this;
}

Transform* Transform::SetLocalRot(glm::vec3 eulerDegrees)
{
	m_rotationEulerDeg = eulerDegrees;
	m_rotation = glm::quat(glm::radians(eulerDegrees));
	m_isLocalChanged = true;
	return this;
}

Transform* Transform::SetLocalRot(float eulerDegreesX, float eulerDegreesY, float eulerDegreesZ)
{
	m_rotationEulerDeg = glm::vec3(eulerDegreesX, eulerDegreesY, eulerDegreesZ);
	m_rotation = glm::quat(glm::radians(m_rotationEulerDeg));
	m_isLocalChanged = true;
	return this;
}

Transform* Transform::SetLocalRot(glm::quat rotation)
{
	m_rotation = rotation;
	return this;
}

float Transform::GetRadius()
{
	return m_radius;
}

glm::vec3 Transform::GetWHD()
{
	return glm::vec3(m_width, m_height, m_depth);
}

void Transform::SetLocalMatrix(glm::mat4 matrix)
{
	m_local = matrix;
}

void Transform::SetRadius(float radius)
{
	m_radius = radius;
}

void Transform::SetWHD(glm::vec3 whd)
{
	m_width = whd.x;
	m_height = whd.y;
	m_depth = whd.z;
}

void Transform::UpdateLocalTransform()
{
	if (m_isLocalChanged) {

		m_local = glm::translate(glm::mat4(1.0f), m_pos) * glm::toMat4(m_rotation) * glm::scale(glm::mat4(1.0f), m_scale);
		m_normalMatrix = glm::transpose(glm::inverse(glm::mat3(m_local)));

		m_isLocalChanged = false;
	}
}

