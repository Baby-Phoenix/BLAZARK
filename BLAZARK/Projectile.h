#pragma once
#include "glm.hpp"

class Projectile
{
public:
	Projectile();
	~Projectile();

	void SetSpeed(float speed);
	void SetVelocity(glm::vec3 velocity);
	void SetLifetime(float lifetime);
	void SetDestroyed(bool isDestroyed);
	void SetID(bool id);
	bool GetDestroyed();
	bool GetId();

	void Update(float dt);

private:
	bool m_ID;
	glm::vec3 m_velocity;
	float m_speed;
	float m_lifetime;
	bool m_isDestroyed;
};