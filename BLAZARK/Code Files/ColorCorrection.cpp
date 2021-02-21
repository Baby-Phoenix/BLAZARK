#include "ColorCorrection.h"

ColorCorrectionEffect::~ColorCorrectionEffect()
{
	Unload();
}

void ColorCorrectionEffect::Init(unsigned width, unsigned height)
{
	int index = int(_buffers.size());
	_buffers.push_back(new FrameBuffer());
	_buffers[index]->AddColorTarget(GL_RGBA8);
	_buffers[index]->AddDepthTarget();
	_buffers[index]->Init(width, height);

	_shaders.push_back(new Shader("Resource Files/Shaders/passthrough_vert.glsl", "Resource Files/Shaders/PostEffects/color_correction_frag.glsl"));
	PostEffect::Init(width, height);

}

void ColorCorrectionEffect::ApplyEffect(PostEffect* buffer)
{
	BindShader(0);
	buffer->BindColorAsTexture(0, 0, 0);
	_LUT[_cur_Slot]->Bind(30);

	_buffers[0]->RenderToFSQ();

	_LUT[_cur_Slot]->Unbind(30);
	buffer->UnbindTexture(0);
	UnbindShader();
}

LUT3D& ColorCorrectionEffect::GetLUT()
{
	return *_LUT[_cur_Slot];
}

unsigned& ColorCorrectionEffect::GetCurSlot()
{
	return _cur_Slot;
}

void ColorCorrectionEffect::AddLUT(std::string filename)
{
	_LUT.push_back(new LUT3D(filename));
}

void ColorCorrectionEffect::Unload()
{
	for (unsigned i = 0; i < _LUT.size(); i++) {
		delete _LUT[i];
	}
	_LUT.clear();
}

LUT3D::LUT3D(std::string filename)
{
	LoadFromFile(filename);
}

void LUT3D::LoadFromFile(std::string filname)
{
	std::string filePath = filname;
	std::ifstream LUTstream;
	LUTstream.open(filePath);

	while (!LUTstream.eof())
	{
		std::string _line;
		std::getline(LUTstream, _line);

		if (_line.empty())
			continue;

		glm::vec3 lineData;
		if (sscanf_s(_line.c_str(), "%f %f %f", &lineData.x, &lineData.y, &lineData.z) == 3)
			m_RGB.push_back(lineData);
	}
	
	std::cout << filname << " loaded successfully\n";

	glEnable(GL_TEXTURE_3D);

	glGenTextures(1, &m_ID);
	Bind();
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);

	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, 64, 64, 64, 0, GL_RGB, GL_FLOAT, &m_RGB[0]);
	Unbind();

	glDisable(GL_TEXTURE_3D);
}

void LUT3D::Bind()
{
	glBindTexture(GL_TEXTURE_3D, m_ID);
}

void LUT3D::Unbind()
{
	glBindTexture(GL_TEXTURE_3D, GL_NONE);
}

void LUT3D::Bind(unsigned slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	Bind();
}

void LUT3D::Unbind(unsigned slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	Unbind();
}
