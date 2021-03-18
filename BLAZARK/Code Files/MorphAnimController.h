#pragma once

#include "ECS.h"
#include "DynamicRenderer.h"

class MorphAnimController {
public:
	MorphAnimController() = default;
	MorphAnimController(unsigned int entity);
	~MorphAnimController() = default;

	MorphAnimController(MorphAnimController&&) = default;
	MorphAnimController& operator = (MorphAnimController&&) = default;

	void Update(float deltaTime);

	// Setters
	void SetFrames(std::vector<Mesh*>& frames, int startIndex, int endIndex);
	void SetFrameTime(float frameTime);

private:
	entt::entity m_entity;

	std::vector<Mesh*> m_frames;

	float m_timer;
	float m_frameTime;
	int m_index;
};