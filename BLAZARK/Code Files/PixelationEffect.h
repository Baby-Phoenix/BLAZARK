#pragma once

#include "Effect.h"

class PixelationEffect : public PostEffect {
public:
	// Initializes the framebuffer
	void Init(unsigned width, unsigned height) override;

	// Applies the effect to this buffer
	void ApplyEffect(PostEffect* effect) override;

	// Getter
	float GetIntensity() const;

	// Setter
	void SetIntensity(float intensity);

private:
	float m_intensity = 0.0;
};