#include "Framebuffer.h"

GLuint FrameBuffer::_fullscreenQuadVBO = 0;
GLuint FrameBuffer::_fullscreenQuadVAO = 0;

int FrameBuffer::_maxColorAttachments = 0;
bool FrameBuffer::_isInitFSQ = false;

DepthTarget::~DepthTarget()
{
	//Unloads the depth target
	Unload();
}

void DepthTarget::Unload()
{
	//Deletes the texture at the specific handle
	glDeleteTextures(1, &_texture.getID());
}

ColorTarget::~ColorTarget()
{
	//Unloads the color target
	Unload();
}

void ColorTarget::Unload()
{
	glDeleteTextures(_numAttachments, &_textures[0].getID());
}

void FrameBuffer::Unload()
{
	//Deletes the framebuffer
	glDeleteFramebuffers(1, &_FBO);
	//Sets init to false
	_isInit = false;
}

void FrameBuffer::Init(unsigned width, unsigned height, bool isDepthBuffer)
{
	//Sets the size to width and height
	SetSize(width, height);

	_isDepthBuffer = isDepthBuffer;

	//Inits framebuffer
	Init();
}

void FrameBuffer::Init()
{
	//Generates the FBO
	glGenFramebuffers(1, &_FBO);
	//Bind it
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);

	if (_depthActive)
	{
		if (_isDepthBuffer) {
			_clearFlag |= GL_DEPTH_BUFFER_BIT;

			glGenTextures(1, &_depth._texture.getID());

			glBindTexture(GL_TEXTURE_CUBE_MAP, _depth._texture.getID());
			for (unsigned int i = 0; i < 6; i++)
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, _width, _height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
				//glTexStorage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 1, GL_DEPTH_COMPONENT24, _width, _height);

			glTextureParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, _filter);
			glTextureParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, _filter);
			glTextureParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, _wrap);
			glTextureParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, _wrap);
			glTextureParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, _wrap);
			/*glTextureParameteri(_depth._texture.getID(), GL_TEXTURE_MIN_FILTER, _filter);
			glTextureParameteri(_depth._texture.getID(), GL_TEXTURE_MAG_FILTER, _filter);
			glTextureParameteri(_depth._texture.getID(), GL_TEXTURE_WRAP_S, _wrap);
			glTextureParameteri(_depth._texture.getID(), GL_TEXTURE_WRAP_T, _wrap);
			glTextureParameteri(_depth._texture.getID(), GL_TEXTURE_WRAP_R, _wrap);*/

			glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _depth._texture.getID(), 0);

			glBindTexture(GL_TEXTURE_CUBE_MAP, GL_NONE);
		}
		else {
			//Because we have depth we need to clear our depth bit
			_clearFlag |= GL_DEPTH_BUFFER_BIT;

			//Generate the texture
			glGenTextures(1, &_depth._texture.getID());
			//Binds the texture
			glBindTexture(GL_TEXTURE_2D, _depth._texture.getID());
			//Sets the texture data
			glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT24, _width, _height);

			//Set texture parameters
			glTextureParameteri(_depth._texture.getID(), GL_TEXTURE_MIN_FILTER, _filter);
			glTextureParameteri(_depth._texture.getID(), GL_TEXTURE_MAG_FILTER, _filter);
			glTextureParameteri(_depth._texture.getID(), GL_TEXTURE_WRAP_S, _wrap);
			glTextureParameteri(_depth._texture.getID(), GL_TEXTURE_WRAP_T, _wrap);

			//Sets up as a framebuffer texture
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depth._texture.getID(), 0);

			glBindTexture(GL_TEXTURE_2D, GL_NONE);
		}
	}

	//If there is more than zero color attachments
		//We create them
	if (_color._numAttachments)
	{
		//Because we have a color target we include a color buffer bit into clear flag
		_clearFlag |= GL_COLOR_BUFFER_BIT;
		//Creates the GLuints to hold the new texture handles;
		GLuint* textureHandles = new GLuint[_color._numAttachments];

		glGenTextures(_color._numAttachments, textureHandles);

		//Loops through them
		for (unsigned i = 0; i < _color._numAttachments; i++)
		{
			_color._textures[i].getID() = textureHandles[i];

			//Binds the texture
			glBindTexture(GL_TEXTURE_2D, _color._textures[i].getID());
			//Sets the texture storage
			glTexStorage2D(GL_TEXTURE_2D, 1, _color._formats[i], _width, _height);

			//Set texture parameters
			glTextureParameteri(_color._textures[i].getID(), GL_TEXTURE_MIN_FILTER, _filter);
			glTextureParameteri(_color._textures[i].getID(), GL_TEXTURE_MAG_FILTER, _filter);
			glTextureParameteri(_color._textures[i].getID(), GL_TEXTURE_WRAP_S, _wrap);
			glTextureParameteri(_color._textures[i].getID(), GL_TEXTURE_WRAP_T, _wrap);

			//Sets up as a framebuffer texture
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, _color._textures[i].getID(), 0);
		}

		delete[] textureHandles;
	}

	//Make sure it's set up right
	CheckFBO();
	//Unbind buffer
	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
	//Set init to true
	_isInit = true;
}

void FrameBuffer::AddDepthTarget()
{
	//If there is a handle already, unload it
	if (_depth._texture.getID())
	{
		_depth.Unload();
	}
	//Make depth active true
	_depthActive = true;
}

void FrameBuffer::AddColorTarget(GLenum format)
{
	//Resizes the textures to number of attachments
	_color._textures.resize(_color._numAttachments + 1);
	//Add the format
	_color._formats.push_back(format);
	//Add the color attachment buffer number
	_color._buffers.push_back(GL_COLOR_ATTACHMENT0 + _color._numAttachments);
	//Incremenets number of attachments
	_color._numAttachments++;
}

void FrameBuffer::BindDepthAsTexture(int textureSlot) const
{
	_depth._texture.bind(textureSlot);
}

void FrameBuffer::BindColorAsTexture(unsigned colorBuffer, int textureSlot) const
{
	_color._textures[colorBuffer].bind(textureSlot);
}

void FrameBuffer::UnbindTexture(int textureSlot) const
{
	//Binds textures to GL_NONE
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
}

void FrameBuffer::Reshape(unsigned width, unsigned height)
{
	//Set size
	SetSize(width, height);
	//Unloads the framebuffer
	Unload();
	//Unload the depth target
	_depth.Unload();
	//Unloads the color target
	_color.Unload();
	//Inits the framebuffer
	Init();
}

void FrameBuffer::SetSize(unsigned width, unsigned height)
{
	//Sets the width and height
	_width = width;
	_height = height;
}

void FrameBuffer::SetViewport() const
{
	glViewport(0, 0, _width, _height);
}

void FrameBuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);

	if (_color._numAttachments)
	{
		glDrawBuffers(_color._numAttachments, &_color._buffers[0]);
	}

	if (_isDepthBuffer) {
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}
}

void FrameBuffer::Unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

void FrameBuffer::RenderToFSQ() const
{
	//Sets viewport
	SetViewport();
	//Bind the framebuffer
	Bind();
	//Draw full screen quad
	DrawFullscreenQuad();
	//Unbind the framebuffer
	Unbind();
}

void FrameBuffer::DrawToBackbuffer()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, _FBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, GL_NONE);

	//Blits the framebuffer to the back buffer
	glBlitFramebuffer(0, 0, _width, _height, 0, 0, _width, _height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, GL_NONE);
}

void FrameBuffer::Clear()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
	glClear(_clearFlag);
	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

bool FrameBuffer::CheckFBO()
{
	//Binds the framebuffer
	Bind();
	//Check the framebuffer status
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Framebuffer is not vibing\n");
		return false;
	}
	return true;
}

void FrameBuffer::InitFullscreenQuad()
{
	//A vbo with Uvs and verts from
	//-1 to 1 for verts
	//0 to 1 for UVs
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
	glGenVertexArrays(1, &_fullscreenQuadVAO);
	//Binds VAO
	glBindVertexArray(_fullscreenQuadVAO);

	//Enables 2 vertex attrib array slots
	glEnableVertexAttribArray(0); //Vertices
	glEnableVertexAttribArray(1); //UVS

	//Generates VBO
	glGenBuffers(1, &_fullscreenQuadVBO);

	//Binds the VBO
	glBindBuffer(GL_ARRAY_BUFFER, _fullscreenQuadVBO);
	//Buffers the vbo data
	glBufferData(GL_ARRAY_BUFFER, vertexSize + texCoordSize, VBO_DATA, GL_STATIC_DRAW);

#pragma warning(push)
#pragma warning(disable : 4312)
	//Sets first attrib array to point to the beginning of the data
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));
	//Sets the second attrib array to point to an offset in the data
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(vertexSize));
#pragma warning(pop)

	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
	glBindVertexArray(GL_NONE);
}

void FrameBuffer::DrawFullscreenQuad()
{
	glBindVertexArray(_fullscreenQuadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(GL_NONE);
}



