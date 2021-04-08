#pragma once

#include "ECS.h"
#include <vector>
#include "Random.h"
#include <iostream>
#include "Projectile.h"
#include "ParticleSystem.h"
#include "MorphAnimController.h"

enum class EntityType { PLAYER, ENEMY, NEROTIST, KAMIKAZI, SCAVENGER, BOMBARDIER, KAMIBULLET, JELLY, CENTIPEDE, HIVEMIND, PORTAL};

class BasicAI
{
public:
	BasicAI() = default;
	BasicAI(entt::entity enemy, entt::entity avoid,entt::entity player = entt::entity(1), float dist = 150);

	~BasicAI() = default;

	virtual void Update(float deltaTime);
	virtual entt::entity GetID();
	void SetBulletMesh(Mesh* bulletMesh);
	void SetTransform(entt::entity enemy, entt::entity player);
	void SetEnemyTransform(entt::entity enemy);
	void SetPlayerTransform(entt::entity player);

	int m_health = 0;
	bool m_isImmune = true;

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

	//for bullet firing
	float m_fireRate = 0.5f, m_startTime = 0.0f;
	bool m_resetTime = false;
	static Mesh* m_bulletMesh;
};

class KamikaziAI : public BasicAI
{
public:
	KamikaziAI (entt::entity enemy, entt::entity avoid, entt::entity player);
	~KamikaziAI () = default;

	void Update(float deltaTime) override;

private:
	
};

class KamakaziBullet : public BasicAI {

public:
	KamakaziBullet(entt::entity enemy, entt::entity player);
	void Update(float deltaTime) override;
	bool GetDestroyed();
	void SetSpeed(float speed);
	void SetDestroyed(bool destroyed);
private:
	bool m_isDestroyed = false;
	float m_lifetime = 10.0f;
	float m_speed = 50.0f;
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

class JellyFishBoss : public BasicAI{

public:
	JellyFishBoss() = default;
	~JellyFishBoss();

	static void Init();
	static void DeleteMeshes();

	void Init(entt::entity jellyEntityID, entt::entity MainplayerID);
	void Update(float deltaTime) override;

	int m_MAX_ENEMIES_SPAWNED = 0;
	float m_Enemy_SpawnRate = 1.0;
	float m_startEnemyTime = 0.0f;
	bool resetEnemyTime = false;
	static std::vector<Mesh*> m_meshes;
	Mesh* m_enemyMesh = nullptr;
	std::vector<ParticleController*> m_particles;

};

class CentipedeBoss : public BasicAI{

public:
	CentipedeBoss() = default;
	~CentipedeBoss() = default;

	void Init(entt::entity jellyEntityID, entt::entity MainplayerID);

	static void Init();
	static void DeleteMeshes();
	void Update(float deltaTime) override;

	static std::vector<Mesh*> m_meshes;
	
private:
	float m_speed = 0.1f;
	bool m_flip = false;

	bool m_resetTimeShot[3] = { false,false, false };
	float m_fireRatePerhead[3] = { 0.6f, 0.6f, 0.6f };
	float m_startTimePerhead[3] = { 0.f, 0.f, 0.f };

};

class HiveMindBoss : public BasicAI {

public:

	HiveMindBoss()= default;
	~HiveMindBoss();

	int Phases();

	void Init(entt::entity HiveMindEntityID, entt::entity MainplayerID);
	void JellyFishDefeated();
	void CentipedeDefeated();

	void Update(float deltaTime) override;

	static void Init();

	static std::vector<Mesh*> m_meshes;

	bool m_JellySpawned = true;
	bool m_CentipedeSpawned = true;

private:
	//the 3 phases for the final boss
	int m_phases = 0;

	float m_startTimeBetweenBoss = 0.0f;
	float m_DelayBetweenBoss = 2.5f;
	bool m_bossPhaseSwitch = true;

	//for jelly fish boss start and end
	bool m_JellyDefeat = false;

	//for the Centipede boss start and end
	bool m_CentipedeDefeat = false;

};