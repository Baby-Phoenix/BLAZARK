#pragma once

#include "ECS.h"
#include "DynamicRenderer.h"
#include <algorithm>
class MorphAnimController {
public:
	MorphAnimController() = default;
	MorphAnimController(unsigned int entity);
	~MorphAnimController() = default;

	MorphAnimController(MorphAnimController&&) = default;
	MorphAnimController& operator = (MorphAnimController&&) = default;

	void Update(float deltaTime);

	// Setters
	void SetFrames(std::vector<Mesh*>& frames, int startIndex, int endIndex, bool isRepeat = true);
	void SetFrameTime(float frameTime);
	void SetAnimate(bool isAnimate);
	void SetTargert(int index);

	bool getAnimate();

private:
	entt::entity m_entity;

	std::vector<Mesh*> m_frames;

	float m_timer;
	float m_frameTime;
	int m_index;
	int m_targetIndex;

	bool m_isRepeat = true;
	bool m_isAnimate = true;
};