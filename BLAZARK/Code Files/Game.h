#pragma once

#include "Scene.h"
#include "Application.h"

class Game
{
public:
	//Constructors/destructors
	Game(GLFWwindow* window);
	virtual ~Game();

	void InitGame();

	void SwitchScene(unsigned int sceneNo);

	//Functions	
	void update(float deltaTime);
	void render();

private:
	//vector<Scene*> m_scenes;

	Scene* m_curScene;

	GLFWwindow* m_window;
	float m_dt;

	//values for scene switching
	//unsigned int SceneNo = 0;
	//bool m_isSceneSwitch = false;

	//Private functions
	void initUniforms();
	void updateUniforms();
	void UpdateScene(float deltaTime);
};

