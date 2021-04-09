#include "ScoreHandler.h"

ScoreHandler::ScoreHandler(glm::vec3 scorePos, Texture* tex, entt::entity* cam)
{
	m_ScorePos = scorePos;
	m_score.push_back(0);
	
	if (cam != nullptr)
		camentity = cam;

	SetTexture(tex);
	CreateSprite();
	UpdateAnimations();
}

void ScoreHandler::SetTexture(Texture* tex)
{
	m_scoreTex = tex;
}

void ScoreHandler::SetScorePos(glm::vec3 scorePos)
{
	m_ScorePos = scorePos;
}

int ScoreHandler::GetScore()
{  
	int score = 0;

	for (int i = 0; i < m_score.size(); i++) {
		score += m_score[i] * std::pow(10, i);

	}

	return score;
}

void ScoreHandler::Add(int num) 
{
	int temp = num;
	int size = ScoreHandler::GetNumberSize(num);
	int total;
	bool addon = false;

	size = size > m_score.size() - 1 ? size : m_score.size();

	for (int i = 0; i < size; i++) {
	 
		if (i > m_score.size() - 1) {
			m_score.push_back(0);
			CreateSprite();
		}

		if(i <= m_score.size() - 1) {

			temp = num % 10;
			num /= 10;
			total = m_score[i] + temp + addon;
				addon = false;	

			if (total >= 10) {
				total = total % 10;
				m_score[i] = total;
				addon = true;
			}
			else
				m_score[i] = total;
			
		}
		

	}

	if (addon) {
		int temp = addon;
		m_score.push_back(0);
		m_score[m_score.size() - 1] = temp % 10;
		CreateSprite();

	}

	UpdateAnimations();

}

int ScoreHandler::GetSize()
{
	return m_score.size();
}

int ScoreHandler::GetNumberSize(int number)
{
	int size = 0;

	while (number > 0)
	{
		number /= 10;
		size++;
	}

	return size;
}

void ScoreHandler::CreateSprite()
{
	glm::vec3 prevPos = m_scoreObj.size() == 0 ? m_ScorePos + glm::vec3(15, 0, 0) : m_scoreObj[m_scoreObj.size() - 1]->GetComponent<Transform>().GetLocalPos();
	glm::vec3 NextPos = prevPos + glm::vec3(6, 0, 0);


	int index = m_scoreObj.size();
	m_scoreObj.push_back(GameObject::Allocate());
	if(index<=0)
	m_scoreObj[index]->AttachComponent<Transform>().SetLocalPos(prevPos);

	else {
		m_scoreObj[index]->AttachComponent<Transform>().SetLocalPos(prevPos);
		for (int i = index-1; i >= 0; i--) {
			m_scoreObj[i]->GetComponent<Transform>().SetLocalPos(NextPos);
			NextPos = NextPos + glm::vec3(6, 0, 0);
		}
	}
	
	auto* tempAnim = &m_scoreObj[index]->AttachComponent<AnimationHandler>();
	auto& anim = m_scoreObj[index]->GetComponent<AnimationHandler>();

	
	anim.InitUVS(m_scoreTex);
	Animation2D Oneclip;

#pragma region InitUVS
	Oneclip.AddFrame(UVS(glm::vec2(1, 53), glm::vec2(42, 1))); //0

	Oneclip.SetIsRepeating(false);
	Oneclip.SetSecPerFrame(1.0);
	anim.AddAnimation(Oneclip);
	Oneclip.Clear();

	Oneclip.AddFrame(UVS(glm::vec2(44, 53), glm::vec2(85, 1))); //1

	Oneclip.SetIsRepeating(false);
	Oneclip.SetSecPerFrame(1.0);
	anim.AddAnimation(Oneclip);
	Oneclip.Clear();

	Oneclip.AddFrame(UVS(glm::vec2(87, 53), glm::vec2(128, 1))); //2

	Oneclip.SetIsRepeating(false);
	Oneclip.SetSecPerFrame(1.0);
	anim.AddAnimation(Oneclip);
	Oneclip.Clear();

	Oneclip.AddFrame(UVS(glm::vec2(130, 53), glm::vec2(171, 1))); //3

	Oneclip.SetIsRepeating(false);
	Oneclip.SetSecPerFrame(1.0);
	anim.AddAnimation(Oneclip);
	Oneclip.Clear();

	Oneclip.AddFrame(UVS(glm::vec2(173, 53), glm::vec2(214, 1))); //4

	Oneclip.SetIsRepeating(false);
	Oneclip.SetSecPerFrame(1.0);
	anim.AddAnimation(Oneclip);
	Oneclip.Clear();

	Oneclip.AddFrame(UVS(glm::vec2(216, 53), glm::vec2(257, 1))); //5

	Oneclip.SetIsRepeating(false);
	Oneclip.SetSecPerFrame(1.0);
	anim.AddAnimation(Oneclip);
	Oneclip.Clear();

	Oneclip.AddFrame(UVS(glm::vec2(259, 53), glm::vec2(300, 1))); //6

	Oneclip.SetIsRepeating(false);
	Oneclip.SetSecPerFrame(1.0);
	anim.AddAnimation(Oneclip);
	Oneclip.Clear();

	Oneclip.AddFrame(UVS(glm::vec2(302, 53), glm::vec2(343, 1))); //7

	Oneclip.SetIsRepeating(false);
	Oneclip.SetSecPerFrame(1.0);
	anim.AddAnimation(Oneclip);
	Oneclip.Clear();

	Oneclip.AddFrame(UVS(glm::vec2(345, 53), glm::vec2(386, 1))); //8

	Oneclip.SetIsRepeating(false);
	Oneclip.SetSecPerFrame(1.0);
	anim.AddAnimation(Oneclip);
	Oneclip.Clear();

	Oneclip.AddFrame(UVS(glm::vec2(388, 53), glm::vec2(429, 1))); //9

	Oneclip.SetIsRepeating(false);
	Oneclip.SetSecPerFrame(1.0);
	anim.AddAnimation(Oneclip);
	Oneclip.Clear();
	

#pragma endregion
	anim.SetActiveAnim(0);

	m_scoreObj[index]->AttachComponent<Sprite2D>(m_scoreTex, m_scoreObj[index]->GetID(), 3, 5, false, tempAnim);
	
	if (camentity != nullptr)
	m_scoreObj[index]->GetComponent<Transform>().SetParent(camentity);
	
}

void ScoreHandler::UpdateAnimations()
{
	for (int i = 0; i < m_scoreObj.size(); i++) {
		m_scoreObj[i]->GetComponent<AnimationHandler>().SetActiveAnim(m_score[i]);
	}
}
