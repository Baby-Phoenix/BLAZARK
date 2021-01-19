#pragma once
#include "Dependencies/entt/entt.hpp"
#include "Transform.h"

class GameObject
{
public:
	Transform transform;

	//constructor
	GameObject(entt::entity ID);
	GameObject(GameObject&&) = delete;

	static GameObject Create();
	static std::unique_ptr<GameObject> Allocate();

	static bool IsEmpty();

	static void SetRegistry(entt::registry* reg);

	//destructor
	virtual ~GameObject();

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
