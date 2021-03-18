#pragma once
#include <vector>
#include <iostream>
#include "ECS.h"
#include "Transform.h"
#include "Sprite2DAnimation.h"
#include "Sprite2D.h"


class ScoreHandler {

public:
	ScoreHandler() = default;
	ScoreHandler(glm::vec3 scorePos, Texture* tex, entt::entity* cam);

	void SetTexture(Texture* tex);
	void SetScorePos(glm::vec3 scorePos);
	void Add(int num);
	int GetSize();
	int GetNumberSize(int number);

private:

	void CreateSprite();
	void UpdateAnimations();

	std::vector<int> m_score;
	std::vector<std::unique_ptr<GameObject>> m_scoreObj;
	glm::vec3 m_ScorePos;
	Texture* m_scoreTex = nullptr;
	static entt::entity* camentity;
};