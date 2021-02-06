#pragma once

#include "Effect.h"

class GreyscaleEffect : public Effect
{
public: 
	//Initialize framebuffer
	void Init(unsigned width, unsigned height) override;

	//Applies the effect to this buffer
	void ApplyEffect(Effect* buffer) override;

	void Draw() override;

	float GetIntensity() const;

	void SetIntensity(float intensity);

private:
	float m_intensity = 1.0f;
};
