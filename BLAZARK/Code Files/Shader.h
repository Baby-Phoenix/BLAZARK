#pragma once

#include <glad/glad.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <string>

class Shader {
public:
	GLuint m_id;

	Shader(const char* vertexShader, const char* fragmentShader, const char* geometryShader = "");
	~Shader();

	void use();
	void unuse();

	// Uniform Setters //
	void set1i(GLint value, const GLchar* name);

	void set1f(GLfloat value, const GLchar* name);

	void setVec2f(glm::fvec2 value, const GLchar* name);

	void setVec3f(glm::fvec3 value, const GLchar* name);

	void setVec4f(glm::fvec4 value, const GLchar* name);

	void setMat3fv(glm::mat3 value, const GLchar* name, GLboolean transpose = GL_FALSE);

	void setMat4fv(glm::mat4 value, const GLchar* name, GLboolean transpose = GL_FALSE);

private:
	std::string loadShaderSource(const char* fileName);

	GLuint loadShader(GLenum type, const char* fileName);

	void linkProgram(GLuint vertexShader, GLuint geometryShader, GLuint fragmentShader);
};