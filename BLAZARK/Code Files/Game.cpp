#include "Game.h"

//Constructors/destructors
Game::Game(GLFWwindow* window)
{
	m_window = window;

	InitGame();
}


void Game::InitGame()
{
	//pushing back all the scenes
	m_scenes.push_back(new Menu("Start_Screen", &m_SceneNo, &m_isSceneSwitch));
	m_scenes.push_back(new Menu("Main_Menu", &m_SceneNo, &m_isSceneSwitch));
	m_scenes.push_back(new Menu("Pause_Menu", &m_SceneNo, &m_isSceneSwitch));
	m_scenes.push_back(new Menu("Game_Over_Lose", &m_SceneNo, &m_isSceneSwitch));
	m_scenes.push_back(new Universe("Universe_19", &m_SceneNo, &m_isSceneSwitch));
	m_scenes.push_back(new Universe("Universe_27", &m_SceneNo, &m_isSceneSwitch));
	m_scenes.push_back(new Universe("Universe_5", &m_SceneNo, &m_isSceneSwitch));

	//setting the first scene
	m_curScene = m_scenes[int(ScenesNum::UNIVERSE_19)];
	m_curScene->InitScene();
	m_curScene->SetWindow(m_window);// giving the current scene the window
}

void Game::SwitchScene()
{
	m_scenes[m_SceneNo]->SetSceneResumeNo(m_curScene->GetSceneResumeNumber());
	m_curScene = m_scenes[m_SceneNo];
	m_curScene->InitScene();
	m_curScene->SetWindow(Application::GetWindow());
	m_isSceneSwitch = false;
}

void Game::update(float deltaTime)
{
	m_dt = deltaTime;

	UpdateScene();

	Render();
}

void Game::Render()
{
	
	m_curScene->Render(m_dt);

	//glBindVertexArray(0);
	//glUseProgram(0);
	//glActiveTexture(0);
	//glBindTexture(GL_TEXTURE_2D, 0);
}

void Game::UpdateScene()
{
	if (m_isSceneSwitch)
		SwitchScene();

	m_curScene->Update(m_dt);
}


