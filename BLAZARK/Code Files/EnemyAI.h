#pragma once

#include "ECS.h"
#include <vector>
#include "Random.h"

class BasicAI
{
public:
	BasicAI();
	~BasicAI() = default;

	virtual void Update();

protected:
	virtual void GeneratePoints();
	virtual void CheckForMainPlayer();

	std::vector<glm::vec3> m_points;
	Transform* m_enemy = nullptr;
	Transform* m_player = nullptr;
	glm::vec3 m_direction;
};

class KamakaziAI : public BasicAI
{
public:
	KamakaziAI ();
	~KamakaziAI () = default;

	void Update() override;

private:
	void CheckForMainPlayer() override;
	void GeneratePoints() override;
};

class ScavengerAI : public BasicAI
{
public:
	ScavengerAI();
	~ScavengerAI() = default;
	
	void Update() override;

private:
	void CheckForMainPlayer() override;
	void GeneratePoints() override;

};

class BombardierAI : public BasicAI
{
public:
	BombardierAI();
	~BombardierAI() = default;

	void Update() override;

private:
	void CheckForMainPlayer() override;
	void GeneratePoints() override;
};