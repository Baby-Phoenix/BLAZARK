#pragma once
#include "Dependencies/entt/entt.hpp"

class gameObject
{
public:

	//constructor
	gameObject(entt::entity ID);
	gameObject(gameObject&&) = delete;

	static gameObject Create();
	static std::unique_ptr<gameObject> Allocate();

	static void SetRegistry(entt::registry* reg);

	//destructor
	virtual ~gameObject();

	//attaches a component
	template<typename T, typename... Args>
	T& AttachComponent(Args&&... args)
	{
		return m_reg->emplace<T>(m_ID, std::forward<Args>(args)...);
	}

	template<typename T>
	void AttachComponent() {
		m_reg->assign<T>(m_ID);
	}

	//returns the component
	template<typename T>
	T& GetComponent()
	{
		return m_reg->get<T>(m_ID);
	}

	template<typename T>
	void RemoveComponent()
	{
		m_reg->remove<T>(m_ID);
	}
private:

	//the entity's id
	entt::entity m_ID;

	//the registry
	static entt::registry* m_reg;
};
