#include "EnemyAI.h"

bool BasicAI::initRandom = false;

glm::vec3 Lerp(glm::vec3 point1, glm::vec3 point2, float t) {
	return ((float(1.0 - t) * point1) + (t * point2));
}

BasicAI::BasicAI(Transform* enemy, Transform* avoid, Transform* player, float dist)
{
	if (!initRandom) {
		Random::Init();
	}
	SetTransform(enemy, player);
	GeneratePoints(avoid);

	m_distance = dist;
}


void BasicAI::Update()
{
	CheckForMainPlayer();

	if (m_isPlayerinRange) {

		glm::vec3 curPoint = m_points[m_nextPoint == 0 ? m_points.size() - 2 : m_nextPoint - 1];
		glm::vec3 nextPoint = m_points[m_points.size() - 1];
		glm::vec3 curPosOfEnemy = m_enemy->GetLocalPos();
		
		/*m_enemy->SetLocalRot(glm::quatLookAt(nextPoint, glm::vec3(0, 1, 0)));*/

		float totalDist = glm::sqrt(((nextPoint.x- curPoint.x)*(nextPoint.x - curPoint.x))+ ((nextPoint.z - curPoint.z) * (nextPoint.z - curPoint.z)));
		float curDist = glm::sqrt(((curPosOfEnemy.x - curPoint.x) * (curPosOfEnemy.x - curPoint.x)) + ((curPosOfEnemy.z - curPoint.z) * (curPosOfEnemy.z - curPoint.z)));

		float t = curDist / totalDist; //0 - 1 interpolation paramenter 

		if (t > 0.3)
		m_enemy->SetLocalPos(Lerp(m_enemy->GetLocalPos(), m_points[m_points.size() - 1], t));
	
	}
	else
	{
		glm::vec3 curPoint = m_points[m_nextPoint == 0 ? m_points.size() - 1 : m_nextPoint - 1];
		glm::vec3 nextPoint = m_points[m_nextPoint];
		glm::vec3 curPosOfEnemy = m_enemy->GetLocalPos();

		/*m_enemy->SetLocalRot(glm::quatLookAt(nextPoint, glm::vec3(0, 1, 0)));*/

		float totalDist = glm::sqrt(((nextPoint.x - curPoint.x) * (nextPoint.x - curPoint.x)) + ((nextPoint.z - curPoint.z) * (nextPoint.z - curPoint.z)));
		float curDist = glm::sqrt(((curPosOfEnemy.x - curPoint.x) * (curPosOfEnemy.x - curPoint.x)) + ((curPosOfEnemy.z - curPoint.z) * (curPosOfEnemy.z - curPoint.z)));

		float t = curDist / totalDist; // 0 - 1 interpolation paramenter 

			m_enemy->SetLocalPos(Lerp(m_enemy->GetLocalPos(), m_points[m_nextPoint], t));

			if (t <= 0.0) {
				m_nextPoint++;
				if (m_nextPoint > m_points.size() - 1) {
					m_nextPoint = 0;
			}
			}
	}
}

void BasicAI::SetTransform(Transform* enemy, Transform* player)
{
	SetEnemyTransform(enemy);
	SetPlayerTransform(player);
}

void BasicAI::SetEnemyTransform(Transform* enemy)
{
	m_enemy = enemy;
}

void BasicAI::SetPlayerTransform(Transform* player)
{
	m_player = player;
}

void BasicAI::GeneratePoints(Transform* avoidPlace)
{
	//gets number of points
	int numberOfPoints = Random::Range1f(2, 5);

	m_points.resize(numberOfPoints);

	//gets the innitial point
	glm::vec3 randomPoint = Random::GetPointBetween(avoidPlace->GetLocalPos(), avoidPlace->GetRadius());
	m_points[0] = (randomPoint);
	std::cout << "("<<m_points[0].x << "," << m_points[0].y << "," << m_points[0].z << ")\n";
	m_enemy->SetLocalPos(m_points[0]);
	m_direction = glm::normalize(m_points[0]);

	

	//gets the rest of the points
	for (unsigned i = 1; i < numberOfPoints; i++) {
		
		glm::vec3 direction = glm::normalize(m_points[i - 1]);
		
		float angle = Random::Range1f(-90, 90);
		glm::vec2 dist = Random::Range2f(10, 100); //(15,30)

		//final direction
		glm::vec3 finalDirection = glm::mat3(glm::vec3(glm::cos(angle), 0, glm::sin(angle)), 
											 glm::vec3(0, 1, 0),
											 glm::vec3(-glm::sin(angle), 0, glm::cos(angle))) * direction;

		m_points[i] = glm::vec3((finalDirection.x + dist.x) + m_points[i - 1].x, 0, (finalDirection.z + dist.y) + m_points[i - 1].z);
		std::cout << "("<<m_points[i].x << "," << m_points[i].y << "," << m_points[i].z << ")\n";
	}

	/*glm::vec3 nextPoint = m_points[1];

	m_enemy->SetLocalRot(glm::quatLookAt(nextPoint, glm::vec3(0, 1, 0)));*/
}

void BasicAI::CheckForMainPlayer()
{
	glm::vec3 enemyPos = m_enemy->GetLocalPos();
	glm::vec3 playerPos = m_enemy->GetLocalPos();

	//check the distance between the player and this enemy
	float compareDist = 
		glm::sqrt(((enemyPos.x - playerPos.x) * (enemyPos.x - playerPos.x)) + 
								  ((enemyPos.z - playerPos.z) * (enemyPos.z - playerPos.z)));

	if (compareDist < m_distance && !m_isPlayerinRange) {
		m_points.push_back(m_player->GetLocalPos());
		m_isPlayerinRange = true;
	}

	else if(compareDist > m_distance && m_isPlayerinRange)
	{
		m_points.pop_back();
		m_isPlayerinRange = false;
	}
}

KamakaziAI::KamakaziAI(Transform* enemy, Transform* player)
{
}

void KamakaziAI::Update()
{
}

void KamakaziAI::CheckForMainPlayer()
{
}


ScavengerAI::ScavengerAI(Transform* enemy, Transform* player)
{
}

void ScavengerAI::Update()
{
}

void ScavengerAI::CheckForMainPlayer()
{
}


BombardierAI::BombardierAI(Transform* enemy, Transform* player)
{
}

void BombardierAI::Update()
{
}

void BombardierAI::CheckForMainPlayer()
{
}
