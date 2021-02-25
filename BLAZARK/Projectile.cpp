#include "Projectile.h"

Projectile::Projectile()
{
	m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	m_lifetime = 3;
	m_speed = 5;
	m_isDestroyed = false;
	//SetPosition(position);
}

Projectile::~Projectile()
{

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

void Projectile::SetID(bool id)
{
	m_ID = id;
}

bool Projectile::GetDestroyed()
{
	return m_isDestroyed;
}

bool Projectile::GetId()
{
	return m_ID;
}

void Projectile::Update(float dt)
{
	//this->SetPosition(this->GetPosition() += this->velocity * this->speed * dt);

	m_lifetime -= dt;

	if (m_lifetime < 0.0f)
		m_isDestroyed = true;;
}
