#pragma once

#include <vector>
#include "Shader.h"
#include "Random.h"
#include "Texture.h";
#include "Transform.h"

class ParticleEmitter
{
public:
	ParticleEmitter(int emitterType);
	~ParticleEmitter();
	void init();


	void update(const float dt);

	void setRadius(float radius); //Only emitterType 1 has a radius
	void setAngleDeg(float degrees); //Only emitterType 1 has a degrees

	void setSpeed(float speed);
	void setLifetime(float min, float max);

	void setControllerPos(glm::vec3 pos);

	float getEmitterType();
	bool getDone();
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
	bool m_isDone = false;
	float m_waveIndex = 0;
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



	ParticleController(int emitterType, glm::vec3 position, Texture* texture, entt::entity parent);
	~ParticleController();

	void setPosition(glm::vec3 position);
	void setRotation(glm::vec3 rotation);
	void setSize(float size);
	void setColor(glm::vec4 startColor, glm::vec4 endColor);
	void setModelMatrix(glm::mat4 matrix);

	ParticleEmitter* getEmitter();

	void update(const float dt, const glm::mat4 ProjectionMatrix, const glm::mat4& ViewMatrix);
	void draw();

private:
	Shader* m_shader;
	ParticleEmitter* m_emitter;
	GLuint m_VAO;

	glm::mat4 m_modelMatrix;
	entt::entity m_parentEntity;
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;
	glm::vec4 m_startColor;
	glm::vec4 m_endColor;
	Texture* m_tex;
	float m_size;

	//Projectile* m_next = nullptr;
};