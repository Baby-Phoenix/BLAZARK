#pragma once

#include <iostream>
#include <string>
#include <glad/glad.h>
#include <glfw3.h>

#include <vector>

class Texture
{
public:
	Texture(const char* fileName);
	Texture() { width = 0; height = 0; id = GL_NONE; }

	Texture(std::vector<const GLchar*> faces);


	~Texture();

	GLuint& getID();

	void bind(const GLint texture_unit) const;


	void unbind();

	void loadFromFile(const char* fileName);

	static GLuint LoadCubemap(std::vector<const GLchar* > faces);


private:
	GLuint id;
	int width;
	int height;
};

