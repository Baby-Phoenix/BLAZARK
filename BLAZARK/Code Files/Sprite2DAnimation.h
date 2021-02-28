#pragma once

#include "Texture.h"
#include "glm.hpp"
#include "Vertex.h"
#include "GLTFLoader.h"

struct UVS
{
	UVS(glm::vec2 bottomLeft,glm::vec2 topRight);
	UVS(glm::vec2 bottomLeft, glm::vec2 bottomRight, glm::vec2 topLeft, glm::vec2 topRight);

	glm::vec2 m_bottomLeft;
	glm::vec2 m_bottomRight;
	glm::vec2 m_topLeft;
	glm::vec2 m_topRight;
};

class Animation2D
{
public:
	Animation2D() = default;
	Animation2D(UVS frame, bool repeat = true);

	//Adding the frames
	void AddFrame(UVS frame);
	void AddFrame(glm::vec2 bottomLeft, glm::vec2 topRight);
	void RemoveFrame();


	void Update(float deltaTime);
	void Reset();

	//getters
	bool GetIfAnimationDone();
	UVS GetCurrFrame();
	UVS GetNextFrame();
	std::vector<UVS> GetAllFrames();
	bool GetIsRepeating();
	float GetSecPerFrame();
	float GetT();

	//setters
	void SetIsRepeating(bool repeat);
	void SetSecPerFrame(float sec);


private:
	std::vector<UVS> m_frames;
	unsigned int m_currentFrame = 0;
	float m_secPerFrame = 1.f;
	float m_timePerFrame = m_secPerFrame;
	float m_timePerFrameOriginal = m_secPerFrame;
	bool m_isRepeating;
	float T = 0;
};

class AnimationHandler
{
public:
	AnimationHandler() = default;
	~AnimationHandler() {
		delete First_frame;
		delete Second_frame;
	}
	void InitUVS(Texture* tex);

	void Update(float deltaTime);
	void UpdateAnimation();

	void AddAnimation(Animation2D anim);

	//Getters
	Animation2D& GetAnimation(int anim);
	std::vector<Animation2D> GetAnimation();;
	int GetActiveAnimation();
	float GetT();

	//Setter
	void SetTextureSize(glm::vec2 size);
	void SetActiveAnim(int anim);
	void SetVAO(VertexArray* vao);

private:
	unsigned int m_activeAnimation = 0;

	std::vector<Animation2D> m_animations;

	glm::vec2 m_texturesize;

	Mesh* First_frame = nullptr;
	Mesh* Second_frame = nullptr;

	VertexArray* m_sprite_VAO = nullptr;

	float T = 0;

};