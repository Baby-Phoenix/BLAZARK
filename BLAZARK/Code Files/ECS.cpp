#include "ECS.h"

entt::registry* gameObject::m_reg = nullptr;

gameObject::gameObject(entt::entity ID)
{
	m_ID = ID;
}

gameObject gameObject::Create()
{
	entt::entity temp_id = m_reg->create();

	return gameObject(temp_id);
}

std::unique_ptr<gameObject> gameObject::Allocate()
{
	entt::entity temp_id = m_reg->create();

	return std::move(std::make_unique<gameObject>(temp_id));
}

void gameObject::SetRegistry(entt::registry* reg)
{
	m_reg = reg;
}


gameObject::~gameObject()
{
	if (m_ID != entt::null) {
		m_reg->destroy(m_ID);
	}
}
