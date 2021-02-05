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
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
}

void FrameBuffer::ReshapeBuffer(unsigned int width, unsigned int height)
{
	SetSize(width, height);
	Unload();
	m_depth.UnloadDepth();
	m_color.UnloadColor();

	Init();


}

void FrameBuffer::SetSize(unsigned width, unsigned height)
{
	m_width = width;
	m_height = height;
}

void FrameBuffer::SetViewport() const
{
	glViewport(0, 0, m_width, m_height);
}

void FrameBuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	if (m_color.m_numAttachments)
	{
		glDrawBuffers(m_color.m_numAttachments, &m_color.m_buffers[0]);
	}

}

void FrameBuffer::Unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

void FrameBuffer::RenderToFSQ() const
{
	SetViewport();
	Bind();
	DrawFullscreenQuad();
	Unbind();
}

void FrameBuffer::DrawToBackbuffer()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, GL_NONE);

	glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, GL_NONE);
}

void FrameBuffer::Clear()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClear(m_clearFlag);
	Unbind();
}

bool FrameBuffer::CheckFBO()
{
	Bind();

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Framebuffer is not ok\n";
		return false;
	}
	return true;
}

void FrameBuffer::InitFullscreenQuad()
{

	float VBO_DATA[]
	{
		-1.f, -1.f, 0.f,
		1.f, -1.f, 0.f,
		-1.f, 1.f, 0.f,

		1.f, 1.f, 0.f,
		-1.f, 1.f, 0.f,
		1.f, -1.f, 0.f,

		0.f, 0.f,
		1.f, 0.f,
		0.f, 1.f,

		1.f, 1.f,
		0.f, 1.f,
		1.f, 0.f
	};

	//Vertex size is 6pts * 3 data points * sizeof (float)
	int vertexSize = 6 * 3 * sizeof(float);
	//texcoord size = 6pts * 2 data points * sizeof(float)
	int texCoordSize = 6 * 2 * sizeof(float);

	//Generates vertex array
	glGenVertexArrays(1, &m_fullscreenQuadVAO);
	//Binds VAO
	glBindVertexArray(m_fullscreenQuadVAO);

	//Enables 2 vertex attrib array slots
	glEnableVertexAttribArray(0); //Vertices
	glEnableVertexAttribArray(1); //UVS

	//Generates VBO
	glGenBuffers(1, &m_fullscreenQuadVBO);

	//Binds the VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_fullscreenQuadVBO);
	//Buffers the vbo data
	glBufferData(GL_ARRAY_BUFFER, vertexSize + texCoordSize, VBO_DATA, GL_STATIC_DRAW);

	//Sets first attrib array to point to the beginning of the data
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));
	//Sets the second attrib array to point to an offset in the data
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(vertexSize));


	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
	glBindVertexArray(GL_NONE);

}

void FrameBuffer::DrawFullscreenQuad()
{
	glBindVertexArray(m_fullscreenQuadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(GL_NONE);
}
