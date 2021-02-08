#include "GreyscaleEffect.h"

void GreyscaleEffect::Init(unsigned width, unsigned height)
{
	int index = int(_buffers.size());
	_buffers.push_back(new FrameBuffer());
	_buffers[index]->AddColor(GL_RGBA8);
	_buffers[index]->AddDepth();
	_buffers[index]->Init(width, height);

	_intensity = 1.0;

	index = int(_shaders.size());
	_shaders.push_back(new Shader("Resource Files/Shaders/passthrough_vert.glsl", "Resource Files/Shaders/greyscale_frag.glsl"));

	PostEffect::Init(width, height);
}

void GreyscaleEffect::ApplyEffect(PostEffect* buffer)
{
	BindShader(0);

	_shaders[0]->set1f(_intensity, "u_Intensity");

	buffer->BindColorAsTexture(0, 0, 0);

	_buffers[0]->RenderToFSQ();

	buffer->UnbindTexture(0);

	UnbindShader();
}

void GreyscaleEffect::DrawToScreen()
{
	BindShader(0);
	_shaders[0]->set1f(_intensity, "U_Intensity");

	BindColorAsTexture(0, 0, 0);

	_buffers[0]->DrawFullscreenQuad();

	UnbindTexture(0);

	UnbindShader();
}

float GreyscaleEffect::GetIntensity() const
{
	return _intensity;
}

void GreyscaleEffect::SetIntensity(float intensity)
{
	_intensity = intensity;
}
