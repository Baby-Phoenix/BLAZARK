#include "PixelationEffect.h"

void PixelationEffect::Init(unsigned width, unsigned height) {
	// Set up framebuffer
	int index = int(_buffers.size());
	_buffers.push_back(new FrameBuffer());
	_buffers[index]->AddColorTarget(GL_RGBA8);
	_buffers[index]->AddDepthTarget();
	_buffers[index]->Init(width, height);

	// Set up shaders
	index = int(_shaders.size());
	_shaders.push_back(new Shader("Resource Files/Shaders/passthrough_vert.glsl", "Resource Files/Shaders/PostEffects/pixelation_frag.glsl"));
}

void PixelationEffect::ApplyEffect(PostEffect* effect) {
	BindShader(0);

	_shaders[0]->set1f(m_intensity, "u_Intensity");

	effect->BindColorAsTexture(0, 0, 0);

	_buffers[0]->RenderToFSQ();

	effect->UnbindTexture(0);

	UnbindShader();
}

float PixelationEffect::GetIntensity() const {
	return m_intensity;
}

void PixelationEffect::SetIntensity(float intensity) {
	m_intensity = intensity;
}
