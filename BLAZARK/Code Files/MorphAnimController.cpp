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

	if (m_isRepeat)
	{
		if (m_frameTime > 0.0f) {
			m_timer += deltaTime;

			if (m_timer > m_frameTime) {
				m_index++;

				if (m_index >= m_frames.size())
				{
					m_index = 0;
				}

			}

			m_timer = fmod(m_timer, m_frameTime);
			t = m_timer / m_frameTime;


		}
		else {
			t = 0.0f;
		}
	}
	else
	{
		

		if (m_frameTime > 0.0f) {
			m_timer += deltaTime;

			if (m_timer > m_frameTime) {

				if (m_isAnimate)
				{
					if (m_index == 0 || m_index == 2)
					{
						m_isAnimate = false;
					}

					m_index++;

					if (m_index == m_frames.size())
					{
					 	m_index = 0;
					}
				}
			}

			m_timer = fmod(m_timer, m_frameTime);
			t = m_timer / m_frameTime;


		}
		else {
			t = 0.0f;
		}
	}
	
	/*if (m_isReversed && !m_isAnimate)
	{
		m_index = 1;
		std::reverse(m_frames.begin(), m_frames.end());
		m_isReversed = false;

	}*/
	
		size_t indexOne, indexTwo = m_index;

	if (m_isAnimate)
	{
		indexOne = (indexTwo == 0) ? m_frames.size() - 1 : indexTwo - 1;

		GameObject::GetComponent<DynamicRenderer>(m_entity).UpdateVAO(*m_frames[indexOne], *m_frames[indexTwo], t);
	}
	
}

void MorphAnimController::SetFrames(std::vector<Mesh*>& frameList, int startIndex, int endIndex, bool isRepeat) {
	for (int i = startIndex; i <= endIndex; i++) {
		m_frames.push_back(frameList[i]);
	}
	
	if (!isRepeat) {
		m_frames.push_back(m_frames[1]);
	}
	
	m_isRepeat = isRepeat;
}

void MorphAnimController::SetFrameTime(float frameTime) {
	m_frameTime = frameTime;
}


void MorphAnimController::SetAnimate(bool isAnimate)
{
	m_isAnimate = isAnimate;
}

void MorphAnimController::SetTargert(int index)
{
	m_targetIndex = index;
}

bool MorphAnimController::getAnimate()
{
	return m_isAnimate;
}
