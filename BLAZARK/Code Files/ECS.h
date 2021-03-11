#pragma once
#include "Dependencies/entt/entt.hpp"
#include "Transform.h"

class GameObject
{
public:

	//constructor
	GameObject(entt::entity ID);
	GameObject(GameObject&&) = delete;

	static GameObject Create();
	static std::unique_ptr<GameObject> Allocate();
	static bool IsEmpty();
	static void SetRegistry(entt::registry* reg);
	static entt::registry& GetCurRegistry() { return *m_reg; }
	
	//attaches a component
	template<typename T, typename... Args>
	static T& AttachComponent(entt::entity ID)
	{
		return m_reg->emplace<T>(ID);
	}

	template<typename T>
	static T& GetComponent(entt::entity id) 
	{
		return m_reg->get<T>(id);
	}

	void DestroyEntity();
	entt::entity GetID();

	//destructor
	virtual ~GameObject();

	//attaches a component
	template<typename T, typename... Args>
	T& AttachComponent(Args&&... args)
	{
		return m_reg->emplace<T>(m_ID, std::forward<Args>(args)...);
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
