#include "Sprite2DAnimation.h"

UVS::UVS(glm::vec2 bottomLeft, glm::vec2 topRight)
	:m_bottomLeft(bottomLeft),
	m_bottomRight(glm::vec2(topRight.x, bottomLeft.y)), 
	m_topLeft(glm::vec2(bottomLeft.x, topRight.y)), m_topRight(topRight)
{
}

UVS::UVS(glm::vec2 bottomLeft, glm::vec2 bottomRight, glm::vec2 topLeft, glm::vec2 topRight)
	:m_bottomLeft(bottomLeft), m_bottomRight(bottomRight), m_topLeft(topLeft), m_topRight(topRight)
{
}

Animation2D::Animation2D(UVS frame, bool repeat)
{
	AddFrame(frame);
	m_isRepeating = repeat;
}

void Animation2D::AddFrame(UVS frame)
{
	m_frames.push_back(frame);
}

void Animation2D::AddFrame(glm::vec2 bottomLeft, glm::vec2 topRight)
{
	m_frames.push_back(UVS(bottomLeft, topRight));
}

void Animation2D::Update(float deltaTime)
{
	//Subtract delta time from
	//Frame time
	m_timePerFrame -= deltaTime;
	T = m_timePerFrame / m_timePerFrameOriginal;

	//If frametime is less than or equal to zero
		//Increase the frame and reset frametime
	if (m_timePerFrame <= 0.f)
	{
		T = 0;
		m_currentFrame++;
		m_timePerFrame = m_secPerFrame;
		m_timePerFrameOriginal = m_timePerFrame;
		//If animation repeating
			//Check for reset
		if (m_isRepeating)
		{
			//If current frame is greater than or equal to total num frames
				//Reset current frame to zero
			if (m_currentFrame >= m_frames.size())
				m_currentFrame = 0;
			
		}
		else
		{
			//If current frame is greater than or equal to total num frames
				//Reset to last frame
			if (m_currentFrame >= m_frames.size())
				m_currentFrame = int(m_frames.size() - 1);
			
		}
	}
}

void Animation2D::Reset()
{
	m_timePerFrame = m_secPerFrame;
	m_currentFrame = 0;
}

bool Animation2D::GetIfAnimationDone()
{
	return (m_frames.size() - 1) <= m_currentFrame;
}

UVS Animation2D::GetCurrFrame()
{
	return m_frames[m_currentFrame];
}

UVS Animation2D::GetNextFrame()
{
	return m_frames[m_currentFrame >= m_frames.size() - 1 ? m_frames.size()-1 : m_currentFrame + 1];
}

std::vector<UVS> Animation2D::GetAllFrames()
{
	return m_frames;
}

bool Animation2D::GetIsRepeating()
{
	return m_isRepeating;
}

float Animation2D::GetSecPerFrame()
{
	return m_secPerFrame;
}

float Animation2D::GetT()
{
	return T;
}

void Animation2D::SetIsRepeating(bool repeat)
{
	m_isRepeating = repeat;
}

void Animation2D::SetSecPerFrame(float sec)
{
	m_secPerFrame = sec;

	Reset();
}

void AnimationHandler::InitUVS(Texture* tex)
{
	m_texturesize = tex->GetWidthAndHeight();
	vbo_one = new VertexBuffer();
	vbo_two = new VertexBuffer();
}

void AnimationHandler::Update(float deltaTime)
{
	m_animations[m_activeAnimation].Update(deltaTime);
	T = 0;
	/*T = m_animations[m_activeAnimation].GetT();*/

	UpdateAnimation();
}

void AnimationHandler::UpdateAnimation()
{

	std::vector<glm::vec4> tempFrame;

	tempFrame.resize(6);
	
	//UVS for the current frame
	auto uv = m_animations[m_activeAnimation].GetCurrFrame();

	glm::vec2 botL = glm::vec2(uv.m_bottomLeft.x / m_texturesize.x, uv.m_bottomLeft.y / m_texturesize.y);
	glm::vec2 botR = glm::vec2(uv.m_bottomRight.x / m_texturesize.x, uv.m_bottomRight.y / m_texturesize.y);
	glm::vec2 topL = glm::vec2(uv.m_topLeft.x / m_texturesize.x, uv.m_topLeft.y / m_texturesize.y);
	glm::vec2 topR = glm::vec2(uv.m_topRight.x / m_texturesize.x, uv.m_topRight.y / m_texturesize.y);


	tempFrame[0] = glm::vec4(topR,botL);
	tempFrame[1] = glm::vec4(botL,topL);
	tempFrame[2] = glm::vec4(topL, topR);

	tempFrame[3] = glm::vec4(topR, botR);
	tempFrame[4] = glm::vec4(botR, botL);
	tempFrame[5] = glm::vec4(botL, topR);

	/*
	tempFrame[0] = 

	*/


	vbo_one->UpdateBufferData(tempFrame);

	//UVS for Frame 1
	m_sprite_VAO->BindBuffer(*vbo_one, (GLint)2);

	//	//UVS for the next frame
	//uv = m_animations[m_activeAnimation].GetNextFrame();

	//	
	//	 botL = glm::vec2(uv.m_bottomLeft.x / m_texturesize.x, uv.m_bottomLeft.y / m_texturesize.y);
	//	 botR = glm::vec2(uv.m_bottomRight.x / m_texturesize.x, uv.m_bottomRight.y / m_texturesize.y);
	//	 topL = glm::vec2(uv.m_topLeft.x / m_texturesize.x, uv.m_topLeft.y / m_texturesize.y);
	//	 topR = glm::vec2(uv.m_topRight.x / m_texturesize.x, uv.m_topRight.y / m_texturesize.y);


	// tempFrame[0] = glm::vec4(botL, botR);
	// tempFrame[1] = glm::vec4(botR, topR);
	// tempFrame[2] = glm::vec4(topR, botL);

	// tempFrame[3] = glm::vec4(botL, topR);
	// tempFrame[4] = glm::vec4(topR, topL);
	// tempFrame[5] = glm::vec4(topL, botL);

	//	/* tempFrame[6] = botL;
	//	 tempFrame[7] = botR;
	//	 tempFrame[8] = topR;

	//	 tempFrame[9] = botL;
	//	 tempFrame[10] = topR;
	//	 tempFrame[11] = topL;*/


	//	 vbo_two->UpdateBufferData(tempFrame);
	//	
	//	//UVS for Frame 2
	//	m_sprite_VAO->BindBuffer(*vbo_two, (GLint)Mesh::VertexAttrib::TEXCOORD);

}

void AnimationHandler::AddAnimation(Animation2D anim)
{
	m_animations.push_back(anim);
}


Animation2D& AnimationHandler::GetAnimation(int anim)
{
	return m_animations[anim];
}

std::vector<Animation2D> AnimationHandler::GetAnimation()
{
	return m_animations;
}

int AnimationHandler::GetActiveAnimation()
{
	return m_activeAnimation;
}

float AnimationHandler::GetT()
{
	return T;
}

VertexBuffer* AnimationHandler::GetUVVBO_two()
{
	return vbo_two;
}

VertexBuffer* AnimationHandler::GetUVVBO_one()
{
	return vbo_one;
}

void AnimationHandler::SetTextureSize(glm::vec2 size)
{
	m_texturesize = size;
}

void AnimationHandler::SetActiveAnim(int anim)
{
	m_activeAnimation = anim;
}

void AnimationHandler::SetVAO(VertexArray* vao)
{
	m_sprite_VAO = vao;
}
