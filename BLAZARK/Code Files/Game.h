#pragma once

#include "Scene.h"

class Game
{
public:
	//Constructors/destructors
	Game(GLFWwindow* window);
	virtual ~Game() = default;

	void InitGame();

	void SwitchScene();

	//Functions	
	void update(float deltaTime);
	void Render();

private:

	std::vector<Scene*> m_scenes;
	Scene* m_curScene;

	GLFWwindow* m_window;
	float m_dt;

	//values for scene switching
	unsigned int m_SceneNo = 0;
	bool m_isSceneSwitch = false;

	//Private functions
	void UpdateScene();
};

