#pragma once

#include <glad/glad.h>
#include <glfw3.h>
#include <glm.hpp>

#include <vector>

#include "Dependencies/tinygltf/stb_image.h"
#include "Shader.h"

class Skybox
{
public:

	Skybox() = default;
	~Skybox() = default;

	static void Init();

	static void Update(glm::mat4 camViewMatrix, glm::mat4* ProjectionMatrix);

private:
	static GLuint LoadCubemap(std::vector<const GLchar*> faces);

	static GLuint m_skyboxVAO;
	static GLuint m_cubemapTexture;

	static Shader* m_shader;

};