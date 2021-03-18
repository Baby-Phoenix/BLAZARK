#include "MorphAnimController.h"

MorphAnimController::MorphAnimController(unsigned int entity) {
	m_entity = entt::entity(entity);
	m_timer = 0.0f;
	m_frameTime = 1.0f;
	m_index = 0;
}

void MorphAnimController::Update(float deltaTime) {
	if (m_frames.empty())
		return;

	float t;

	if (m_frameTime > 0.0f) {
		m_timer += deltaTime;

		if (m_timer > m_frameTime) {
			m_index++;

			if (m_index >= m_frames.size())
				m_index = 0;
		}

		m_timer = fmod(m_timer, m_frameTime);
		t = m_timer / m_frameTime;
	}
	else {
		t = 0.0f;
	}

	size_t indexOne, indexTwo = m_index;
	indexOne = (indexTwo == 0) ? m_frames.size() - 1 : indexTwo - 1;

	GameObject::GetComponent<DynamicRenderer>(m_entity).UpdateVAO(*m_frames[indexOne], *m_frames[indexTwo], t);
}

void MorphAnimController::SetFrames(std::vector<Mesh*>& frameList, int startIndex, int endIndex) {
	for (int i = startIndex; i <= endIndex; i++) {
		m_frames.push_back(frameList[i]);
	}
}

void MorphAnimController::SetFrameTime(float frameTime) {
	m_frameTime = frameTime;
}
