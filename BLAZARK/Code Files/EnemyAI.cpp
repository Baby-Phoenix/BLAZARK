#include "EnemyAI.h"

bool BasicAI::initRandom = false;
Mesh* BasicAI::m_bulletMesh = nullptr;

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
			auto bullet = GameObject::Allocate();
			bullet->AttachComponent<Projectile>(&m_enemy, entt::entity(0), bullet.get(), *m_bulletMesh).SetID(bullet->GetID());
			bullet->GetComponent<Projectile>().SetSpeed(500);
			bullet->AttachComponent<EntityType>() = EntityType::ENEMY;
			bullet->GetComponent<Projectile>().SetVelocity(glm::vec3(0, 0, -1));
			glm::vec3 offset1 = glm::vec3(0, 0, -10);
			bullet->GetComponent<Transform>().MoveLocalPos(offset1);
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
	{
		m_isPlayerinRange = true;
		glm::vec3 rotationvector = glm::degrees(glm::eulerAngles(glm::quat_cast(glm::transpose(glm::lookAt(enemyPos, playerPos, glm::vec3(0, 1, 0))))));
		enemyTrans.RotateLocal(rotationvector);
	}

	else if(compareDist > m_distance && m_isPlayerinRange)
		m_isPlayerinRange = false;
}

KamakaziAI::KamakaziAI(entt::entity enemy, entt::entity avoid, entt::entity player)
	:BasicAI(enemy,avoid, player, 300)
{
}

void KamakaziAI::Update(float deltaTime)
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
	:BasicAI(enemy, avoid, player, 100)
{
	m_fireRate = 1.5f;
}

void BombardierAI::Update(float deltaTime)
{

	CheckForMainPlayer();

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
			auto kamabullet = GameObject::Allocate();
			kamabullet->AttachComponent<Transform>().SetLocalPos(GameObject::GetComponent<Transform>(m_enemy).GetLocalPos());
			kamabullet->GetComponent<Transform>().SetLocalScale(glm::vec3(2));
			kamabullet->AttachComponent<KamakaziBullet>(kamabullet->GetID(), m_player);
			kamabullet->AttachComponent<EntityType>() = EntityType::KAMABULLET;
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
