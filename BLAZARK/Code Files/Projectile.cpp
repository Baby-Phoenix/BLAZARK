#include "Projectile.h"

Projectile::Projectile(entt::entity* entityOrigin, entt::entity camera, GameObject* bulletObject, Mesh& projectileMesh)
{
	m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	m_lifetime = 3;
	m_speed = 5;
	m_isDestroyed = false;

	//Bullet
	auto origEntity = GameObject::GetComponent<Transform>(*entityOrigin);

	bulletObject->AttachComponent<Transform>().SetLocalPos(glm::vec3(origEntity.GetLocalPos().x, origEntity.GetLocalPos().y, origEntity.GetLocalPos().z));
	bulletObject->GetComponent<Transform>().SetLocalRot(origEntity.GetLocalRot().x, origEntity.GetLocalRot().y, origEntity.GetLocalRot().z);
	bulletObject->GetComponent<Transform>().SetWHD(glm::vec3(projectileMesh.GetWidth(), projectileMesh.GetHeight(), projectileMesh.GetDepth()));
	bulletObject->AttachComponent<StaticRenderer>(camera, bulletObject->GetID(), projectileMesh, nullptr, true);
}

void Projectile::SetSpeed(float speed)
{
	m_speed = speed;
}

void Projectile::SetID(entt::entity ID)
{
	m_ID = ID;
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

	if (m_lifetime < 0.0f)
		m_isDestroyed = true;
		

	if(!m_isDestroyed)
	{
		GameObject::GetComponent<Transform>(m_ID).MoveLocalPos(movement);
		m_lifetime -= deltaTime;
	}
}
