#include "SepiaEffect.h"

void SepiaEffect::Init(unsigned width, unsigned height)
{
	int index = int(_buffers.size());
	_buffers.push_back(new FrameBuffer());
	_buffers[index]->AddColorTarget(GL_RGBA8);
	_buffers[index]->AddDepthTarget();
	_buffers[index]->Init(width, height);

	//Set up shaders
	_shaders.push_back(new Shader("Resource Files/Shaders/passthrough_vert.glsl", "Resource Files/Shaders/PostEffects/sepia_frag.glsl"));

	PostEffect::Init(width, height);
}

void SepiaEffect::ApplyEffect(PostEffect* buffer)
{
	BindShader(0);
	_shaders[0]->set1f(_intensity, "u_Intensity");

	buffer->BindColorAsTexture(0, 0, 0);

	_buffers[0]->RenderToFSQ();

	buffer->UnbindTexture(0);

	UnbindShader();
}

float SepiaEffect::GetIntensity() const
{
	return _intensity;
}

void SepiaEffect::SetIntensity(float intensity)
{
	_intensity = intensity;
}
