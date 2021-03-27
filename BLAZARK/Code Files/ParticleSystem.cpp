#include "ParticleSystem.h"

ParticleEmitter::ParticleEmitter(int emitterType)
{
	// create 200 particles
	m_particles.resize(2000);

	m_emitterType = emitterType;

	glCreateVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	//Create a vertex and position buffer
	glGenBuffers(1, &m_vertexBuffer);
	glGenBuffers(1, &m_positionBuffer);

	m_vertices.push_back(0.0f);
	m_vertices.push_back(0.0f);
	m_vertices.push_back(0.0f);

	m_vertices.push_back(1.0f);
	m_vertices.push_back(0.0f);
	m_vertices.push_back(0.0f);

	m_vertices.push_back(0.0f);
	m_vertices.push_back(1.0f);
	m_vertices.push_back(0.0f);

	m_vertices.push_back(1.0f);
	m_vertices.push_back(1.0f);
	m_vertices.push_back(0.0f);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), m_vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	// fill the position buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_particles.size() * 4 * sizeof(float), m_positions, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
	glVertexAttribDivisor(4, 1);

	glBindVertexArray(0);
	
}

ParticleEmitter::~ParticleEmitter()
{
	glDeleteBuffers(1, &m_vertexBuffer);
	glDeleteBuffers(1, &m_positionBuffer);
}

void ParticleEmitter::init()
{
	for (int i = 0; i < m_particles.size(); ++i)
	{


		if (m_emitterType == 0) //QUAD EMITTER
		{

			m_particles[i].m_position = Random::Range3f(-10, 10);
			m_particles[i].m_lifetime = Random::Range1f(m_minLife, m_maxLife);

		}
		else if (m_emitterType == 1) //CONE EMITTER
		{

			float tanTheta = glm::tan(glm::radians(m_degrees));


			glm::vec2 tempVec2 = Random::Circle2f(m_radius);

			m_particles[i].m_velocity = m_speed * glm::normalize(glm::vec3(tanTheta * tempVec2.x, tanTheta * tempVec2.y, 1.0f));
			m_particles[i].m_position = glm::vec3(tempVec2.x, tempVec2.y, 0);
			m_particles[i].m_lifetime = Random::Range1f(m_minLife, m_maxLife);


		}
		else if (m_emitterType == 2) //Explosion EMITTER
		{



			glm::vec3 pos = Random::Range3f(-0.1, 0.1);
			pos += m_controllerPos;

			m_particles[i].m_velocity = m_speed * glm::normalize(pos - m_controllerPos);

			if (glm::sqrt((pos.x * pos.x) + (pos.y * pos.y) + (pos.z * pos.z)) < 1)
				m_particles[i].m_position = pos;

			m_particles[i].m_lifetime = Random::Range1f(m_minLife, m_maxLife);

		}
		else if (m_emitterType == 3) //Portal EMITTER
		{


			float tanTheta = glm::tan(glm::radians(m_degrees));


			glm::vec2 tempVec2 = Random::Circle2f(m_radius);

			m_particles[i].m_velocity = m_speed * glm::normalize(glm::vec3(tanTheta * tempVec2.x, tanTheta * tempVec2.y, 1.0f));
			m_particles[i].m_position = glm::vec3(tempVec2.x, tempVec2.y, 0);
			m_particles[i].m_lifetime = Random::Range1f(m_minLife, m_maxLife);

		}

	}
}

void ParticleEmitter::update(const float dt)
{
	for (int i = 0; i < m_particles.size(); ++i)
	{
		//Subtract from the particles lifetime
		m_particles[i].m_lifetime -= dt;

		if (m_emitterType == 0) //QUAD EMITTER
		{
			if (m_particles[i].m_lifetime <= 0.0f)
			{
				m_particles[i].m_position = Random::Range3f(-10, 10);
				m_particles[i].m_lifetime = Random::Range1f(m_minLife, m_maxLife);
			}

			//Position update
			m_particles[i].m_position -= glm::vec3(0.0f, 0.0f, -dt * (Random::Range1f(1, 2)));
		}
		else if (m_emitterType == 1) //CONE EMITTER
		{

			float tanTheta = glm::tan(glm::radians(m_degrees));

			if (m_particles[i].m_lifetime <= 0.0f)
			{
				glm::vec2 tempVec2 = Random::Circle2f(m_radius);

				m_particles[i].m_velocity = m_speed * glm::normalize(glm::vec3(tanTheta * tempVec2.x, tanTheta * tempVec2.y, 1.0f));
				m_particles[i].m_position = glm::vec3(tempVec2.x, tempVec2.y, 0);
				m_particles[i].m_lifetime = Random::Range1f(m_minLife, m_maxLife);
			}

			m_particles[i].m_position += dt * m_particles[i].m_velocity;
		}
		else if (m_emitterType == 2) //Explosion EMITTER
		{

			if (m_particles[i].m_lifetime <= 0.0f)
			{
				m_particles.clear();

				if(i == m_particles.size() - 1)
					m_isDone = true;
				
				/*glm::vec3 pos = Random::Range3f(-0.1, 0.1);
			

				m_particles[i].m_velocity = m_speed * glm::normalize(pos - m_controllerPos);

				if (glm::sqrt((pos.x * pos.x) + (pos.y * pos.y) + (pos.z * pos.z)) < 1)
					m_particles[i].m_position = pos;

				m_particles[i].m_lifetime = Random::Range1f(m_minLife, m_maxLife);*/
			}

			if(!m_particles.empty())
				m_particles[i].m_position += dt * m_particles[i].m_velocity;
		}
		else if (m_emitterType == 3) //Portal EMITTER
		{

			float tanTheta = glm::tan(glm::radians(m_degrees));

			if (m_particles[i].m_lifetime <= 0.0f)
			{
				glm::vec2 tempVec2 = Random::Circle2f(m_radius);

				m_particles[i].m_velocity = m_speed * glm::normalize(glm::vec3(1.0f, tanTheta * tempVec2.y, 1.0f));
				m_particles[i].m_position = glm::vec3(tempVec2.x, tempVec2.y, 0);
				m_particles[i].m_lifetime = Random::Range1f(m_minLife, m_maxLife);
			}

			m_particles[i].m_position += dt * m_particles[i].m_velocity;

		}

		if (!m_particles.empty())
		{
			//Update the position buffer
			m_positions[i * 4 + 0] = m_particles[i].m_position[0];
			m_positions[i * 4 + 1] = m_particles[i].m_position[1];
			m_positions[i * 4 + 2] = m_particles[i].m_position[2];

			m_positions[i * 4 + 3] = m_particles[i].m_lifetime;
		}

	}
}




void ParticleEmitter::setRadius(float radius)
{
	m_radius = radius;
}

void ParticleEmitter::setAngleDeg(float degrees)
{
	m_degrees = degrees;
}

void ParticleEmitter::setSpeed(float speed)
{
	m_speed = speed;
}

void ParticleEmitter::setLifetime(float min, float max)
{
	m_minLife = min;
	m_maxLife = max;
}

void ParticleEmitter::setControllerPos(glm::vec3 pos)
{
	m_controllerPos = pos;
}

float ParticleEmitter::getEmitterType()
{
	return m_emitterType;
}

bool ParticleEmitter::getDone()
{
	return m_isDone;
}

void ParticleEmitter::draw()
{
	//Bind and update data
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), m_vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_particles.size() * 4 * sizeof(float), m_positions, GL_DYNAMIC_DRAW);

	//Draw
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, m_particles.size());

	glBindVertexArray(0);
	glUseProgram(0);
}


ParticleController::ParticleController(int emitterType, glm::vec3 position, Texture* texture, entt::entity parent)
{
	m_shader = new Shader("Resource Files/Shaders/particles_vertex_shader.glsl", "Resource Files/Shaders/particles_fragment_shader.glsl");
	m_emitter = new ParticleEmitter(emitterType);
	m_emitter->setControllerPos(position);
	m_position = position;
	m_rotation = glm::vec3(0.0);
	m_scale = glm::vec3(1.0);
	m_size = 0.1;
	m_tex = texture;
	m_parentEntity = parent;
	m_modelMatrix = GameObject::GetComponent<Transform>(parent).UpdateGlobal();
}

ParticleController::~ParticleController()
{
	delete m_shader;
	delete m_emitter;
}

void ParticleController::setPosition(glm::vec3 position)
{
	m_position = position;
}

void ParticleController::setRotation(glm::vec3 rotation)
{
	m_rotation = rotation;
}

void ParticleController::setSize(float size)
{
	m_size = size;
}

void ParticleController::setColor(glm::vec4 startColor, glm::vec4 endColor)
{
	m_startColor = startColor;
	m_endColor = endColor;
}

ParticleEmitter* ParticleController::getEmitter()
{
	return m_emitter;
}


void ParticleController::update(const float dt, const glm::mat4 ProjectionMatrix, const glm::mat4& ViewMatrix)
{
	m_shader->use();
	
	//glUniform1i(10, 0);
	//m_shader->set1i(0, "myTexture");
	

	m_emitter->update(dt);

	

	if (m_emitter->getEmitterType() != 2)
	{
		m_modelMatrix = GameObject::GetComponent<Transform>(m_parentEntity).UpdateGlobal() * glm::mat4(glm::translate(glm::mat4(1.0f), m_position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::scale(glm::mat4(1.0f), m_scale));
	}
	m_shader->setMat4fv(ProjectionMatrix, "ProjectionMatrix");
	m_shader->setMat4fv(ViewMatrix, "ViewMatrix");
	m_shader->setMat4fv(m_modelMatrix, "ModelMatrix");
	m_shader->setVec4f(m_startColor, "sParticleColor");
	m_shader->setVec4f(m_endColor, "eParticleColor");
	m_shader->set1f(m_size, "particleSize");
	
}

void ParticleController::draw()
{
	glDepthMask(GL_FALSE);
	m_shader->use();
	m_tex->bind(10);
	m_emitter->draw();
	m_tex->unbind();
	glDepthMask(GL_TRUE);
}