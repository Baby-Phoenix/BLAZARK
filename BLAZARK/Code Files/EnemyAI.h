#pragma once

#include "ECS.h"
#include <vector>
#include "Random.h"
#include <iostream>

class BasicAI
{
public:
	BasicAI() = default;
	BasicAI(entt::entity enemy, entt::entity avoid, entt::entity player = entt::entity(1), float dist = 150);

	~BasicAI() = default;

	virtual void Update(float deltaTime);
	void SetTransform(entt::entity enemy, entt::entity player);
	void SetEnemyTransform(entt::entity enemy);
	void SetPlayerTransform(entt::entity player);

protected:
	void GeneratePoints(Transform avoidPlace);
	void CheckForMainPlayer();

	std::vector<glm::vec3> m_points;
	unsigned int m_curPoint = 0;
	entt::entity m_enemy;
	entt::entity m_player;
	float m_distance = 150;
	bool m_isPlayerinRange = false;
	static bool initRandom;
};

class KamakaziAI : public BasicAI
{
public:
	KamakaziAI (entt::entity enemy, entt::entity avoid, entt::entity player);
	~KamakaziAI () = default;

	void Update(float deltaTime) override;

private:
	
};

class ScavengerAI : public BasicAI
{
public:
	ScavengerAI(entt::entity enemy, entt::entity avoid, entt::entity player);
	~ScavengerAI() = default;
	
	void Update(float deltaTime) override;
	
};

class BombardierAI : public BasicAI
{
public:
	BombardierAI(entt::entity enemy, entt::entity avoid, entt::entity player);
	~BombardierAI() = default;

	void Update(float deltaTime) override;

};