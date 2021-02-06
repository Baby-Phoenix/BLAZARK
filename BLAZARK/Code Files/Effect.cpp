#include "Effect.h"

void Effect::Init(unsigned int width, unsigned int height)
{
	int index = int(m_buffers.size());
	m_buffers.push_back(std::move(std::make_unique<FrameBuffer>()));
	m_buffers[index]->AddColor(GL_RGBA8);
	m_buffers[index]->AddDepth();
	m_buffers[index]->Init(width, height);

	index = int(m_effect_shaders.size());
	m_effect_shaders.push_back(new Shader("Resource Files/Shaders/passthrough_vert.glsl", "Resource Files/Shaders/passthrough_frag.glsl"));

}

void Effect::ApplyEffect(Effect* previousEffect)
{
	BindShader(0);
	previousEffect->BindColorTexture(0, 0, 0);
	
	m_buffers[0]->RenderToFSQ();

	previousEffect->UnbindTexture(0);

	UnBindShader();
}

void Effect::Draw()
{
	BindShader(0);

	BindColorTexture(0, 0, 0);

	m_buffers[0]->DrawFullscreenQuad();

	UnbindTexture(0);

	UnBindShader();
}

void Effect::ReshapeBuffer(unsigned int width, unsigned int height)
{
	for (int i = 0; i < m_buffers.size(); i++) 
		m_buffers[i]->ReshapeBuffer(width, height);
	
}

void Effect::Clear()
{
	for (int i = 0; i < m_buffers.size(); i++)
		m_buffers[i]->Clear();
}

void Effect::Unload()
{
	for (int i = 0; i < m_buffers.size(); i++)
	{
		if (m_buffers[i].get() != nullptr) {
			m_buffers[i]->Unload();
			m_buffers[i].reset();
		}
	}

	m_effect_shaders.clear();
}

void Effect::BindBuffer(int index)
{
	m_buffers[index]->Bind();
}

void Effect::UnbindBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

void Effect::BindColorTexture(int index, int colorBuffer, int textureSlot)
{
	m_buffers[index]->BindColorTexture(colorBuffer, textureSlot);
}

void Effect::BindDepthTexture(int index, int textureSlot)
{
	m_buffers[index]->BindDepthTexture(textureSlot);
}

void Effect::UnbindTexture(int textureSlot)
{
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
}

void Effect::BindShader(int index)
{
	m_effect_shaders[index]->use();
}

void Effect::UnBindShader()
{
	glUseProgram(GL_NONE);
}
