#include "FrameBuffer.h"

GLuint FrameBuffer::m_fullscreenQuadVBO = 0;
GLuint FrameBuffer::m_fullscreenQuadVAO = 0;

int FrameBuffer::m_maxColorAttachments = 0;
bool FrameBuffer::m_isInitFSQ = false;

Depth::~Depth()
{
	UnloadDepth();
}

void Depth::UnloadDepth()
{
	glDeleteTextures(1, &m_texture.getID());
}

Color::~Color()
{
	UnloadColor();
}

void Color::UnloadColor()
{
	glDeleteTextures(m_numAttachments, &m_texture[0].getID());
}

FrameBuffer::~FrameBuffer()
{
}

void FrameBuffer::Unload()
{
	glDeleteFramebuffers(1,&m_FBO);
	m_isInit = false;
}

void FrameBuffer::Init(unsigned width, unsigned height)
{
	SetSize(width,height);
	Init();
}

void FrameBuffer::Init()
{
	glGenFramebuffers(1, &m_FBO);

	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	if (m_depthActive) {

		m_clearFlag |= GL_DEPTH_BUFFER_BIT;

		glGenTextures(1, &m_depth.m_texture.getID());
		glBindTexture(GL_TEXTURE_2D, m_depth.m_texture.getID());
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT24, m_width, m_height);

		glTextureParameteri(m_depth.m_texture.getID(), GL_TEXTURE_MIN_FILTER, m_filter);
		glTextureParameteri(m_depth.m_texture.getID(), GL_TEXTURE_MAG_FILTER, m_filter);
		glTextureParameteri(m_depth.m_texture.getID(), GL_TEXTURE_WRAP_S, m_wrap);
		glTextureParameteri(m_depth.m_texture.getID(), GL_TEXTURE_WRAP_T, m_wrap);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth.m_texture.getID(), 0);
	
		glBindTexture(GL_TEXTURE_2D, GL_NONE);
	}

	if (m_color.m_numAttachments) {
		m_clearFlag |= GL_COLOR_BUFFER_BIT;

		//Creates the texture handles
		GLuint* texHandles = new GLuint[m_color.m_numAttachments];
		glGenTextures(m_color.m_numAttachments, texHandles);

		for (int i = 0; i < m_color.m_numAttachments; i++) {
			m_color.m_texture[i].getID() = texHandles[i];

			glBindTexture(GL_TEXTURE_2D, m_color.m_texture[i].getID());
			glTexStorage2D(GL_TEXTURE_2D, 1, m_color.m_formats[i], m_width, m_height);

			glTextureParameteri(m_color.m_texture[i].getID(), GL_TEXTURE_MIN_FILTER, m_filter);
			glTextureParameteri(m_color.m_texture[i].getID(), GL_TEXTURE_MAG_FILTER, m_filter);
			glTextureParameteri(m_color.m_texture[i].getID(), GL_TEXTURE_WRAP_S, m_filter);
			glTextureParameteri(m_color.m_texture[i].getID(), GL_TEXTURE_WRAP_T, m_filter);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_color.m_texture[i].getID(), 0);

		}
		delete[] texHandles;

	}

	CheckFBO();

	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
	m_isInit = true;
}

void FrameBuffer::AddDepth()
{
	if (m_depth.m_texture.getID()) {
		m_depth.UnloadDepth();
	}

	m_depthActive = true;
}

void FrameBuffer::AddColor(GLenum format)
{
	m_color.m_texture.resize(m_color.m_numAttachments + 1);
	m_color.m_formats.push_back(format);
	m_color.m_buffers.push_back(GL_COLOR_ATTACHMENT0 + m_color.m_numAttachments);

	m_color.m_numAttachments++;
}

void FrameBuffer::BindDepthTexture(int textureSlot) const
{

	m_depth.m_texture.bind(textureSlot);
}

void FrameBuffer::BindColorTexture(unsigned colorBuffer, int textureSlot) const
{
	m_color.m_texture[colorBuffer].bind(textureSlot);
}

void FrameBuffer::UnbindTexture(int textureSlot) const
{
}

void FrameBuffer::ReshapeBuffer(unsigned int width, unsigned int height)
{
}

void FrameBuffer::SetSize(unsigned width, unsigned height)
{
}

void FrameBuffer::SetViewport() const
{
}

void FrameBuffer::Bind() const
{
}

void FrameBuffer::Unbind() const
{
}

void FrameBuffer::RenderToFSQ() const
{
}

void FrameBuffer::DrawToBackbuffer()
{
}

void FrameBuffer::Clear()
{
}

bool FrameBuffer::CheckFBO()
{
	return false;
}

void FrameBuffer::InitFullscreenQuad()
{
}

void FrameBuffer::DrawFullscreenQuad()
{
}
