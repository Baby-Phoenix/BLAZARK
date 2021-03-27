#pragma once

#include "glm.hpp"
#include <vector>
#include "Shader.h"
#include "Random.h"
#include "Texture.h";

class ParticleEmitter
{
public:
	ParticleEmitter(int emitterType);
	~ParticleEmitter();

	void update(const float dt);

	void setRadius(float radius); //Only emitterType 1 has a radius
	void setAngleDeg(float degrees); //Only emitterType 1 has a degrees

	void setSpeed(float speed);
	void setLifetime(float min, float max);

	void setControllerPos(glm::vec3 pos);
	void draw();
	

private:
	struct Particle
	{
		glm::vec3 m_position;
		glm::vec3 m_velocity;
		glm::vec4 m_color;
		float m_lifetime;
	};

	std::vector<Particle> m_particles;

	std::vector< float > m_vertices;

	float m_emitterType;
	float m_radius = 1;
	float m_positions[8000];
	float m_speed = 1.0f;
	float m_minLife = 1;
	float m_maxLife = 7;
	float m_degrees = 15;
	glm::vec3 m_controllerPos;
	GLuint m_vertexBuffer;
	GLuint m_positionBuffer;

	GLuint m_VAO;
};

enum ParticleName
{
	PLAYER_CENTER_LEFT = 0,
	PLAYER_CENTER_RIGHT,
};

class ParticleController
{
public:



	ParticleController(int emitterType, glm::vec3 position, Texture* texture);
	~ParticleController();

	void setPosition(glm::vec3 position);
	void setRotation(glm::vec3 rotation);
	void setSize(float size);
	void setColor(glm::vec4 startColor, glm::vec4 endColor);

	ParticleEmitter* getEmitter();

	void update(const float dt, const glm::mat4 ProjectionMatrix, const glm::mat4& ViewMatrix, glm::mat4 ParentMatrix);
	void draw();

private:
	Shader* m_shader;
	ParticleEmitter* m_emitter;
	GLuint m_VAO;

	glm::mat4 m_modelMatrix;
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;
	glm::vec4 m_startColor;
	glm::vec4 m_endColor;
	Texture* m_tex;
	float m_size;

	ParticleController* m_next = nullptr;
};