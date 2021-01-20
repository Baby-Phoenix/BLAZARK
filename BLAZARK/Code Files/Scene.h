#pragma once

#include <string>
#include "Camera.h"
#include"StaticRenderer.h"
//#include "libs.h"
//#include "ParticleSystem.h"
//#include "Projectile.h"
#include "Random.h"
#include "GamepadInput.h"

using namespace std;


enum class ScenesNum { MENU = 0, UNIVERSE_19, UNIVERSE_27, UNIVERSE_5, PAUSE };

class Scene {

public:
	Scene(string name);

	Scene() { }
	string GetName();


	virtual void Render(float deltaTime);

	virtual void Update(float deltaTime);

	virtual void InitScene();

	void SetWindow(GLFWwindow* window);

	//vector <Shader*> GetShader();

	entt::registry* GetScene();

	virtual void KeyInput() { }

	virtual void MouseInput() { }

	virtual void GamepadInput() { }



protected:

	unsigned int* SceneNo = nullptr;
	bool* switchIt = nullptr;

	bool initialised = false;

	string m_name = "None";

	GLFWwindow* m_window = nullptr;

	//Entites
	entt::registry* m_sceneReg = nullptr;

	GamePad gamepad;

	//shaders
	//vector<Shader*> shaders;

};

class Menu : public Scene
{

public:

	Menu(string name, unsigned int* num = nullptr, bool* change = nullptr);

	void InitScene() override;

	void Update(float deltaTime) override;


	void KeyInput() override;

	void MouseInput() override;

	void GamepadInput() override;

	void Render(float deltaTime) override;

};

class Universe : public Scene
{
public:

	Universe(string name, unsigned int* num = nullptr, bool* change = nullptr);


	void InitScene() override;

	void InitSkyBox();

	void Update(float deltaTime) override;

	void Render(float deltaTime) override;

	void UpdateSkyBox();

	void KeyInput() override;

	void MouseInput() override;

	void GamepadInput() override;

private:

	float deltaTime;

	//Skybox
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
	GLuint skyboxVAO;
	GLuint cubemapTexture;

	//Particles
	//std::vector<ParticleController*> particles;

};