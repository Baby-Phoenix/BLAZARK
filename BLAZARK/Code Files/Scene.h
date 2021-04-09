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
#include "AudioEngine.h"

enum class ScenesNum { START_SCREEN, MAIN_MENU, PAUSE_MENU, GAME_OVER, WIN, UNIVERSE_19, UNIVERSE_27, UNIVERSE_5 };

class Scene {

public:
	Scene(std::string name);

	Scene() { }
	std::string GetName();

	virtual void Render(float deltaTime) {	}

	virtual void Update(float deltaTime) {	}

	virtual void InitScene(int Prescore);

	void SetWindow(GLFWwindow* window);

	virtual int GetSceneScore();

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

	void InitScene(int Prescore) override;

	void Update(float deltaTime) override;

	 unsigned int GetSceneResumeNumber() override;
	 void SetSceneResumeNo(unsigned int sceneno) override;

	 int GetSceneScore() override;

	void KeyInput() override;

	void GamepadInput() override;

	void Render(float deltaTime) override;

private:
	std::unique_ptr<GameObject> m_StartOrResume[4];
	float m_scoreTime = 1.0f;

	ScoreHandler* score = nullptr;

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

	void InitScene(int Prescore) override;

	void InitFmod();

	void Update(float deltaTime) override;

	void Render(float deltaTime) override;

	void KeyInput() override;

	void GamepadInput() override;

	void SolarSystemUpdate();

	int GetSceneScore() override;

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
	std::unique_ptr<GameObject> m_arrowTotheBoss;
	std::vector<Projectile*> m_bullets;

	glm::mat4 shadowProjection = glm::mat4(0);
	std::vector<glm::mat4> shadowTransformations;

	entt::entity MainPlayerID;
	entt::entity BossID;
	entt::entity MoonID;
	entt::entity CamID;
	entt::entity CometID;
	entt::entity AsteroidBeltID;
	entt::entity PortalID;

	Shader* m_shader;
	bool m_isBossSpawn = false;
	bool m_isBossDead = false;
	static entt::entity health;
	static int m_PlayerHealth;
};

class Tutorial : public Scene 
{
public:



private:


};