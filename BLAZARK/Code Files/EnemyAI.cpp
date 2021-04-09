#include "EnemyAI.h"

bool BasicAI::initRandom = false;
Mesh* BasicAI::m_bulletMesh = nullptr;
std::vector<Mesh*> JellyFishBoss::m_meshes;
std::vector<Mesh*> CentipedeBoss::m_meshes;
std::vector<Mesh*> HiveMindBoss::m_meshes;

glm::vec3 Lerp(glm::vec3 point1, glm::vec3 point2, float t) {
	return ((float(1.0 - t) * point1) + (t * point2));
}

BasicAI::BasicAI(entt::entity enemy, entt::entity avoid, entt::entity player, float dist)
{
	if (!initRandom) {
		Random::Init();
	}
	SetTransform(enemy, player);
	GeneratePoints(GameObject::GetComponent<Transform>(avoid));

	m_distance = dist;
}


void BasicAI::Update(float deltaTime)
{
	CheckForMainPlayer();

	auto& enemyTrans = GameObject::GetComponent<Transform>(m_enemy);

	glm::vec3 curPoint;
	glm::vec3 nextPoint;
	glm::vec3 curPosOfEnemy;

	AudioEngine& engine = AudioEngine::Instance();
	AudioEvent& shoot = engine.GetEvent("Enemy Shooting");

	if (m_isPlayerinRange) {

#pragma region movement
		curPoint = m_points[m_curPoint];  //curpoint the enemy started from
		nextPoint = GameObject::GetComponent<Transform>(m_player).GetLocalPos(); //current position of the player
		curPosOfEnemy = enemyTrans.GetLocalPos(); //current position

		float totalDist = glm::sqrt(((nextPoint.x - curPoint.x) * (nextPoint.x - curPoint.x)) + ((nextPoint.z - curPoint.z) * (nextPoint.z - curPoint.z)));
		float curDist = glm::sqrt(((curPosOfEnemy.x - nextPoint.x) * (curPosOfEnemy.x - nextPoint.x)) + ((curPosOfEnemy.z - nextPoint.z) * (curPosOfEnemy.z - nextPoint.z)));

		float t = curDist / totalDist; //0 - 1 interpolation paramenter 

		glm::vec3 finalPos = glm::normalize(nextPoint - curPosOfEnemy) * (deltaTime * 50);


		if (t > 0.3)
			enemyTrans.MoveLocalPosFixed(finalPos);

#pragma endregion

#pragma region BulletShooting

		if (glfwGetTime() - m_startTime >= m_fireRate) {
			
			if (!shoot.isPlaying())
				shoot.Play();
			else
			{
				shoot.Stop();
				shoot.Play();
			}

			auto bullet = GameObject::Allocate();
			bullet->AttachComponent<Projectile>(&m_enemy, entt::entity(0), bullet.get(), *m_bulletMesh).SetID(bullet->GetID());
			bullet->GetComponent<Projectile>().SetSpeed(100);
			bullet->AttachComponent<EntityType>() = EntityType::ENEMY;
			bullet->GetComponent<Projectile>().SetVelocity(glm::vec3(0, 0, -1));
			glm::vec3 offset1 = glm::vec3(0, 0, -10);
			bullet->GetComponent<Transform>().MoveLocalPos(offset1);
			bullet->GetComponent<Transform>().SetLocalScale(glm::vec3(3.0));
			m_resetTime = true;
		}
		if (m_resetTime) {
			m_startTime = glfwGetTime();
			m_resetTime = false;
		}

#pragma endregion
	}
	else
	{

		curPoint = m_points[m_curPoint];
		nextPoint = m_points[m_curPoint == m_points.size() - 1 ? 0 : m_curPoint + 1];
		curPosOfEnemy = enemyTrans.GetLocalPos();

		float totalDist = glm::sqrt(((nextPoint.x - curPoint.x) * (nextPoint.x - curPoint.x)) + ((nextPoint.z - curPoint.z) * (nextPoint.z - curPoint.z)));
		float curDist = glm::sqrt(((curPosOfEnemy.x - curPoint.x) * (curPosOfEnemy.x - curPoint.x)) + ((curPosOfEnemy.z - curPoint.z) * (curPosOfEnemy.z - curPoint.z)));

		float t = curDist / totalDist; // 0 - 1 interpolation paramenter 

		glm::vec3 finalPos = glm::normalize(nextPoint - curPosOfEnemy) * (deltaTime * 40);
		enemyTrans.MoveLocalPosFixed(finalPos);

		if (t > 1.0) {
			m_curPoint++;
			if (m_curPoint > m_points.size() - 1)
				m_curPoint = 0;
		}
	}


	
		glm::vec3 rotationvector = glm::degrees(glm::eulerAngles(glm::quat_cast(glm::transpose(glm::lookAt(curPosOfEnemy, nextPoint, glm::vec3(0, 1, 0))))));
		enemyTrans.SetLocalRot(rotationvector);
	
}

entt::entity BasicAI::GetID()
{
	return m_enemy;
}

void BasicAI::SetBulletMesh(Mesh* bulletMesh)
{
	if (m_bulletMesh == nullptr) {
		m_bulletMesh = bulletMesh;
	}
}

void BasicAI::SetTransform(entt::entity enemy, entt::entity player)
{
	SetEnemyTransform(enemy);
	SetPlayerTransform(player);
}

void BasicAI::SetEnemyTransform(entt::entity enemy)
{
	m_enemy = enemy;
}

void BasicAI::SetPlayerTransform(entt::entity player)
{
	m_player = player;
}

void BasicAI::GeneratePoints(Transform avoidPlace)
{
	//gets number of points
	int numberOfPoints = Random::Range1f(2, 5);

	m_points.resize(numberOfPoints);

	//gets the innitial point
	glm::vec3 randomPoint = Random::GetPointBetween(avoidPlace.GetLocalPos(), avoidPlace.GetRadius());
	m_points[0] = randomPoint;
	GameObject::GetComponent<Transform>(m_enemy).SetLocalPos(m_points[0]);	

	//gets the rest of the points
	for (unsigned i = 1; i < numberOfPoints; i++) {
		
		glm::vec3 direction = glm::normalize(m_points[i - 1] - avoidPlace.GetLocalPos());
		
		float angle = Random::Range1f(-90, 90);
		glm::vec2 dist = Random::Range2f(10, 100); //(15,30)

		//final direction
		glm::vec3 finalDirection = glm::mat3(glm::vec3(glm::cos(angle), 0, glm::sin(angle)), 
											 glm::vec3(0, 1, 0),
											 glm::vec3(-glm::sin(angle), 0, glm::cos(angle))) * direction;

		m_points[i] = glm::vec3((finalDirection.x + dist.x) + m_points[i - 1].x, 0, (finalDirection.z + dist.y) + m_points[i - 1].z);
	}

}

void BasicAI::CheckForMainPlayer()
{
	Transform& enemyTrans = GameObject::GetComponent<Transform>(m_enemy);
	glm::vec3 enemyPos = enemyTrans.GetLocalPos();

	glm::vec3 playerPos = GameObject::GetComponent<Transform>(m_player).GetLocalPos();

	//check the distance between the player and this enemy
	float compareDist = 
		glm::sqrt(((enemyPos.x - playerPos.x) * (enemyPos.x - playerPos.x)) + 
								  ((enemyPos.z - playerPos.z) * (enemyPos.z - playerPos.z)));

	if (compareDist <= m_distance && !m_isPlayerinRange)
		m_isPlayerinRange = true;
	

	else if(compareDist > m_distance && m_isPlayerinRange)
		m_isPlayerinRange = false;
}

KamikaziAI::KamikaziAI(entt::entity enemy, entt::entity avoid, entt::entity player)
	:BasicAI(enemy,avoid, player, 300)
{
}

void KamikaziAI::Update(float deltaTime)
{
	CheckForMainPlayer();

	auto& enemyTrans = GameObject::GetComponent<Transform>(m_enemy);

	glm::vec3 curPoint;
	glm::vec3 nextPoint;
	glm::vec3 curPosOfEnemy;
	
	if (m_isPlayerinRange) {

		nextPoint = GameObject::GetComponent<Transform>(m_player).GetLocalPos(); //current position of the player
		curPosOfEnemy = enemyTrans.GetLocalPos(); //current position

		glm::vec3 finalPos = glm::normalize(nextPoint - curPosOfEnemy) * (deltaTime * 50);

		enemyTrans.MoveLocalPosFixed(finalPos);

	}
	else
	{

		curPoint = m_points[m_curPoint];
		nextPoint = m_points[m_curPoint == m_points.size() - 1 ? 0 : m_curPoint + 1];
		curPosOfEnemy = enemyTrans.GetLocalPos();

		float totalDist = glm::sqrt(((nextPoint.x - curPoint.x) * (nextPoint.x - curPoint.x)) + ((nextPoint.z - curPoint.z) * (nextPoint.z - curPoint.z)));
		float curDist = glm::sqrt(((curPosOfEnemy.x - curPoint.x) * (curPosOfEnemy.x - curPoint.x)) + ((curPosOfEnemy.z - curPoint.z) * (curPosOfEnemy.z - curPoint.z)));

		float t = curDist / totalDist; // 0 - 1 interpolation paramenter 

		glm::vec3 finalPos = glm::normalize(nextPoint - curPosOfEnemy) * (deltaTime * 40);
		enemyTrans.MoveLocalPosFixed(finalPos);

		if (t > 1.0) {
			m_curPoint++;
			if (m_curPoint > m_points.size() - 1)
				m_curPoint = 0;
		}
	}

		glm::vec3 rotationvector = glm::degrees(glm::eulerAngles(glm::quat_cast(glm::transpose(glm::lookAt(curPosOfEnemy, nextPoint, glm::vec3(0, 1, 0))))));
		enemyTrans.SetLocalRot(rotationvector);
	
}


ScavengerAI::ScavengerAI(entt::entity enemy, entt::entity avoid, entt::entity player)
	:BasicAI(enemy, avoid, player, 500)
{
}

void ScavengerAI::Update(float deltaTime)
{
	CheckForMainPlayer();

	auto& enemyTrans = GameObject::GetComponent<Transform>(m_enemy);

	glm::vec3 curPoint;
	glm::vec3 nextPoint;
	glm::vec3 curPosOfEnemy;

	if (m_isPlayerinRange) {

	
	}

		curPoint = m_points[m_curPoint];
		nextPoint = m_points[m_curPoint == m_points.size() - 1 ? 0 : m_curPoint + 1];
		curPosOfEnemy = enemyTrans.GetLocalPos();

		float totalDist = glm::sqrt(((nextPoint.x - curPoint.x) * (nextPoint.x - curPoint.x)) + ((nextPoint.z - curPoint.z) * (nextPoint.z - curPoint.z)));
		float curDist = glm::sqrt(((curPosOfEnemy.x - curPoint.x) * (curPosOfEnemy.x - curPoint.x)) + ((curPosOfEnemy.z - curPoint.z) * (curPosOfEnemy.z - curPoint.z)));

		float t = curDist / totalDist; // 0 - 1 interpolation paramenter 

		glm::vec3 finalPos = glm::normalize(nextPoint - curPosOfEnemy) * (deltaTime * 40);
		enemyTrans.MoveLocalPosFixed(finalPos);

		if (t > 1.0) {
			m_curPoint++;
			if (m_curPoint > m_points.size() - 1)
				m_curPoint = 0;
		}
	

		glm::vec3 rotationvector = glm::degrees(glm::eulerAngles(glm::quat_cast(glm::transpose(glm::lookAt(curPosOfEnemy, nextPoint, glm::vec3(0, 1, 0))))));
		enemyTrans.SetLocalRot(rotationvector);
}

BombardierAI::BombardierAI(entt::entity enemy, entt::entity avoid, entt::entity player)
	:BasicAI(enemy, avoid, player, 300)
{
	m_fireRate = 2.5f;
}

void BombardierAI::Update(float deltaTime)
{

	CheckForMainPlayer();

	AudioEngine& engine = AudioEngine::Instance();
	AudioEvent& shoot = engine.GetEvent("Enemy Shooting");

	auto& enemyTrans = GameObject::GetComponent<Transform>(m_enemy);

	glm::vec3 curPoint;
	glm::vec3 nextPoint;
	glm::vec3 curPosOfEnemy;

	if (m_isPlayerinRange) {

#pragma region movement
		curPoint = m_points[m_curPoint];  //curpoint the enemy started from
		nextPoint = GameObject::GetComponent<Transform>(m_player).GetLocalPos(); //current position of the player
		curPosOfEnemy = enemyTrans.GetLocalPos(); //current position

		float totalDist = glm::sqrt(((nextPoint.x - curPoint.x) * (nextPoint.x - curPoint.x)) + ((nextPoint.z - curPoint.z) * (nextPoint.z - curPoint.z)));
		float curDist = glm::sqrt(((curPosOfEnemy.x - nextPoint.x) * (curPosOfEnemy.x - nextPoint.x)) + ((curPosOfEnemy.z - nextPoint.z) * (curPosOfEnemy.z - nextPoint.z)));

		float t = curDist / totalDist; //0 - 1 interpolation paramenter 

		glm::vec3 finalPos = glm::normalize(nextPoint - curPosOfEnemy) * (deltaTime * 50);


		if (t > 0.5)
			enemyTrans.MoveLocalPosFixed(finalPos);

#pragma endregion

#pragma region BulletShooting

		if (glfwGetTime() - m_startTime >= m_fireRate) {
			if (!shoot.isPlaying())
				shoot.Play();
			else
			{
				shoot.Stop();
				shoot.Play();
			}

			auto kamabullet = GameObject::Allocate();
			kamabullet->AttachComponent<Transform>().SetLocalPos(GameObject::GetComponent<Transform>(m_enemy).GetLocalPos());
			kamabullet->GetComponent<Transform>().SetLocalScale(glm::vec3(5));
			kamabullet->AttachComponent<KamakaziBullet>(kamabullet->GetID(), m_player).SetSpeed(25);
			kamabullet->AttachComponent<EntityType>() = EntityType::KAMIBULLET;
			kamabullet->AttachComponent<StaticRenderer>(entt::entity(0), kamabullet->GetID(), *m_bulletMesh, nullptr);
			kamabullet->GetComponent<Transform>().SetWHD(glm::vec3(m_bulletMesh->GetWidth() * 2, m_bulletMesh->GetHeight() * 2, m_bulletMesh->GetDepth() * 2));

			m_resetTime = true;
		}
		if (m_resetTime) {
			m_startTime = glfwGetTime();
			m_resetTime = false;
		}

#pragma endregion
	}
	else
	{

		curPoint = m_points[m_curPoint];
		nextPoint = m_points[m_curPoint == m_points.size() - 1 ? 0 : m_curPoint + 1];
		curPosOfEnemy = enemyTrans.GetLocalPos();

		float totalDist = glm::sqrt(((nextPoint.x - curPoint.x) * (nextPoint.x - curPoint.x)) + ((nextPoint.z - curPoint.z) * (nextPoint.z - curPoint.z)));
		float curDist = glm::sqrt(((curPosOfEnemy.x - curPoint.x) * (curPosOfEnemy.x - curPoint.x)) + ((curPosOfEnemy.z - curPoint.z) * (curPosOfEnemy.z - curPoint.z)));

		float t = curDist / totalDist; // 0 - 1 interpolation paramenter 

		glm::vec3 finalPos = glm::normalize(nextPoint - curPosOfEnemy) * (deltaTime * 40);
		enemyTrans.MoveLocalPosFixed(finalPos);

		if (t > 1.0) {
			m_curPoint++;
			if (m_curPoint > m_points.size() - 1)
				m_curPoint = 0;
		}
	}



	glm::vec3 rotationvector = glm::degrees(glm::eulerAngles(glm::quat_cast(glm::transpose(glm::lookAt(curPosOfEnemy, nextPoint, glm::vec3(0, 1, 0))))));
	enemyTrans.SetLocalRot(rotationvector);



}

KamakaziBullet::KamakaziBullet(entt::entity enemy, entt::entity player)
{
	m_enemy = enemy;
	m_player = player;
}

void KamakaziBullet::Update(float deltaTime)
{

	if (!m_isDestroyed) {
		auto& enemyTrans = GameObject::GetComponent<Transform>(m_enemy);

		glm::vec3 curPoint;
		glm::vec3 nextPoint;
		glm::vec3 curPosOfEnemy;

		nextPoint = GameObject::GetComponent<Transform>(m_player).GetLocalPos(); //current position of the player
		curPosOfEnemy = enemyTrans.GetLocalPos(); //current position

		glm::vec3 finalPos = glm::normalize(nextPoint - curPosOfEnemy) * (deltaTime * m_speed);

		enemyTrans.MoveLocalPosFixed(finalPos);


		glm::vec3 rotationvector = glm::degrees(glm::eulerAngles(glm::quat_cast(glm::transpose(glm::lookAt(curPosOfEnemy, nextPoint, glm::vec3(0, 1, 0))))));
		enemyTrans.SetLocalRot(rotationvector);

		m_lifetime -= deltaTime;

		if (m_lifetime <= 0) {
			m_isDestroyed = true;
		}
	}
}

bool KamakaziBullet::GetDestroyed()
{
	return m_isDestroyed;
}

void KamakaziBullet::SetSpeed(float speed)
{
	m_speed = speed;
}

void KamakaziBullet::SetDestroyed(bool destroyed)
{
	m_isDestroyed = destroyed;
}


JellyFishBoss::~JellyFishBoss()
{
	for (int i = 0; i < m_particles.size(); i++) {
		m_particles[i]->getEmitter()->SetDone();
	}
}

void JellyFishBoss::Init()
{
	for (int i = 0; i < 25; i++) {
		m_meshes.push_back(new Mesh());
		loadOBJ(("Resource Files/OBJFiles/Universe-19/EnemyShips/Morph/Boss/jellyfishBoss_" + std::to_string(i) + ".obj").c_str(), *m_meshes[i]);
	}

}

void JellyFishBoss::DeleteMeshes()
{
	for (int i = 0; i < m_meshes.size(); i++) {
		delete m_meshes[i];
		m_meshes.erase(m_meshes.begin() + i);
	}
}

void JellyFishBoss::Init(entt::entity jellyEntityID, entt::entity MainplayerID)
{
	m_enemy = jellyEntityID;
	m_player = MainplayerID;
	m_distance = 550;
	m_health = 10;

}


void JellyFishBoss::Update(float deltaTime)
{
	CheckForMainPlayer();

	AudioEngine& engine = AudioEngine::Instance();
	AudioEvent& shoot = engine.GetEvent("Enemy Shooting");

	if (m_isPlayerinRange) {
		m_isImmune = false;
		auto& enemyTrans = GameObject::GetComponent<Transform>(m_enemy);
		glm::vec3 nextPoint;
		glm::vec3 curPosOfEnemy;
		glm::vec3 temp = glm::vec3(0, 30, 0);

		nextPoint = GameObject::GetComponent<Transform>(m_player).GetLocalPos(); //current position of the player
		curPosOfEnemy = enemyTrans.GetLocalPos(); //current position

		if (glfwGetTime() - m_startTime >= m_fireRate) {

			if (!shoot.isPlaying())
				shoot.Play();
			else
			{
				shoot.Stop();
				shoot.Play();
			}

			auto bullet = GameObject::Allocate();
			bullet->AttachComponent<Projectile>(&m_enemy, entt::entity(0), bullet.get(), *m_bulletMesh).SetID(bullet->GetID());
			bullet->GetComponent<Projectile>().SetSpeed(300);
			bullet->AttachComponent<EntityType>() = EntityType::ENEMY;
			bullet->GetComponent<Projectile>().SetVelocity(glm::vec3(0, 0, -1));
			glm::vec3 offset1 = glm::vec3(0, 30, -10);
			bullet->GetComponent<Transform>().MoveLocalPos(offset1);
		bullet->GetComponent<Transform>().SetLocalScale(glm::vec3(10));
			m_resetTime = true;
		}
		if (m_resetTime) {
			m_startTime = glfwGetTime();
			m_resetTime = false;
		}

		if (m_MAX_ENEMIES_SPAWNED <= 3) {
			if (glfwGetTime() - m_startEnemyTime >= m_Enemy_SpawnRate) {

			

				GameObject::GetComponent<Transform>(m_enemy).MoveLocalPosFixed(temp);
				auto enemy = GameObject::Allocate();
				enemy->AttachComponent<Transform>();
				enemy->AttachComponent<BasicAI>(enemy->GetID(), m_enemy, m_player);
				enemy->AttachComponent<EntityType>() = EntityType::NEROTIST;
				enemy->AttachComponent<StaticRenderer>(entt::entity(0), enemy->GetID(), *m_enemyMesh, nullptr);
				enemy->GetComponent<Transform>().SetWHD(glm::vec3(m_enemyMesh->GetWidth(), m_enemyMesh->GetHeight(), m_enemyMesh->GetDepth()));
				resetEnemyTime = true;
				m_MAX_ENEMIES_SPAWNED++;
				temp = glm::vec3(0, -30, 0);
				enemyTrans.MoveLocalPosFixed(temp);
			}

			if (resetEnemyTime) {
				m_startEnemyTime = glfwGetTime();
				resetEnemyTime = false;
			}
			
		}
		temp = glm::vec3(0, 30, 0);
		enemyTrans.MoveLocalPosFixed(temp);
		glm::vec3 rotationvector = glm::degrees(glm::eulerAngles(glm::quat_cast(glm::transpose(glm::lookAt(enemyTrans.GetLocalPos(), nextPoint, glm::vec3(0, 1, 0))))));
		enemyTrans.SetLocalRot(rotationvector);
		temp = glm::vec3(0, -30, 0);
		enemyTrans.MoveLocalPosFixed(temp);
	}

	else {
		m_isImmune = true;
	}
}

void CentipedeBoss::Init(entt::entity jellyEntityID, entt::entity MainplayerID)
{
	m_enemy = jellyEntityID;
	m_player = MainplayerID;
	m_distance = 550;
	m_health = 10;

}

void CentipedeBoss::Init()
{
	for (int i = 0; i < 5; i++) {
		m_meshes.push_back(new Mesh());
		loadOBJ(("Resource Files/OBJFiles/Universe-27/EnemyShips/Morph/Boss/CentipedeBoss_" + std::to_string(i) + ".obj").c_str(), *m_meshes[i]);
	}
}

void CentipedeBoss::DeleteMeshes()
{
	for (int i = 0; i < m_meshes.size(); i++) {
		delete m_meshes[i];
		m_meshes.erase(m_meshes.begin() + i);
	}
}

void CentipedeBoss::Update(float deltaTime)
{
	CheckForMainPlayer();

	AudioEngine& engine = AudioEngine::Instance();
	AudioEvent& shoot = engine.GetEvent("Centipede");

	if (m_isPlayerinRange) {
		m_isImmune = false;
		auto& enemyTrans = GameObject::GetComponent<Transform>(m_enemy);
		glm::vec3 nextPoint;
		glm::vec3 curPosOfEnemy;

		nextPoint = GameObject::GetComponent<Transform>(m_player).GetLocalPos(); //current position of the player
		curPosOfEnemy = enemyTrans.GetLocalPos(); //current position


		//main head
		if (glfwGetTime() - m_startTimePerhead[0] >= m_fireRatePerhead[0]) {

			if (!shoot.isPlaying())
				shoot.Play();
			else
			{
				shoot.Stop();
				shoot.Play();
			}

			auto bullet = GameObject::Allocate();
			bullet->AttachComponent<Projectile>(&m_enemy, entt::entity(0), bullet.get(), *m_bulletMesh).SetID(bullet->GetID());
			bullet->GetComponent<Projectile>().SetSpeed(250);
			bullet->AttachComponent<EntityType>() = EntityType::ENEMY;
			bullet->GetComponent<Projectile>().SetVelocity(glm::vec3(0, 0, -1));
			glm::vec3 offset1 = glm::vec3(0, 0, -10);
			bullet->GetComponent<Transform>().MoveLocalPos(offset1);
			bullet->GetComponent<Transform>().SetLocalScale(glm::vec3(10));
			m_resetTimeShot[0] = true;
			
			if (m_resetTimeShot[0]) {
				m_startTimePerhead[0] = glfwGetTime();
				m_resetTimeShot[0] = false;
				m_fireRatePerhead[0] = m_fireRatePerhead[0] - ((m_speed * 0.001) * ((m_speed) / glm::abs(m_speed)));
			}
		}


		//Left head
		if (glfwGetTime() - m_startTimePerhead[1] >= m_fireRatePerhead[1]) {
		auto bullet = GameObject::Allocate();
			bullet->AttachComponent<Projectile>(&m_enemy, entt::entity(0), bullet.get(), *m_bulletMesh).SetID(bullet->GetID());
			bullet->GetComponent<Projectile>().SetSpeed(300);
			bullet->AttachComponent<EntityType>() = EntityType::ENEMY;
			bullet->GetComponent<Projectile>().SetVelocity(glm::vec3(0, 0, -1));
			glm::vec3 offset1 = glm::vec3(10, 0, 0);
			bullet->GetComponent<Transform>().MoveLocalPos(offset1);
			bullet->GetComponent<Transform>().SetLocalScale(glm::vec3(10));
			auto& bulletTrans = bullet->GetComponent<Transform>();
			bullet->GetComponent<Transform>().SetLocalRot(glm::vec3(bulletTrans.GetLocalRot().x, bulletTrans.GetLocalRot().y - 90, bulletTrans.GetLocalRot().z));
			m_resetTimeShot[1] = true;

			if (m_resetTimeShot[1]) {
				m_startTimePerhead[1] = glfwGetTime();
				m_resetTimeShot[1] = false;
				m_fireRatePerhead[1] = m_fireRatePerhead[1] - ((m_speed * 0.001) * ((m_speed) / glm::abs(m_speed)));
			}
		}

	//right head
		if (glfwGetTime() - m_startTimePerhead[2] >= m_fireRatePerhead[2]) {
			auto bullet = GameObject::Allocate();
			bullet->AttachComponent<Projectile>(&m_enemy, entt::entity(0), bullet.get(), *m_bulletMesh).SetID(bullet->GetID());
			bullet->GetComponent<Projectile>().SetSpeed(300);
			bullet->AttachComponent<EntityType>() = EntityType::ENEMY;
			bullet->GetComponent<Projectile>().SetVelocity(glm::vec3(0, 0, -1));
			glm::vec3 offset1 = glm::vec3(-10, 0, 0);
			bullet->GetComponent<Transform>().MoveLocalPos(offset1);
			bullet->GetComponent<Transform>().SetLocalScale(glm::vec3(10));
			auto& bulletTrans = bullet->GetComponent<Transform>();
			bullet->GetComponent<Transform>().SetLocalRot(glm::vec3(bulletTrans.GetLocalRot().x, bulletTrans.GetLocalRot().y + 90, bulletTrans.GetLocalRot().z));
			m_resetTimeShot[2] = true;
			
			if (m_resetTimeShot[2]) {
				m_startTimePerhead[2] = glfwGetTime();
				m_resetTimeShot[2] = false;
				m_fireRatePerhead[2] = m_fireRatePerhead[2] - ((m_speed * 0.001) * ((m_speed) / glm::abs(m_speed)));
			}
		}

		if (m_speed <= 20 && !m_flip)
			m_speed += deltaTime;
		
		else if (m_speed >= -20 && m_flip)
			m_speed -= deltaTime;
			
		if (m_speed > 20 || m_speed < -20)
		{
			m_flip = !m_flip;
			m_fireRatePerhead[0] = 0.6f;
			m_fireRatePerhead[1] = 0.6f;
			m_fireRatePerhead[2] = 0.6f;
		}
		
		glm::vec3 rotationvector = glm::vec3(0, m_speed, 0);
		enemyTrans.RotateLocal(rotationvector);
		
		}

	else {
		m_isImmune = true;
	}
}


HiveMindBoss::~HiveMindBoss()
{
	JellyFishBoss::DeleteMeshes();
	CentipedeBoss::DeleteMeshes();

	for (int i = 0; i < m_meshes.size(); i++) {
		delete m_meshes[i];
		m_meshes.erase(m_meshes.begin() + i);
	}
}

int HiveMindBoss::Phases()
{
	return m_phases;
}

void HiveMindBoss::Init(entt::entity HiveMindEntityID, entt::entity MainplayerID)
{
	m_enemy = HiveMindEntityID;
	m_player = MainplayerID;
	m_distance = 600;
	m_health = 20;
	m_fireRate = 1.5f;
}

void HiveMindBoss::JellyFishDefeated()
{
	m_JellyDefeat = true;
}

void HiveMindBoss::CentipedeDefeated()
{
	m_CentipedeDefeat = true;
}

void HiveMindBoss::Update(float deltaTime)
{
	CheckForMainPlayer();

	auto& enemyTrans = GameObject::GetComponent<Transform>(m_enemy);
	glm::vec3 nextPoint = GameObject::GetComponent<Transform>(m_player).GetLocalPos();
	glm::vec3 curPosOfEnemy;

	if (m_isPlayerinRange) {
	
//Phase one where the jelly fish boss is summoned
		if (m_phases == 0) {
			m_isImmune = true;
			
			if (m_bossPhaseSwitch) {
				if (glfwGetTime() - m_startTimeBetweenBoss >= m_DelayBetweenBoss) {
					m_JellySpawned = false;
					m_bossPhaseSwitch = false;
					
				}
			}

			if (m_JellyDefeat)
			{
				m_phases++;
				m_bossPhaseSwitch = true;
				m_startTimeBetweenBoss = glfwGetTime();
			}
		}


//Phase one where the jelly fish boss is summoned
		else if (m_phases == 1) {
			m_isImmune = true;
			
			if (m_bossPhaseSwitch) {
				if (glfwGetTime() - m_startTimeBetweenBoss >= m_DelayBetweenBoss) {
					m_CentipedeSpawned = false;
					m_bossPhaseSwitch = false;
				}
			}
			if (m_CentipedeDefeat)
			{
				m_phases++;
				m_bossPhaseSwitch = true;
				m_startTimeBetweenBoss = glfwGetTime();
			}
		}

		else if (m_phases == 2) {
			m_isImmune = false;
			if (glfwGetTime() - m_startTime >= m_fireRate) {
				auto kamabullet = GameObject::Allocate();
				kamabullet->AttachComponent<Transform>().SetLocalPos(GameObject::GetComponent<Transform>(m_enemy).GetLocalPos());
				kamabullet->GetComponent<Transform>().SetLocalScale(glm::vec3(5));
				kamabullet->AttachComponent<KamakaziBullet>(kamabullet->GetID(), m_player).SetSpeed(25);
				kamabullet->AttachComponent<EntityType>() = EntityType::KAMIBULLET;
				kamabullet->AttachComponent<StaticRenderer>(entt::entity(0), kamabullet->GetID(), *m_bulletMesh, nullptr);
				kamabullet->GetComponent<Transform>().SetWHD(glm::vec3(m_bulletMesh->GetWidth() * 2, m_bulletMesh->GetHeight() * 2, m_bulletMesh->GetDepth() * 2));

				m_resetTime = true;
			}
			if (m_resetTime) {
				m_startTime = glfwGetTime();
				m_resetTime = false;
			}
		}
	}

	else
		m_isImmune = true;
	

	glm::vec3 rotationvector = glm::degrees(glm::eulerAngles(glm::quat_cast(glm::transpose(glm::lookAt(enemyTrans.GetLocalPos(), nextPoint, glm::vec3(0, 1, 0))))));
	enemyTrans.SetLocalRot(rotationvector);
}

void HiveMindBoss::Init()
{
	for (int i = 0; i < 5; i++) {
		m_meshes.push_back(new Mesh());
		loadOBJ(("Resource Files/OBJFiles/Universe-5/EnemyShips/Morph/Boss/Hivemind_" + std::to_string(i) + ".obj").c_str(), *m_meshes[i]);
	}
}
