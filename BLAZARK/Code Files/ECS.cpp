#include "ECS.h"

entt::registry* GameObject::m_reg = nullptr;

GameObject::GameObject(entt::entity ID)
{
	m_ID = ID;
}

GameObject GameObject::Create()
{
	entt::entity temp_id = m_reg->create();

	return GameObject(temp_id);
}

std::unique_ptr<GameObject> GameObject::Allocate()
{
	entt::entity temp_id = m_reg->create();

	return std::move(std::make_unique<GameObject>(temp_id));
}

bool GameObject::IsEmpty()
{
	return m_reg->empty();
}

void GameObject::SetRegistry(entt::registry* reg)
{
	m_reg = reg;
}


GameObject::~GameObject()
{
	if (m_ID != entt::null) {
		m_reg->destroy(m_ID);
	}
}
