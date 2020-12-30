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
	Transform* SetLocalPos(glm::vec3 value);
	Transform* SetLocalRot(glm::vec3 eulerDegrees);

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

	Transform* m_parent;
	std::vector<Transform*> m_children;
};
