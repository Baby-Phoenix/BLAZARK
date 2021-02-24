#include "BloomEffect.h"

void BloomEffect::Init(unsigned width, unsigned height)
{
	int index = _buffers.size();
	
	// 0 
	_buffers.push_back(new FrameBuffer());
	_buffers[index]->AddColorTarget(GL_RGBA8);
	_buffers[index]->Init(width, height);

	//1
	index++;
	_buffers.push_back(new FrameBuffer());
	_buffers[index]->AddColorTarget(GL_RGBA8);
	_buffers[index]->Init(width, height);
	index++;

	_shaders.push_back(new Shader("Resource Files/Shaders/passthrough_vert.glsl", "Resource Files/Shaders/passthrough_frag.glsl")); //0
	_shaders.push_back(new Shader("Resource Files/Shaders/passthrough_vert.glsl", "Resource Files/Shaders/PostEffects/BlurlFrag.glsl")); // 1
	_shaders.push_back(new Shader("Resource Files/Shaders/passthrough_vert.glsl", "Resource Files/Shaders/PostEffects/BloomHighPassFrag.glsl")); // 2 
	_shaders.push_back(new Shader("Resource Files/Shaders/passthrough_vert.glsl", "Resource Files/Shaders/PostEffects/BloomCompositeFrag.glsl")); //3 

}

void BloomEffect::ApplyEffect(PostEffect* buffer)
{
		//for the previous effect
		BindShader(0);

		buffer->BindColorAsTexture(0, 0, 0);

		_buffers[0]->RenderToFSQ();
		 
		buffer->UnbindTexture(0);

		UnbindShader();

		//the first shader -> high pass
		BindShader(2);
		_shaders[2]->set1f(_threshold, "threshold");

		BindColorAsTexture(0, 0, 0);

		_buffers[0]->RenderToFSQ();

		UnbindTexture(0);

		UnbindShader();


		for (unsigned int i = 0; i < _number_passes; i++) {

			// blur frag
			BindShader(1);
			_shaders[1]->set1i((i % 2 == 0), "horizontal");

			BindColorAsTexture(0, 0, 0);

			_buffers[1]->RenderToFSQ();

			UnbindTexture(0);

			UnbindShader();
		}

		//final composite
		BindShader(3);

		buffer->BindColorAsTexture(0, 0, 0);
		BindColorAsTexture(1, 0, 1);

		_buffers[0]->RenderToFSQ();

		UnbindTexture(1);
		UnbindTexture(0);

	UnbindShader();

}

float& BloomEffect::GetThreshold()
{
	return _threshold;
}

unsigned int& BloomEffect::GetNumberOfPasses()
{
	return _number_passes;
}

