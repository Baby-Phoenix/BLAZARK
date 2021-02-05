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


	Texture(std::vector<const GLchar*> faces);


	~Texture();

	inline GLuint& getID();

	void bind(const GLint texture_unit);


	void unbind();

	void loadFromFile(const char* fileName);

	static GLuint LoadCubemap(std::vector<const GLchar* > faces);


private:
	GLuint id;
	int width;
	int height;
};

