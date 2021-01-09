#include "Skybox.h"

void Skybox::Init()
{
	m_shader = new Shader("Resource Files/Shaders/skybox_vert_shader.glsl", "Resource Files/Shaders/skybox_frag_shader.glsl");

	for (size_t i = 0; i < 108; i++)
	{
		skyboxVertices[i] *= 10;
	}

	//Setup VAO
	GLuint skyboxVBO;
	glGenVertexArrays(1, &this->m_skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(this->m_skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->m_skyboxVAO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);

	//Load images
	std::vector<const GLchar*> faces;
	faces.push_back("Resource Files/Textures/UniverseOne/SkyBox/1.png");
	faces.push_back("Resource Files/Textures/UniverseOne/SkyBox/2.png");
	faces.push_back("Resource Files/Textures/UniverseOne/SkyBox/3.png");
	faces.push_back("Resource Files/Textures/UniverseOne/SkyBox/4.png");
	faces.push_back("Resource Files/Textures/UniverseOne/SkyBox/5.png");
	faces.push_back("Resource Files/Textures/UniverseOne/SkyBox/6.png");
	this->m_cubemapTexture = LoadCubemap(faces);
}

void Skybox::Update(glm::mat4 camViewMatrix, glm::mat4* ProjectionMatrix)
{
	glm::mat4 ViewMatrix;

	//Draw Skybox
	glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
	m_shader->use();
	ViewMatrix = glm::mat4(glm::mat3(camViewMatrix));	// Remove any translation component of the view matrix
	glUniformMatrix4fv(glGetUniformLocation(m_shader->m_id, "ViewMatrix"), 1, GL_FALSE, glm::value_ptr(ViewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(m_shader->m_id, "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(*ProjectionMatrix));
	//skybox cube
	glBindVertexArray(this->m_skyboxVAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->m_cubemapTexture);
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
