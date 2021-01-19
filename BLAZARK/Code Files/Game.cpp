#include "Game.h"
#include "gtx/matrix_decompose.hpp"

//Constructors/destructors
Game::Game(GLFWwindow* window)
{
	m_window = window;

	InitGame();
	initUniforms();
}


Game::~Game()
{

	/*for (size_t i = 0; i < this->m_scenes.size(); i++)
		delete this->m_scenes[i];

	for (auto*& i : this->m_curScene->GetEntityRegistry())
		delete i;*/
}

void Game::InitGame()
{
	////pushing back all the scenes 
	//m_scenes.push_back(new Menu("Menu", &SceneNo, &switchIt));
	//m_scenes.push_back(new Universe("Universe_19", &SceneNo, &switchIt));
	//m_scenes.push_back(new Universe("Universe_27", &SceneNo, &switchIt));
	//m_scenes.push_back(new Universe("Universe_5", &SceneNo, &switchIt));
	//m_scenes.push_back(new Menu("Pause", &SceneNo, &switchIt));

	////giving all the scenes the projection matrix
	//m_scenes[scenes::UNIVERSE_19]->SetProjectionMatrix(&ProjectionMatrix);
	//m_scenes[scenes::UNIVERSE_27]->SetProjectionMatrix(&ProjectionMatrix);
	//m_scenes[scenes::UNIVERSE_5]->SetProjectionMatrix(&ProjectionMatrix);

	////setting the first scene
	//m_curScene = m_scenes[scenes::UNIVERSE_19];
	//m_curScene->InitScene();
	//m_curScene->SetWindow(m_window);// giving the current scene the window
}

void Game::SwitchScene(unsigned int sceneNo)
{
	/*m_curScene = m_scenes[sceneNo];
	m_curScene->InitScene();
	initUniforms();
	m_curScene->SetWindow(Application::GetWindow());
	m_isSceneSwitch = false;*/
}

void Game::update(float deltaTime)
{
	m_dt = deltaTime;

	UpdateScene(deltaTime);

	render();
}

void Game::render()
{
	//Update the uniforms
	updateUniforms();

	//m_curScene->Render(Application::GetDeltaTime());

	glBindVertexArray(0);
	glUseProgram(0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Game::initUniforms()
{
	/*this->m_curScene->GetShader()[SHADER_CORE_PROGRAM]->setMat4fv(m_curScene->GetCam()->getViewMatrix(), "ViewMatrix");
	this->m_curScene->GetShader()[SHADER_CORE_PROGRAM]->setMat4fv(ProjectionMatrix, "ProjectionMatrix");*/

}

void Game::updateUniforms()
{


	//this->m_curScene->GetShader()[SHADER_CORE_PROGRAM]->setMat4fv(this->m_curScene->GetCam()->getViewMatrix(), "ViewMatrix");
	//this->m_curScene->GetShader()[SHADER_CORE_PROGRAM]->setVec3f(this->m_curScene->GetCam()->getPosition(), "cameraPos");


	////Update frame buffer size and projection matrix
	//glfwGetFramebufferSize(this->window, &this->framebufferWidth, &this->framebufferHeight);

	//if (this->m_curScene->GetName() == "Menu" || this->m_curScene->GetName() == "Pause")
	//	this->ProjectionMatrix = glm::ortho(-4.0f, 4.0f, -4.0f, 4.0f, this->nearPlane, this->farPlane);

	//else
	//	this->ProjectionMatrix = glm::perspective(glm::radians(this->fov), static_cast<float>(this->framebufferWidth) / this->framebufferHeight, this->nearPlane, this->farPlane);

	//this->m_curScene->GetShader()[SHADER_CORE_PROGRAM]->setMat4fv(this->ProjectionMatrix, "ProjectionMatrix");
}

void Game::UpdateScene(float deltaTime)
{
	/*if (m_isSceneSwitch)
		SwitchScene(SceneNo);

	m_curScene->Update(deltaTime);*/
}


