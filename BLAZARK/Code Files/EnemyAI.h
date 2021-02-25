#pragma once

#include "ECS.h"
#include <vector>
#include "Random.h"
#include <iostream>

class BasicAI
{
public:
	BasicAI() = default;
	BasicAI(Transform* enemy, Transform* avoid, Transform* player = nullptr, float dist = 45);

	~BasicAI() = default;

	virtual void Update();
	void SetTransform(Transform* enemy, Transform* player);
	void SetEnemyTransform(Transform* enemy);
	void SetPlayerTransform(Transform* player);

protected:
	 void GeneratePoints(Transform* avoidPlace = nullptr);
	virtual void CheckForMainPlayer();

	std::vector<glm::vec3> m_points;
	Transform* m_enemy = nullptr;
	Transform* m_player = nullptr;
	glm::vec3 m_direction;
	float distance;
	static bool initRandom;
};

class KamakaziAI : public BasicAI
{
public:
	KamakaziAI (Transform* enemy, Transform* player = nullptr);
	~KamakaziAI () = default;

	void Update() override;

private:
	void CheckForMainPlayer() override;
};

class ScavengerAI : public BasicAI
{
public:
	ScavengerAI(Transform* enemy, Transform* player = nullptr);
	~ScavengerAI() = default;
	
	void Update() override;

private:
	void CheckForMainPlayer() override;

};

class BombardierAI : public BasicAI
{
public:
	BombardierAI(Transform* enemy, Transform* player = nullptr);
	~BombardierAI() = default;

	void Update() override;

private:
	void CheckForMainPlayer() override;
	
};