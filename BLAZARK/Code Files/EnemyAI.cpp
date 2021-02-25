#include "EnemyAI.h"

bool BasicAI::initRandom = false;


BasicAI::BasicAI(Transform* enemy, Transform* avoid, Transform* player, float dist)
{
	if (!initRandom) {
		Random::Init();
	}
	SetTransform(enemy, player);
	GeneratePoints(avoid);
}


void BasicAI::Update()
{
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
	
}

void BasicAI::CheckForMainPlayer()
{
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
