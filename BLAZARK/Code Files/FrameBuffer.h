#pragma once

#include "Texture.h"
#include "Shader.h"

struct Depth
{
	~Depth();
	void UnloadDepth();
	Texture m_texture;
};

struct Color
{
	~Color();

	void UnloadColor();
	std::vector<Texture> m_texture;
	std::vector<GLenum> m_formats;
	std::vector<GLenum> m_buffers;
	unsigned int m_numAttachments = 0;
};


class FrameBuffer 
{
	FrameBuffer();
	~FrameBuffer();

	void Unload();

	virtual void Init(unsigned width, unsigned height);

	void Init();

	void AddDepth();

	void AddColor(GLenum format);

	
	void BindDepthTexture(int textureSlot) const;
	void BindColorTexture(unsigned colorBuffer, int textureSlot) const;
	void UnbindTexture(int textureSlot) const;

	void ReshapeBuffer(unsigned int width, unsigned int height);
	
	void SetSize(unsigned width, unsigned height);

	void SetViewport() const;

	void Bind() const;
	void Unbind() const;

	//Renders the framebuffer to our FullScreenQuad
	void RenderToFSQ() const;

	void DrawToBackbuffer();

	void Clear();
	//Checks to make sure the framebuffer is... OK
	bool CheckFBO();

	//Initializes fullscreen quad
	//*Creates VAO for full screen quad
	//*covers -1 to 1 range
	static void InitFullscreenQuad();
	//Draws our fullscreen quad
	static void DrawFullscreenQuad();

	//Initial width and height is zero
	unsigned int m_width = 0;
	unsigned int m_height = 0;
protected:
	//OpenGL framebuffer handle
	GLuint m_FBO;
	//Depth attachment (either one or none)
	Depth m_depth;
	//Color attachments (can be either 1 or above
	Color m_color;

	//Default filter is GL_NEAREST
	GLenum m_filter = GL_NEAREST;
	//Default filter is GL_CLAMP_TO_EDGE
	GLenum m_wrap = GL_CLAMP_TO_EDGE;

	//Clearflag is nothing by default
	GLbitfield m_clearFlag = 0;

	//Is the framebuffer initialized
	bool m_isInit = false;
	//Depth attachment?
	bool m_depthActive = false;

	//Full screen quad VBO handle
	static GLuint m_fullscreenQuadVBO;
	//Full screen quad VAO handle
	static GLuint m_fullscreenQuadVAO;

	//The maximum amount of color attachments
	static int m_maxColorAttachments;
	//Is the fullscreen quad initialized
	static bool m_isInitFSQ;
};