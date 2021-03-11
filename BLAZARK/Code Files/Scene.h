#pragma once

#include "StaticRenderer.h"
#include "btBulletDynamicsCommon.h"
#include "Skybox.h"
#include "Sprite2D.h"
#include "Random.h"
#include "GamepadInput.h"
#include "ColorCorrection.h"
#include "GreyscaleEffect.h"
#include "SepiaEffect.h"
#include "EnemyAI.h"
#include "Projectile.h"

enum class ScenesNum { START_SCREEN, MAIN_MENU, PAUSE_MENU, UNIVERSE_19, UNIVERSE_27, UNIVERSE_5 };

class Scene {

public:
	Scene(std::string name);

	Scene() { }
	std::string GetName();


	virtual void Render(float deltaTime) {	}

	virtual void Update(float deltaTime) {	}

	virtual void InitScene();

	void SetWindow(GLFWwindow* window);

	entt::registry* GetScene();

	virtual void KeyInput() { }

	virtual void GamepadInput() { }

protected:

	unsigned int* SceneNo = nullptr;
	bool* switchIt = nullptr;

	bool initialised = false;

	std::string m_name = "None";

	GLFWwindow* m_window = nullptr;

	//Entites
	entt::registry* m_sceneReg = nullptr;

	GamePad gamepad;

	Camera* camera;

	static std::vector<Texture*> m_textures;
	static std::vector<Mesh*> m_meshes;
};

class Menu : public Scene
{

public:

	Menu(std::string name, unsigned int* num = nullptr, bool* change = nullptr);

	void InitScene() override;

	void Update(float deltaTime) override;


	void KeyInput() override;

	void GamepadInput() override;

	void Render(float deltaTime) override;

};

class Universe : public Scene
{
public:

	Universe(std::string name, unsigned int* num = nullptr, bool* change = nullptr);


	void InitScene() override;

	void Update(float deltaTime) override;

	void Render(float deltaTime) override;

	void KeyInput() override;

	void GamepadInput() override;

	void SolarSystemUpdate();

	bool isCollide(Transform Obj1, Transform Obj2);

private:

	
	float m_fireRate = 0.1;
	bool m_resetTime = true;
	float m_startTime = 0.0;

	std::vector<entt::entity> m_solarSystem;
	std::vector<Projectile*> m_bullets;
	entt::entity MainPlayerID;
};