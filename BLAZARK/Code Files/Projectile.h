#pragma once

#include "ECS.h"
#include "Mesh.h"
#include "StaticRenderer.h"

class Projectile
{
public:
	Projectile(entt::entity* entityOrigin, entt::entity camera, static Mesh& projectileMesh);

	void SetSpeed(float speed);
	void SetVelocity(glm::vec3 velocity);
	void SetLifetime(float lifetime);
	void SetDestroyed(bool isDestroyed);

	bool GetDestroyed();
	entt::entity GetID();

	void Update(float deltaTime);

private:
	float m_lifetime;
	float m_speed;

	entt::entity m_ID;
	bool m_isDestroyed;

	glm::vec3 m_velocity;
};