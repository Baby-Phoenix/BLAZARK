#pragma once
#include "FrameBuffer.h"

class Effect {

public:
	virtual void Init(unsigned int width,unsigned int height);

	virtual void ApplyEffect(Effect* previousEffect);
	virtual void Draw();

	virtual void ReshapeBuffer(unsigned int width, unsigned int height);

	void Clear();

	void Unload();

	void BindBuffer(int index);
	void UnbindBuffer();

	void BindColorTexture(int index, int colorBuffer, int textureSlot);
	void BindDepthTexture(int index, int textureSlot);
	void UnbindTexture(int textureSlot);

	void BindShader(int index);
	void UnBindShader();

protected:

	std::vector<std::unique_ptr<FrameBuffer>> m_buffers;

	std::vector<Shader*> m_effect_shaders;
	

};