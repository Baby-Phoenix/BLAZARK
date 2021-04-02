#include "Skybox.h"

GLuint Skybox::m_skyboxVAO = NULL;
GLuint Skybox::m_cubemapTexture = NULL;
Shader* Skybox::m_shader = nullptr;

void Skybox::Init(std::string scenename)
{
	m_shader = new Shader("Resource Files/Shaders/skybox_vert_shader.glsl", "Resource Files/Shaders/skybox_frag_shader.glsl");

	GLfloat skyboxVertices[108] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	for (size_t i = 0; i < 108; i++)
	{
		skyboxVertices[i] *= 10;
	}

	//Setup VAO
	GLuint skyboxVBO;
	glGenVertexArrays(1, &m_skyboxVAO);
	glBindVertexArray(m_skyboxVAO);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &skyboxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
	glBindVertexArray(GL_NONE);

	//Load images
	std::vector<const GLchar*> faces;

	if (scenename == "Universe_19")
	{
		faces.push_back("Resource Files/Textures/Universe-19/SkyBox/1.png");
		faces.push_back("Resource Files/Textures/Universe-19/SkyBox/2.png");
		faces.push_back("Resource Files/Textures/Universe-19/SkyBox/3.png");
		faces.push_back("Resource Files/Textures/Universe-19/SkyBox/4.png");
		faces.push_back("Resource Files/Textures/Universe-19/SkyBox/5.png");
		faces.push_back("Resource Files/Textures/Universe-19/SkyBox/6.png");
	}
	else if (scenename == "Universe_27")
	{
		faces.push_back("Resource Files/Textures/Universe-27/SkyBox/1.png");
		faces.push_back("Resource Files/Textures/Universe-27/SkyBox/2.png");
		faces.push_back("Resource Files/Textures/Universe-27/SkyBox/3.png");
		faces.push_back("Resource Files/Textures/Universe-27/SkyBox/4.png");
		faces.push_back("Resource Files/Textures/Universe-27/SkyBox/5.png");
		faces.push_back("Resource Files/Textures/Universe-27/SkyBox/6.png");
	}
	else if (scenename == "Universe_5")
	{
		faces.push_back("Resource Files/Textures/Universe-5/SkyBox/1.png");
		faces.push_back("Resource Files/Textures/Universe-5/SkyBox/2.png");
		faces.push_back("Resource Files/Textures/Universe-5/SkyBox/3.png");
		faces.push_back("Resource Files/Textures/Universe-5/SkyBox/4.png");
		faces.push_back("Resource Files/Textures/Universe-5/SkyBox/5.png");
		faces.push_back("Resource Files/Textures/Universe-5/SkyBox/6.png");
	}

	m_cubemapTexture = LoadCubemap(faces);
}

void Skybox::Draw(glm::mat4 camViewMatrix, glm::mat4 ProjectionMatrix)
{
	glm::mat4 ViewMatrix;

	//Draw Skybox
	glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
	m_shader->use();
	ViewMatrix = glm::mat4(glm::mat3(camViewMatrix));	// Remove any translation component of the view matrix
	glUniformMatrix4fv(glGetUniformLocation(m_shader->m_id, "ViewMatrix"), 1, GL_FALSE, glm::value_ptr(ViewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(m_shader->m_id, "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
	//skybox cube
	glBindVertexArray(m_skyboxVAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // Set depth function back to default

	//Reset
	glUseProgram(0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Skybox::LoadCubemap(std::vector<const GLchar*> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	int imageWidth, imageHeight;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = stbi_load(faces[i], &imageWidth, &imageHeight, 0, 3);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		stbi_image_free(image);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}
