#pragma once

#include"Effect.h"

class BloomEffect : public PostEffect
{
public:

	//Initializes framebuffer
	//Ovverides post effect Init
	void Init(unsigned width, unsigned height) override;

	//Applies the effect to this buffer
	//passes the previous framebuffer with the texture to apply as parameter
	void ApplyEffect(PostEffect* buffer) override;

	//Getters
	float& GetThreshold();
	unsigned int& GetNumberOfPasses();
	
private:
	
	float _threshold = 0.0f;
	unsigned int _number_passes = 50;
};