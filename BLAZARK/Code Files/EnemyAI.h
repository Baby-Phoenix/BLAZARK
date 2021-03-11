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
	virtual void CheckForMainPlayer();

	std::vector<glm::vec3> m_points;
	unsigned int m_curPoint = 0;
	entt::entity m_enemy;
	entt::entity m_player;
	glm::vec3 m_direction;
	float m_distance = 150;
	bool m_isPlayerinRange = false;
	static bool initRandom;
};

class KamakaziAI : public BasicAI
{
public:
	KamakaziAI (Transform* enemy, Transform* player = nullptr);
	~KamakaziAI () = default;

	void Update(float deltaTime) override;

private:
	void CheckForMainPlayer() override;
};

class ScavengerAI : public BasicAI
{
public:
	ScavengerAI(Transform* enemy, Transform* player = nullptr);
	~ScavengerAI() = default;
	
	void Update(float deltaTime) override;

private:
	void CheckForMainPlayer() override;

};

class BombardierAI : public BasicAI
{
public:
	BombardierAI(Transform* enemy, Transform* player = nullptr);
	~BombardierAI() = default;

	void Update(float deltaTime) override;

private:
	void CheckForMainPlayer() override;
	
};