#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include "glm.hpp"
#include "gtc/quaternion.hpp"

#include <vector>

class Transform
{
public:
	Transform();
	~Transform();

	//void DoFK();

	glm::mat4& UpdateGlobal();

	glm::mat4& GetGlobal();

	glm::mat3 GetNormal();
	glm::mat4& GetLocal();
	glm::vec3& GetLocalScale() { return m_scale; }
	glm::vec3& GetLocalPos() { return m_pos; }
	glm::vec3& GetLocalRot() { return m_rotationEulerDeg; }

	void SetParent(Transform* parent);

	Transform* MoveLocalPos(glm::vec3& localMovement);
	Transform* MoveLocalPosFixed(glm::vec3& localMovement);

	Transform* RotateLocal(glm::vec3& rotationDeg);
	Transform* RotateLocalFixed(glm::vec3& rotationDeg);

	Transform* SetLocalScale(glm::vec3 value);
	Transform* SetLocalScale(float scaleX, float scaleY, float scaleZ);
	Transform* SetLocalPos(glm::vec3 value);
	Transform* SetLocalPos(float posX, float posY, float posZ);
	Transform* SetLocalRot(glm::vec3 eulerDegrees);
	Transform* SetLocalRot(float eulerDegreesX, float eulerDegreesY, float eulerDegreesZ);
	float GetRadius();
	//width x, height y, depth z
	glm::vec3 GetWHD();

	void SetLocalMatrix(glm::mat4 matrix);
	void SetRadius(float radius);

	//width x, height y, depth z
	void SetWHD(glm::vec3 whd);

private:
	void AddChild(Transform* child);
	void RemoveChild(Transform* child);

	void UpdateLocalTransform();

	glm::vec3 m_pos;
	glm::vec3 m_scale;
	glm::quat m_rotation;
	glm::vec3 m_rotationEulerDeg;

	bool m_isLocalChanged;
	glm::mat4 m_local;
	glm::mat4 m_global;
	glm::mat3 m_normalMatrix;

	float m_radius = 0.f;
	float m_width, m_height, m_depth;

	Transform* m_parent;
	std::vector<Transform*> m_children;
};
