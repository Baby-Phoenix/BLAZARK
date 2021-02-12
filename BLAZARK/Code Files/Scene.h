#pragma once

#include "StaticRenderer.h"
#include "btBulletDynamicsCommon.h"
#include "Skybox.h"
#include "Sprite2D.h"
//#include "libs.h"
//#include "ParticleSystem.h"
//#include "Projectile.h"
#include "Random.h"
#include "GamepadInput.h"
#include "ColorCorrection.h"
#include "GreyscaleEffect.h"
#include "SepiaEffect.h"

using namespace std;


enum class ScenesNum { START_SCREEN, MAIN_MENU, PAUSE_MENU, UNIVERSE_19, UNIVERSE_27, UNIVERSE_5 };

class Scene {

public:
	Scene(string name);

	Scene() { }
	string GetName();


	virtual void Render(float deltaTime) {	}

	virtual void Update(float deltaTime) {	}

	virtual void InitScene();

	void SetWindow(GLFWwindow* window);

	//vector <Shader*> GetShader();

	entt::registry* GetScene();

	virtual void KeyInput() { }

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

	Camera* camera;

	static vector<Texture*> m_textures;
	static vector<Mesh*> m_meshes;
};

class Menu : public Scene
{

public:

	Menu(string name, unsigned int* num = nullptr, bool* change = nullptr);

	void InitScene() override;

	void Update(float deltaTime) override;


	void KeyInput() override;

	void GamepadInput() override;

	void Render(float deltaTime) override;

};

class Universe : public Scene
{
public:

	Universe(string name, unsigned int* num = nullptr, bool* change = nullptr);


	void InitScene() override;

	void Update(float deltaTime) override;

	void Render(float deltaTime) override;

	void KeyInput() override;

	void GamepadInput() override;

private:

	float deltaTime;

	entt::entity MainPlayerID;
};