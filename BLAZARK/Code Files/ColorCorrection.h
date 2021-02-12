#pragma once

#include "Effect.h"


class LUT3D
{
public:

	LUT3D() = default;
	LUT3D(std::string filename);

	void LoadFromFile(std::string filname);
	void Bind();
	void Unbind();

	void Bind(unsigned slot);
	void Unbind(unsigned slot);

private:
	GLuint m_ID = GL_NONE;
	std::vector<glm::vec3> m_RGB;
};

class ColorCorrectionEffect : public PostEffect
{
public:

	~ColorCorrectionEffect();
	//Initializes framebuffer
//Overrides post effect Init
	void Init(unsigned width, unsigned height) override;

	//Applies the effect to this buffer
	//passes the previous framebuffer with the texture to apply as parameter
	void ApplyEffect(PostEffect* buffer) override;

	//Getters and setter
	LUT3D& GetLUT();
	unsigned& GetCurSlot();

	void Unload();
private:

	unsigned int _cur_Slot = 0;
	std::vector<LUT3D*> _LUT;

};

