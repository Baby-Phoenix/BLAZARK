#include "GreyscaleEffect.h"

void GreyscaleEffect::Init(unsigned width, unsigned height)
{
    int index = int(m_buffers.size());
    m_buffers.push_back(std::move(std::make_unique<FrameBuffer>()));
    m_buffers[index]->AddColor(GL_RGBA8);
    m_buffers[index]->Init(width, height);

    index = int(m_effect_shaders.size());
    m_effect_shaders.push_back(new Shader("Resource Files/Shaders/passthrough_vert.glsl", "Resource Files/Shaders/passthrough_frag.glsl"));
}

void GreyscaleEffect::ApplyEffect(Effect* previousEffect)
{
    BindShader(0);

    m_effect_shaders[0]->set1f(m_intensity, "u_Intensity");

    previousEffect->BindColorTexture(0, 0, 0);

    m_buffers[0]->RenderToFSQ();

    previousEffect->UnbindTexture(0);

    UnBindShader();
}

void GreyscaleEffect::Draw()
{
    BindShader(0);
    m_effect_shaders[0]->set1f(m_intensity, "u_Intensity");

    BindColorTexture(0, 0, 0);

    m_buffers[0]->DrawFullscreenQuad();

    UnbindTexture(0);

    UnBindShader();
}

float GreyscaleEffect::GetIntensity() const
{
    return m_intensity;
}

void GreyscaleEffect::SetIntensity(float intensity)
{
    m_intensity = intensity;
}