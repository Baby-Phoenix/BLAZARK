#pragma once


#include "btBulletDynamicsCommon.h"
#include "Skybox.h"
#include "Sprite2D.h"
#include "Random.h"
#include "GamepadInput.h"
#include "PixelationEffect.h"
#include "ColorCorrection.h"
#include "GreyscaleEffect.h"
#include "SepiaEffect.h"
#include "EnemyAI.h"
#include "ScoreHandler.h"

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

	virtual unsigned int GetSceneResumeNumber();
	virtual void SetSceneResumeNo(unsigned int sceneno);

protected:

	unsigned int* SceneNo = nullptr;
	bool* switchIt = nullptr;
	unsigned int m_SceneResumeNo;
	float m_deltaTime;
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

	 unsigned int GetSceneResumeNumber() override;
	 void SetSceneResumeNo(unsigned int sceneno) override;

	void KeyInput() override;

	void GamepadInput() override;

	void Render(float deltaTime) override;

private:
	std::unique_ptr<GameObject> m_StartOrResume[4];
	float m_scoreTime = 1.0f;


	unsigned int m_curButton = 0;
	bool m_switchButton = false;
	bool m_ControlsSelected = false;
	float m_delay = 0.0f;
};

class Universe : public Scene
{
public:

	Universe(std::string name, unsigned int* num = nullptr, bool* change = nullptr);

	 unsigned int GetSceneResumeNumber() override;
	 void SetSceneResumeNo(unsigned int sceneno) override;

	void InitScene() override;

	void Update(float deltaTime) override;

	void Render(float deltaTime) override;

	void KeyInput() override;

	void GamepadInput() override;

	void SolarSystemUpdate();

	bool isBoxCollide(Transform Obj1, Transform Obj2);
	bool isBoxCircleCollide(Transform objBox, Transform objCircle);

private:

	float m_fireRate = 0.3;
	bool m_resetTime = true;
	float m_startTime = 0.0;
	
	//Particles
	ParticleController* particleTemp;
	std::vector<ParticleController*> particles;

	std::vector<entt::entity> m_solarSystem;
	std::unique_ptr<GameObject> m_score;
	std::vector<Projectile*> m_bullets;

	glm::mat4 shadowProjection = glm::mat4(0);
	std::vector<glm::mat4> shadowTransformations;

	entt::entity MainPlayerID;
	entt::entity BossID;
	entt::entity MoonID;
	entt::entity CamID;
	entt::entity CometID;
	entt::entity AsteroidBeltID;
	Shader* m_shader;
	bool m_isBossSpawn = false;
	static entt::entity health;
	static int m_PlayerHealth;
};