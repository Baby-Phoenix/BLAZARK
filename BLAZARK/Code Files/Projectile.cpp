#include "Projectile.h"

Projectile::Projectile(Transform& transform, entt::entity camera, static Mesh& projectileMesh)
{
	m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	m_lifetime = 3;
	m_speed = 5;
	m_isDestroyed = false;

	//Bullet
	auto bulletEntity = GameObject::Allocate();
	m_ID = bulletEntity->GetID();
	bulletEntity->AttachComponent<Transform>().SetLocalPos(transform.GetLocalPos());
	bulletEntity->GetComponent<Transform>().SetLocalScale(glm::vec3(5));
	bulletEntity->GetComponent<Transform>().SetLocalRot(transform.GetLocalRot());
	bulletEntity->AttachComponent<StaticRenderer>(camera, bulletEntity->GetID(), projectileMesh, nullptr);
}

void Projectile::SetSpeed(float speed)
{
	m_speed = speed;
}

void Projectile::SetVelocity(glm::vec3 velocity)
{
	m_velocity = velocity;
}

void Projectile::SetLifetime(float lifetime)
{
	m_lifetime = lifetime;
}

void Projectile::SetDestroyed(bool isDestroyed)
{
	m_isDestroyed = isDestroyed;
}

bool Projectile::GetDestroyed()
{
	return m_isDestroyed;
}

entt::entity Projectile::GetID()
{
	return m_ID;
}

void Projectile::Update(float deltaTime)
{
	glm::vec3 movement = m_velocity * m_speed * deltaTime;

	if (!m_isDestroyed)
	{
		GameObject::GetComponent<Transform>(m_ID).MoveLocalPos(movement);
		m_lifetime -= deltaTime;
	}
	else
		GameObject::GetComponent<StaticRenderer>(m_ID).SetisDraw(false);

	if (m_lifetime < 0.0f)
		m_isDestroyed = true;;
}
