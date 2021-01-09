#include "Shader.h"

Shader::Shader(const char* vertexShader, const char* fragmentShader) {
	GLuint vert = 0;
	GLuint frag = 0;

	vert = loadShader(GL_VERTEX_SHADER, vertexShader);
	frag = loadShader(GL_FRAGMENT_SHADER, fragmentShader);

	linkProgram(vert, frag);

	glDeleteShader(vert);
	glDeleteShader(frag);
}

Shader::~Shader() {
	glDeleteProgram(m_id);
}

void Shader::use() {
	glUseProgram(m_id);
}

void Shader::unuse() {
	glUseProgram(0);
}

void Shader::set1i(GLint value, const GLchar* name) {
	use();

	glUniform1i(glGetUniformLocation(m_id, name), value);

	unuse();
}

void Shader::set1f(GLfloat value, const GLchar* name) {
	use();

	glUniform1f(glGetUniformLocation(m_id, name), value);

	unuse();
}

void Shader::setVec2f(glm::fvec2 value, const GLchar* name) {
	use();

	glUniform2fv(glGetUniformLocation(m_id, name), 1, glm::value_ptr(value));

	unuse();
}

void Shader::setVec3f(glm::fvec3 value, const GLchar* name) {
	use();

	glUniform3fv(glGetUniformLocation(m_id, name), 1, glm::value_ptr(value));

	unuse();
}

void Shader::setVec4f(glm::fvec4 value, const GLchar* name) {
	use();

	glUniform4fv(glGetUniformLocation(m_id, name), 1, glm::value_ptr(value));

	unuse();
}

void Shader::setMat3fv(glm::mat3 value, const GLchar* name, GLboolean transpose) {
	use();

	glUniformMatrix3fv(glGetUniformLocation(m_id, name), 1, transpose, glm::value_ptr(value));

	unuse();
}

void Shader::setMat4fv(glm::mat4 value, const GLchar* name, GLboolean transpose) {
	use();

	glUniformMatrix4fv(glGetUniformLocation(m_id, name), 1, transpose, glm::value_ptr(value));

	unuse();
}

std::string Shader::loadShaderSource(const char* fileName) {
	std::string temp = "";
	std::string src = "";
	std::ifstream in_file;

	in_file.open(fileName);

	if (in_file.is_open()) {
		while (std::getline(in_file, temp)) {
			src += temp + "\n";
		}
	}
	else {
		std::cout << "ERROR::SHADER::COULD_NOT_OPEN_FILE: " << fileName << "\n";
	}

	in_file.close();

	src.replace(src.find("#version"), 12, "#version 440");

	return src;
}

GLuint Shader::loadShader(GLenum type, const char* fileName) {
	char infoLog[512];
	GLint success;
	GLuint shader = glCreateShader(type);
	std::string str_src = this->loadShaderSource(fileName);
	const GLchar* src = str_src.c_str();

	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COULD_NOT_COMPILE_SHADER: " << fileName << "\n";
		std::cout << infoLog << "\n";
	}

	return shader;
}

void Shader::linkProgram(GLuint vertexShader, GLuint fragmentShader) {
	char infoLog[512];
	GLint success;

	m_id = glCreateProgram();

	glAttachShader(m_id, vertexShader);
	glAttachShader(m_id, fragmentShader);
	glLinkProgram(m_id);
	glGetProgramiv(m_id, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(m_id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COULD_NOT_LINK_PROGRAM" << "\n";
		std::cout << infoLog << "\n";
	}

	glUseProgram(0);
}
