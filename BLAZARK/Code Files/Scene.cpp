#include "Scene.h"


int Universe::m_PlayerHealth = 0;
entt::entity Universe::health = entt::entity(0);

enum class TextureType { START = 3 , RESUME, CONTROLS, EXIT, BACKGROUND, CONTROLSMENU, SCORE, SCORENUM, COMET, FIRE, BLINK, YELLOW, ORANGE};

enum class PlayerMesh { PLAYERSHIPXWINGS, PLAYERSHIPXWINGE = 2, PLAYERSHIPPENCIL, PLAYERSHIPBAT, PLAYERBULLET };

enum class EnemyMesh { SCAVENGERU1S = 6, SCAVENGERU1E = 8, NEROTISTU1, KAMAKAZI, BOMBARDIER, JELLYFISH/*JELLYFISHS, JELLYFISHE = 32*/ };

enum class PlanetMesh { SOLARI = int(EnemyMesh::JELLYFISH)+1, VERASTEN, YECHIN, KERANTIA, LUNARI, GUERISTIS, KEMINTH,
						LUTERO, DEDMOS, TITANIUS, KREILLO, PAXALLUS, DERANGI, RHETOID, MAGAANTU , COMET};

enum Universe19SS { SVC, SYC, SKRC, SGC, SKEC, SOLARI, VERASTEN, YECHIN, KERANTIA, GUERISTIS, KEMINTH, KEMINTHRING, HPC };
enum Universe27SS { LDMC = int(Universe19SS::HPC) + 1, LTC, LKC, LPC, LDRC, LRC, LMC, LUTERO, DEDMOS, TITANIUS, KREILLO, PAXALLUS, DERANGI, RHETOID, MEGAANTU};
enum Universe5SS {};

std::vector<Mesh*> Scene::m_meshes;
std::vector<Texture*> Scene::m_textures;

std::unique_ptr<GameObject> effect;

// Solar System Rotation
glm::vec3 sunRotation = glm::vec3(0, -0.025, 0);
glm::vec3 planetRotation = glm::vec3(0, 0.045583, 0);
glm::vec3 moonOrbit = glm::vec3(0, 0.01022, 0);
glm::vec3 verastenOrbit = glm::vec3(0, 0.004787, 0);
glm::vec3 yechinOrbit = glm::vec3(0, 0.003502, 0);
glm::vec3 kerantiaOrbit = glm::vec3(0, 0.002978, 0);
glm::vec3 gueristisOrbit = glm::vec3(0, 0.0024077, 0);
glm::vec3 keminthOrbit = glm::vec3(0, 0.000543, 0);
glm::vec3 asteroidOrbit = glm::vec3(0, 0.00543, 0);
glm::vec3 cometVelocity = glm::vec3(1, 0, 0);

// Key Toggles
bool texTglPressed = false;
bool isPlayerAnim = false;
bool isWingOpen = false;
bool isexplode = false;
bool isDodge = false;
float dodgeCount = 0.5;
float tempDir = 1;

Scene::Scene(std::string name)
	:m_name(name)
{
	if (m_textures.size() < 1) {
		// HUD //
		m_textures.push_back(new Texture("Resource Files/Textures/HUD/health_spritesheet.png"));
		m_textures.push_back(new Texture("Resource Files/Textures/HUD/Abilities_Unavailable_Temp.png"));
		m_textures.push_back(new Texture("Resource Files/Textures/HUD/PowerUP_Unavailable_Temp.png"));
		
		/////////
		m_textures.push_back(new Texture("Resource Files/Textures/Menu/TitleScreen.png"));
		m_textures.push_back(new Texture("Resource Files/Textures/Menu/Resume.png"));
		m_textures.push_back(new Texture("Resource Files/Textures/Menu/Control.png"));
		m_textures.push_back(new Texture("Resource Files/Textures/Menu/Exit.png"));
		m_textures.push_back(new Texture("Resource Files/Textures/Menu/Pause_Menu_Background.png"));
		m_textures.push_back(new Texture("Resource Files/Textures/Menu/Controls.png"));

		m_textures.push_back(new Texture("Resource Files/Textures/HUD/SCORE_LABEL.png"));
		m_textures.push_back(new Texture("Resource Files/Textures/HUD/ScoreAnim.png"));

		m_textures.push_back(new Texture("Resource Files/Textures/CometTexture2.png"));
		m_textures.push_back(new Texture("Resource Files/Textures/red.png"));
		m_textures.push_back(new Texture("Resource Files/Textures/blink.png"));
		m_textures.push_back(new Texture("Resource Files/Textures/yellow.png"));
		m_textures.push_back(new Texture("Resource Files/Textures/orange.png")); 
	}

	if (m_meshes.size() < 1) {
		// Player Ships //
		for (int i = 0; i <= 2; i++) {
			m_meshes.push_back(new Mesh());
			loadOBJ(("Resource Files/OBJFiles/PlayerShips/Morph/PlayerShipX-WingFrame" + std::to_string(i) + ".obj").c_str(), *m_meshes[i]);
		}
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/PlayerShips/PlayerShipPencil.obj", *m_meshes[int(PlayerMesh::PLAYERSHIPPENCIL)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/PlayerShips/PlayerShipBat.obj", *m_meshes[int(PlayerMesh::PLAYERSHIPBAT)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Misc/laserBullet.obj", *m_meshes[int(PlayerMesh::PLAYERBULLET)]);
		// Universe-19 //
		for (int i = 1; i <= 3; i++) {
			m_meshes.push_back(new Mesh());
			loadOBJ(("Resource Files/OBJFiles/Universe-19/EnemyShips/Morph/Scavenger_Idle_" + std::to_string(i) + ".obj").c_str(), *m_meshes[i + int(PlayerMesh::PLAYERBULLET)]);
		}
		
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Universe-19/EnemyShips/Nerotist.obj", *m_meshes[int(EnemyMesh::NEROTISTU1)]);

		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Universe-19/EnemyShips/Kamikaze.obj", *m_meshes[int(EnemyMesh::KAMAKAZI)]);

		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Universe-19/EnemyShips/Bombardier.obj", *m_meshes[int(EnemyMesh::BOMBARDIER)]);

		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Universe-19/EnemyShips/jellyfishBoss.obj", *m_meshes[int(EnemyMesh::JELLYFISH)]);

		/*for (int i = 1; i <= 26; i++) {
			m_meshes.push_back(new Mesh());
			loadOBJ(("Resource Files/OBJFiles/Universe-19/EnemyShips/Morph/Boss/jellyfishBoss_" + std::to_string(i) + ".obj").c_str(), *m_meshes[i + int(EnemyMesh::BOMBARDIER)]);
		}*/

		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Universe-19/Planets/Solari.obj", *m_meshes[int(PlanetMesh::SOLARI)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Universe-19/Planets/Verasten.obj", *m_meshes[int(PlanetMesh::VERASTEN)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Universe-19/Planets/Yechin.obj", *m_meshes[int(PlanetMesh::YECHIN)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Universe-19/Planets/Kerantia.obj", *m_meshes[int(PlanetMesh::KERANTIA)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Universe-19/Planets/Lunari.obj", *m_meshes[int(PlanetMesh::LUNARI)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Universe-19/Planets/Gueristis.obj", *m_meshes[int(PlanetMesh::GUERISTIS)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Universe-19/Planets/Keminth.obj", *m_meshes[int(PlanetMesh::KEMINTH)]);
		// Universe-27 //
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Universe-27/Planets/Lutero.obj", *m_meshes[int(PlanetMesh::LUTERO)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Universe-27/Planets/Dedmos.obj", *m_meshes[int(PlanetMesh::DEDMOS)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Universe-27/Planets/Titanius.obj", *m_meshes[int(PlanetMesh::TITANIUS)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Universe-27/Planets/Kreillo.obj", *m_meshes[int(PlanetMesh::KREILLO)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Universe-27/Planets/Paxallus.obj", *m_meshes[int(PlanetMesh::PAXALLUS)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Universe-27/Planets/Derangi.obj", *m_meshes[int(PlanetMesh::DERANGI)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Universe-27/Planets/Rhetoid.obj", *m_meshes[int(PlanetMesh::RHETOID)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Universe-27/Planets/Magaantu.obj", *m_meshes[int(PlanetMesh::MAGAANTU)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Misc/Comet2.obj", *m_meshes[int(PlanetMesh::COMET)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Misc/Debris/Barrel.obj", *m_meshes[int(PlanetMesh::COMET) + 1]);
		m_meshes.push_back(new Mesh()); 
		loadOBJ("Resource Files/OBJFiles/Misc/Debris/Bottle.obj", *m_meshes[int(PlanetMesh::COMET) + 2]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Misc/Debris/BoxContainer.obj", *m_meshes[int(PlanetMesh::COMET) + 3]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Misc/Asteroids/Asteroids_RING_2.obj", *m_meshes[int(PlanetMesh::COMET) + 4]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Misc/Asteroids/Asteroids_Cluster_1.obj", *m_meshes[int(PlanetMesh::COMET) + 5]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Misc/Asteroids/Asteroids_Cluster_2.obj", *m_meshes[int(PlanetMesh::COMET) + 6]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Misc/Asteroids/Asteroids_Cluster_3.obj", *m_meshes[int(PlanetMesh::COMET) + 7]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Misc/Asteroids/Asteroids_Cluster_4.obj", *m_meshes[int(PlanetMesh::COMET) + 8]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Misc/Asteroids/Asteroids_Cluster_5.obj", *m_meshes[int(PlanetMesh::COMET) + 9]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Misc/Asteroids/Asteroids_Cluster_6.obj", *m_meshes[int(PlanetMesh::COMET) + 10]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Misc/Asteroids/Asteroids_Cluster_7.obj", *m_meshes[int(PlanetMesh::COMET) + 11]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Misc/Asteroids/Asteroids_Cluster_8.obj", *m_meshes[int(PlanetMesh::COMET) + 12]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Universe-19/Planets/KeminthRing.obj", *m_meshes[int(PlanetMesh::COMET) + 13]);
	}
}

std::string Scene::GetName()
{
	return m_name;
}

void Scene::InitScene()
{
	
	//creating a new registry for the scene when initialised
	if (m_sceneReg == nullptr)
	m_sceneReg = new entt::registry();

	//Giving the ECS the same registry as the current scene
	GameObject::SetRegistry(m_sceneReg);

	if (GameObject::IsEmpty()) {
		//add entites in here if not initialised
	}
}

void Scene::SetWindow(GLFWwindow* window)
{
	m_window = window;
}

entt::registry* Scene::GetScene()
{
	return m_sceneReg;
}

unsigned int Scene::GetSceneResumeNumber()
{
	return m_SceneResumeNo;
}

void Scene::SetSceneResumeNo(unsigned int sceneno)
{
	m_SceneResumeNo = sceneno;
}

Menu::Menu(std::string name, unsigned int* num, bool* change)
	:Scene(name)
{
	SceneNo = num;
	switchIt = change;
}

void Menu::InitScene()
{
	//creating a new registry for the scene when initialised
	if (m_sceneReg == nullptr) 
		m_sceneReg = new entt::registry();
	
	//Giving the ECS the same registry as the current scene
	GameObject::SetRegistry(m_sceneReg);

	if (GameObject::IsEmpty()) {

		auto cameraEntity = GameObject::Allocate();
		cameraEntity->AttachComponent<Transform>();
		camera = &cameraEntity->AttachComponent<Camera>(int(cameraEntity->GetID()));
		cameraEntity->GetComponent<Transform>().SetLocalPos(glm::vec3(0.0f, 0.0f, 100.0f));

		if (m_name == "Start_Screen") {
			auto titleScreen = GameObject::Allocate();
			titleScreen->AttachComponent<Sprite2D>(m_textures[int(TextureType::START)], titleScreen->GetID(), 100, 100);
			titleScreen->AttachComponent<Transform>();
		}
		else if (m_name == "Main_Menu") {

		}
		else if (m_name == "Pause_Menu") {
			

			//Resume
			{
				m_StartOrResume[0] = GameObject::Allocate();
				auto* tempAnim = &m_StartOrResume[0]->AttachComponent<AnimationHandler>();
				auto& anim = m_StartOrResume[0]->GetComponent<AnimationHandler>();
				anim.InitUVS(m_textures[int(TextureType::RESUME)]);
				Animation2D Oneclip;
				Oneclip.AddFrame(UVS(glm::vec2(1, 50), glm::vec2(258, 1)));
				Oneclip.SetIsRepeating(true);
				Oneclip.SetSecPerFrame(1.0);
				anim.AddAnimation(Oneclip);

				Animation2D Twoclip;
				Twoclip.AddFrame(UVS(glm::vec2(260, 50), glm::vec2(517, 1)));
				Twoclip.SetIsRepeating(false);
				Twoclip.SetSecPerFrame(1.0);
				anim.AddAnimation(Twoclip);
				anim.SetActiveAnim(1);

				m_StartOrResume[0]->AttachComponent<Sprite2D>(m_textures[int(TextureType::RESUME)], m_StartOrResume[0]->GetID(), 17, 6, false, tempAnim);
				m_StartOrResume[0]->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 10, 10));
			}

			//Instructions
			{
				m_StartOrResume[1] = GameObject::Allocate();
				auto* tempAnim = &m_StartOrResume[1]->AttachComponent<AnimationHandler>();
				auto& anim = m_StartOrResume[1]->GetComponent<AnimationHandler>();
				anim.InitUVS(m_textures[int(TextureType::CONTROLS)]);
				Animation2D Oneclip;
				Oneclip.AddFrame(UVS(glm::vec2(1, 50), glm::vec2(346, 1)));

				Oneclip.SetIsRepeating(false);
				Oneclip.SetSecPerFrame(1.0);
				anim.AddAnimation(Oneclip);
				anim.SetActiveAnim(0);
				 
				Animation2D Twoclip;
				Twoclip.AddFrame(UVS(glm::vec2(348, 50), glm::vec2(693, 1)));

				Twoclip.SetIsRepeating(false);
				Twoclip.SetSecPerFrame(1.0);
				anim.AddAnimation(Twoclip);
				anim.SetActiveAnim(0);


				m_StartOrResume[1]->AttachComponent<Sprite2D>(m_textures[int(TextureType::CONTROLS)], m_StartOrResume[1]->GetID(), 25, 6, true, tempAnim);
				m_StartOrResume[1]->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, -30, 10));
			}

			//Exit
			{
				m_StartOrResume[2] = GameObject::Allocate();
				auto* tempAnim = &m_StartOrResume[2]->AttachComponent<AnimationHandler>();
				auto& anim = m_StartOrResume[2]->GetComponent<AnimationHandler>();
				anim.InitUVS(m_textures[int(TextureType::EXIT)]);
				Animation2D Oneclip;
				Oneclip.AddFrame(UVS(glm::vec2(1, 50), glm::vec2(136, 1)));

				Oneclip.SetIsRepeating(false);
				Oneclip.SetSecPerFrame(1.0);
				anim.AddAnimation(Oneclip);
				anim.SetActiveAnim(0);

				Animation2D Twoclip;
				Twoclip.AddFrame(UVS(glm::vec2(138, 50), glm::vec2(273, 1)));

				Twoclip.SetIsRepeating(false);
				Twoclip.SetSecPerFrame(1.0);
				anim.AddAnimation(Twoclip);
				anim.SetActiveAnim(0);


				m_StartOrResume[2]->AttachComponent<Sprite2D>(m_textures[int(TextureType::EXIT)], m_StartOrResume[2]->GetID(), 10, 6, true, tempAnim);
				m_StartOrResume[2]->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, -65, 10));
			}

			//instruction background
			{
				m_StartOrResume[3] = GameObject::Allocate();

				m_StartOrResume[3]->AttachComponent<Sprite2D>(m_textures[int(TextureType::CONTROLSMENU)], m_StartOrResume[3]->GetID(), 100, 100);
				m_StartOrResume[3]->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, -10));
			}
		
			auto background = GameObject::Allocate();
			background->AttachComponent<Sprite2D>(m_textures[int(TextureType::BACKGROUND)], background->GetID(), 100, 100);
			background->AttachComponent<Transform>().SetLocalPos(0, 0, -5);

			}
	}
}

void Menu::Update(float deltaTime)
{
	// Transform Update
	m_sceneReg->view<Transform>().each([=](Transform& transform) {	transform.UpdateGlobal(); });
	m_sceneReg->view<AnimationHandler>().each([=](AnimationHandler& anim) {	anim.Update(deltaTime); });
	m_deltaTime = deltaTime;

	// Key Input
	KeyInput();
	GamepadInput();

	// Camera Update
	camera->Update();
}

unsigned int Menu::GetSceneResumeNumber()
{
	return m_SceneResumeNo;
}

void Menu::SetSceneResumeNo(unsigned int sceneno)
{
	m_SceneResumeNo = sceneno;
}


void Menu::KeyInput()
{
	// Scene Switching //
	if (glfwGetKey(m_window, GLFW_KEY_0) == GLFW_PRESS) {
		*switchIt = true;
		*SceneNo = int(ScenesNum::START_SCREEN);
	}
	if (glfwGetKey(m_window, GLFW_KEY_1) == GLFW_PRESS) {
		*switchIt = true;
		*SceneNo = int(ScenesNum::UNIVERSE_19);
	}
	if (glfwGetKey(m_window, GLFW_KEY_2) == GLFW_PRESS) {
		*switchIt = true;
		*SceneNo = int(ScenesNum::UNIVERSE_27);
	}


	if (m_name == "Pause_Menu") {
		if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			if (m_ControlsSelected) {
				m_StartOrResume[3]->GetComponent<Transform>().SetLocalPos(glm::vec3(0, 0, -10));
				m_ControlsSelected = false;
			}
		}

		if (m_switchButton)
			m_delay -= m_deltaTime;

		if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS) {

			if (!m_switchButton)
				m_switchButton = true;

			if (m_delay <= 0.0f) {
				m_StartOrResume[m_curButton]->GetComponent<AnimationHandler>().SetActiveAnim(0);
				m_curButton = m_curButton <= 0 ? 2 : m_curButton - 1;
				m_StartOrResume[m_curButton]->GetComponent<AnimationHandler>().SetActiveAnim(1);
				m_switchButton = false;
				m_delay = 0.2f;
			}
		}
		if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS) {

			if (!m_switchButton)
				m_switchButton = true;

			if (m_delay <= 0.0f) {
				m_StartOrResume[m_curButton]->GetComponent<AnimationHandler>().SetActiveAnim(0);
				m_curButton = m_curButton >= 2 ? 0 : m_curButton + 1;
				m_StartOrResume[m_curButton]->GetComponent<AnimationHandler>().SetActiveAnim(1);
				m_switchButton = false;
				m_delay = 0.2f;
			}
		}

		if (glfwGetKey(m_window, GLFW_KEY_ENTER) == GLFW_PRESS) {
			if (m_curButton == 0) {
				*switchIt = true;
				*SceneNo = m_SceneResumeNo;
			}
			else if (m_curButton == 1) {
				m_StartOrResume[3]->GetComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 20));
				m_ControlsSelected = true;
			}
			else if (m_curButton == 2) {
				glfwSetWindowShouldClose(m_window, GLFW_TRUE);
			}
		}
	}
}

void Menu::GamepadInput()
{
	
	if (gamepad.getGamepadInput()) {
		//button input
		if (gamepad.buttons.A) {

		}
		else if (gamepad.buttons.B) {

		}
		else if (gamepad.buttons.X) {

		}
		else if (gamepad.buttons.Y) {

		}

		//trigger input
		//Left Trigger move forward
		if (gamepad.trigger.LT > 0 && gamepad.trigger.LT < 1) {
			/*temp = glm::vec3(0, 0, -2);
			playerEnt.MoveLocalPos(temp);*/
		}

		else if (gamepad.trigger.LT > -1 && gamepad.trigger.LT < 0) {

		}

		if (gamepad.trigger.RT > 0 && gamepad.trigger.RT < 1) {

		}

		else if (gamepad.trigger.RT > -1 && gamepad.trigger.RT < 0) {

		}


		//Left Joystick rotate left
		if (gamepad.axes[Joystick::LEFT].X > -0.1f && gamepad.axes[Joystick::LEFT].X > -0.33) {
		}
		else if (gamepad.axes[Joystick::LEFT].X < -0.3 && gamepad.axes[Joystick::LEFT].X > -0.66f) {

		}
		else if (gamepad.axes[Joystick::LEFT].X < -0.66) {

		}


		if (gamepad.axes[Joystick::LEFT].Y > 0 && gamepad.axes[Joystick::LEFT].Y < 1) {

		}
		else if (gamepad.axes[Joystick::LEFT].Y > -1 && gamepad.axes[Joystick::LEFT].Y < 0) {

		}

		if (gamepad.axes[Joystick::RIGHT].X > 0 && gamepad.axes[Joystick::RIGHT].X < 1) {

		}
		else if (gamepad.axes[Joystick::RIGHT].X > -1 && gamepad.axes[Joystick::RIGHT].X < 0) {

		}

		if (gamepad.axes[Joystick::RIGHT].Y > 0 && gamepad.axes[Joystick::RIGHT].Y < 1) {

		}
		else if (gamepad.axes[Joystick::RIGHT].Y > -1 && gamepad.axes[Joystick::RIGHT].Y < 0) {

		}
	}
}

void Menu::Render(float deltaTime)
{
	m_sceneReg->view<Sprite2D>().each([=](Sprite2D& render) { render.Draw(camera); });
}

Universe::Universe(std::string name, unsigned int* num, bool* change)
	:Scene(name)
{
	SceneNo = num;
	switchIt = change;
}

unsigned int Universe::GetSceneResumeNumber()
{
	return m_SceneResumeNo;
}

void Universe::SetSceneResumeNo(unsigned int sceneno)
{
}

void Universe::InitScene()
{
	//creating a new registry for the scene when initialised
	if (m_sceneReg == nullptr)
		m_sceneReg = new entt::registry();

	//Giving the ECS the same registry as the current scene
	GameObject::SetRegistry(m_sceneReg);

	if (GameObject::IsEmpty()) {
		// Camera
		auto cameraEntity = GameObject::Allocate();
		entt::entity CameraID;
		entt::entity* camentity = new entt::entity(cameraEntity->GetID());
		CameraID = cameraEntity->GetID();
		CamID = cameraEntity->GetID();
		cameraEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 13, 25));
		camera = &cameraEntity->AttachComponent<Camera>(int(CameraID));
		camera->PerspectiveProj(0.1f, 1000.0f, Application::GetWindowWidth() / Application::GetWindowHeight(), 1.0f);

		// Player
		auto playerEntity = GameObject::Allocate();
		MainPlayerID = playerEntity->GetID();
		playerEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 0));
		playerEntity->AttachComponent<DynamicRenderer>(cameraEntity->GetID(), MainPlayerID, *m_meshes[int(PlayerMesh::PLAYERSHIPXWINGS/*EnemyMesh::SCAVENGERU1S*/)], nullptr);
		playerEntity->AttachComponent<EntityType>() = EntityType::PLAYER;
		playerEntity->GetComponent<Transform>().SetLocalRot(0, 180, 0);
		playerEntity->GetComponent<Transform>().SetWHD(glm::vec3(m_meshes[int(PlayerMesh::PLAYERSHIPXWINGS/*EnemyMesh::SCAVENGERU1S*/)]->GetWidth(), m_meshes[int(PlayerMesh::PLAYERSHIPXWINGS/*EnemyMesh::SCAVENGERU1S*/)]->GetHeight(), m_meshes[int(PlayerMesh::PLAYERSHIPXWINGS/*EnemyMesh::SCAVENGERU1S*/)]->GetDepth()));

		playerController = &playerEntity->AttachComponent<MorphAnimController>(int(MainPlayerID));
		playerController->SetFrames(m_meshes, int(PlayerMesh::PLAYERSHIPXWINGS/*EnemyMesh::SCAVENGERU1S*/), int(PlayerMesh::PLAYERSHIPXWINGE/*EnemyMesh::SCAVENGERU1E*/), false);

		//Player Thrusters
		//Left - 0
		glm::vec3 playerPos = GameObject::GetComponent<Transform>(MainPlayerID).GetLocalPos();
		Texture* sky2Tex = new Texture("Resource Files/Textures/sky3.png");
		particleTemp = new ParticleController(1, glm::vec3(playerPos.x - 0.6, playerPos.y - 0.0, playerPos.z + 2.2f), m_textures[int(TextureType::FIRE)], MainPlayerID);
		particleTemp->getEmitter()->setRadius(0.3);
		particleTemp->getEmitter()->setLifetime(0.1f, 1.5f);
		particleTemp->getEmitter()->setSpeed(2);	
		particleTemp->getEmitter()->init();
		particles.push_back(particleTemp);
		//Center Right - 1
		particleTemp = new ParticleController(1, glm::vec3(playerPos.x + 0.6, playerPos.y - 0.0, playerPos.z + 2.2f), m_textures[int(TextureType::FIRE)], MainPlayerID);
		particleTemp->getEmitter()->setRadius(0.3);
		particleTemp->getEmitter()->setLifetime(0.1f, 1.5f);
		particleTemp->getEmitter()->setSpeed(2);
		particleTemp->getEmitter()->init();
		particles.push_back(particleTemp);
		glm::vec3 tempv3 = glm::vec3(120, 0, 2400);
		GameObject::GetComponent<Transform>(MainPlayerID).MoveLocalPos(tempv3);
		

		//auto tempEnt = GameObject::Allocate();
		//tempEnt->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 0));
		////TESTING PORTAL
		//particleTemp = new ParticleController(3, playerEntity->GetComponent<Transform>().GetLocalPos(), sky2Tex, tempEnt->GetID());
		////particleTemp->setRotation(glm::vec3(0, 180, 0));
		//particleTemp->getEmitter()->setRadius(30);
		//particleTemp->setSize(5);
		//particleTemp->getEmitter()->setLifetime(0.1f, 1.5f);
		//particleTemp->getEmitter()->setSpeed(0.5);
		//particleTemp->getEmitter()->init();
		//particles.push_back(particleTemp);

		//HUD
		auto healthent = GameObject::Allocate();
		health = healthent->GetID();
		auto* tempAnim = &healthent->AttachComponent<AnimationHandler>();
		auto* anim = &healthent->GetComponent<AnimationHandler>();
		anim->InitUVS(m_textures[0]);
		
		Animation2D Oneclip;
		Oneclip.AddFrame(UVS(glm::vec2(1, 235), glm::vec2(235, 1)));
		Oneclip.SetIsRepeating(false);
		Oneclip.SetSecPerFrame(1.0);
		anim->AddAnimation(Oneclip);
		Oneclip.Clear();

		Oneclip.AddFrame(UVS(glm::vec2(236, 235), glm::vec2(470, 1)));
		Oneclip.SetIsRepeating(false);
		Oneclip.SetSecPerFrame(1.0);
		anim->AddAnimation(Oneclip);
		Oneclip.Clear();

		Oneclip.AddFrame(UVS(glm::vec2(471, 235), glm::vec2(705, 1)));
		Oneclip.SetIsRepeating(false);
		Oneclip.SetSecPerFrame(1.0);
		anim->AddAnimation(Oneclip);
		Oneclip.Clear();

		Oneclip.AddFrame(UVS(glm::vec2(706, 235), glm::vec2(939, 1)));
		/*Oneclip.AddFrame(UVS(glm::vec2(1, 235), glm::vec2(235, 1)));
		Oneclip.AddFrame(UVS(glm::vec2(236, 235), glm::vec2(470, 1)));
		Oneclip.AddFrame(UVS(glm::vec2(471, 235), glm::vec2(705, 1)));
		Oneclip.AddFrame(UVS(glm::vec2(706, 235), glm::vec2(939, 1)));*/

		Oneclip.SetIsRepeating(false);
		Oneclip.SetSecPerFrame(1.0);
		anim->AddAnimation(Oneclip);
		m_PlayerHealth = anim->GetAnimation().size() - 1;
		anim->SetActiveAnim(m_PlayerHealth);

		healthent->AttachComponent<Sprite2D>(m_textures[0], healthent->GetID(), 15, 15, true, tempAnim);
		healthent->AttachComponent<Transform>().SetLocalPos(glm::vec3(-80, -80, -10));

		auto abilities = GameObject::Allocate();
		abilities->AttachComponent<Sprite2D>(m_textures[1], abilities->GetID(), 15, 15);
		abilities->AttachComponent<Transform>().SetLocalPos(glm::vec3(80, -80, -10));
		auto powerUp = GameObject::Allocate();
		powerUp->AttachComponent<Sprite2D>(m_textures[2], powerUp->GetID(), 6, 45);
		powerUp->AttachComponent<Transform>().SetLocalPos(glm::vec3(-90, 10, -10));


		glm::vec3 scorePos;
		auto score = GameObject::Allocate();
		score->AttachComponent<Sprite2D>(m_textures[int(TextureType::SCORE)], score->GetID(), 10, 5);
		score->AttachComponent<Transform>().SetLocalPos(-80, 80, -10);

		scorePos = score->GetComponent<Transform>().GetLocalPos();

		m_score = GameObject::Allocate();
		m_score->AttachComponent<ScoreHandler>(scorePos, m_textures[int(TextureType::SCORENUM)], camentity);


		//effects
		/*effect = GameObject::Allocate();
		effect->AttachComponent<PostEffect>().Init(Application::GetWindowWidth(), Application::GetWindowHeight());*/
		/*effect->AttachComponent<GreyscaleEffect>().Init(Application::GetWindowWidth(), Application::GetWindowHeight());
		effect->AttachComponent<SepiaEffect>().Init(Application::GetWindowWidth(), Application::GetWindowHeight());
		effect->AttachComponent<ColorCorrectionEffect>().Init(Application::GetWindowWidth(), Application::GetWindowHeight());
		effect->GetComponent<ColorCorrectionEffect>().AddLUT("Resource Files/LUTs/NeutralLUT.cube");*/

		if (m_name == "Universe_19") {

			m_SceneResumeNo = int(ScenesNum::UNIVERSE_19);

			// Solar System Centerpoint
			auto SVCEntity = GameObject::Allocate();
			m_solarSystem.push_back(SVCEntity->GetID());
			SVCEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 0));
			auto SYCEntity = GameObject::Allocate();
			m_solarSystem.push_back(SYCEntity->GetID());
			SYCEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 0));
			auto SKrCEntity = GameObject::Allocate();
			m_solarSystem.push_back(SKrCEntity->GetID());
			SKrCEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 0));
			auto SGCEntity = GameObject::Allocate();
			m_solarSystem.push_back(SGCEntity->GetID());
			SGCEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 0));
			auto SKeCEntity = GameObject::Allocate();
			m_solarSystem.push_back(SKeCEntity->GetID());
			SKeCEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 0));

			// Solari
			auto sunEntity = GameObject::Allocate();
			m_solarSystem.push_back(sunEntity->GetID());
			sunEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 0));
			sunEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), sunEntity->GetID(), *m_meshes[int(PlanetMesh::SOLARI)], nullptr, true);
			sunEntity->GetComponent<Transform>().SetRadius(3 * (m_meshes[int(PlanetMesh::SOLARI)]->GetWidth() / 2));
			sunEntity->GetComponent<Transform>().SetLocalScale(glm::vec3(3.0));

			// Verasten
			auto lavaPlanetEntity = GameObject::Allocate();
			m_solarSystem.push_back(lavaPlanetEntity->GetID());
			lavaPlanetEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 750));
			lavaPlanetEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), lavaPlanetEntity->GetID(), *m_meshes[int(PlanetMesh::VERASTEN)], nullptr);
			lavaPlanetEntity->GetComponent<Transform>().SetRadius((m_meshes[int(PlanetMesh::VERASTEN)]->GetWidth() / 2));
			
			//Comet
			auto Cometent = GameObject::Allocate();
			CometID = Cometent->GetID();
			//Cometent->AttachComponent<Transform>().SetLocalPos(glm::vec3(-12000, 2500, 6000));
			Cometent->AttachComponent<Transform>().SetLocalScale(glm::vec3(40, 40, 40));
			Cometent->AttachComponent<StaticRenderer>(cameraEntity->GetID(), Cometent->GetID(), *m_meshes[int(PlanetMesh::COMET)], m_textures[int(TextureType::COMET)], true);
			auto& cometPos = Cometent->GetComponent<Transform>().GetLocalPos();
			particleTemp = new ParticleController(1, glm::vec3(cometPos.x - 5, cometPos.y, cometPos.z), m_textures[int(TextureType::ORANGE)], m_textures[int(TextureType::FIRE)], CometID);
			particleTemp->setRotation(glm::vec3(0, -90, 0));
			particleTemp->getEmitter()->setRadius(3);
			particleTemp->setSize(50);
			particleTemp->getEmitter()->setAngleDeg(2);
			particleTemp->getEmitter()->setLifetime(1.5f, 5.0f);
			particleTemp->getEmitter()->setSpeed(5);
			particleTemp->getEmitter()->init();
			particles.push_back(particleTemp);
			glm::vec3 tempv3 = glm::vec3(-12000, 2500, 8000);
			Cometent->GetComponent<Transform>().MoveLocalPos(tempv3);

			// Yechin
			auto desertPlanetEntity = GameObject::Allocate();
			m_solarSystem.push_back(desertPlanetEntity->GetID());
			desertPlanetEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(1500, 0, 0));
			desertPlanetEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), desertPlanetEntity->GetID(), *m_meshes[int(PlanetMesh::YECHIN)], nullptr);
			desertPlanetEntity->GetComponent<Transform>().SetRadius((m_meshes[int(PlanetMesh::YECHIN)]->GetWidth() / 2));
			
			// Kerantia
			auto homePlanetEntity = GameObject::Allocate();
			m_solarSystem.push_back(homePlanetEntity->GetID());
			homePlanetEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, -2250));
			homePlanetEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), homePlanetEntity->GetID(), *m_meshes[int(PlanetMesh::KERANTIA)], nullptr);
			homePlanetEntity->GetComponent<Transform>().SetRadius((m_meshes[int(PlanetMesh::KERANTIA)]->GetWidth() / 2));
			
			// Lunari
			auto moonEntity = GameObject::Allocate();
			MoonID = moonEntity->GetID();
			moonEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 250));
			moonEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), moonEntity->GetID(), *m_meshes[int(PlanetMesh::LUNARI)], nullptr);
			moonEntity->GetComponent<Transform>().SetRadius((m_meshes[int(PlanetMesh::LUNARI)]->GetWidth() / 2));

			// Gueristis
			auto rockPlanetEntity = GameObject::Allocate();
			m_solarSystem.push_back(rockPlanetEntity->GetID());
			rockPlanetEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(-2875, 0, 0));
			rockPlanetEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), rockPlanetEntity->GetID(), *m_meshes[int(PlanetMesh::GUERISTIS)], nullptr);
			rockPlanetEntity->GetComponent<Transform>().SetRadius((m_meshes[int(PlanetMesh::GUERISTIS)]->GetWidth() / 2));

			// Keminth
			auto icePlanetEntity = GameObject::Allocate();
			m_solarSystem.push_back(icePlanetEntity->GetID());
			icePlanetEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 3500));
			icePlanetEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), icePlanetEntity->GetID(), *m_meshes[int(PlanetMesh::KEMINTH)], nullptr);
			icePlanetEntity->GetComponent<Transform>().SetRadius((m_meshes[int(PlanetMesh::KEMINTH)]->GetWidth() / 2));
			auto icePlanetRingEntity = GameObject::Allocate();
			m_solarSystem.push_back(icePlanetRingEntity->GetID());
			icePlanetRingEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 3500));
			icePlanetRingEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), icePlanetRingEntity->GetID(), *m_meshes[int(PlanetMesh::COMET) + 13], nullptr);

			// Home Planet Centerpoint
			auto HPCEntity = GameObject::Allocate();
			m_solarSystem.push_back(HPCEntity->GetID());
			HPCEntity->AttachComponent<Transform>().SetLocalPos(homePlanetEntity->GetComponent<Transform>().GetLocalPos());

			//Setting Solar System Parent/Childe
			lavaPlanetEntity->GetComponent<Transform>().SetParent(&m_solarSystem[Universe19SS::SVC]);
			desertPlanetEntity->GetComponent<Transform>().SetParent(&m_solarSystem[Universe19SS::SYC]);
			homePlanetEntity->GetComponent<Transform>().SetParent(&m_solarSystem[Universe19SS::SKRC]);
			moonEntity->GetComponent<Transform>().SetParent(&m_solarSystem[Universe19SS::HPC]);
			rockPlanetEntity->GetComponent<Transform>().SetParent(&m_solarSystem[Universe19SS::SGC]);
			icePlanetEntity->GetComponent<Transform>().SetParent(&m_solarSystem[Universe19SS::SKEC]);
			icePlanetRingEntity->GetComponent<Transform>().SetParent(&m_solarSystem[Universe19SS::SKEC]);
			HPCEntity->GetComponent<Transform>().SetParent(&m_solarSystem[Universe19SS::SKRC]);

			entt::entity* sunID = new entt::entity(sunEntity->GetID());
			entt::entity* lavaID = new entt::entity(lavaPlanetEntity->GetID());
			entt::entity* desertID = new entt::entity(desertPlanetEntity->GetID());
			entt::entity* rockID = new entt::entity(rockPlanetEntity->GetID());
			entt::entity* homeID = new entt::entity(homePlanetEntity->GetID());

			//Ice planet enemies
			for (int i = 0; i < 15; i++) {
				auto enemy = GameObject::Allocate();
				enemy->AttachComponent<Transform>();
				enemy->AttachComponent<KamakaziAI>(enemy->GetID(), icePlanetEntity->GetID(), playerEntity->GetID()).SetBulletMesh(m_meshes[int(PlayerMesh::PLAYERBULLET)]);;
				enemy->AttachComponent<EntityType>() = EntityType::KAMAKAZI;
				enemy->AttachComponent<StaticRenderer>(cameraEntity->GetID(), enemy->GetID(), *m_meshes[int(EnemyMesh::KAMAKAZI)], nullptr);
				enemy->GetComponent<Transform>().SetWHD(glm::vec3(m_meshes[int(EnemyMesh::KAMAKAZI)]->GetWidth(), m_meshes[int(EnemyMesh::KAMAKAZI)]->GetHeight(), m_meshes[int(EnemyMesh::KAMAKAZI)]->GetDepth()));

			}
			//Lava planet enemies
			for (int i = 0; i < 10; i++) {
				auto enemy = GameObject::Allocate();
				enemy->AttachComponent<Transform>();
				enemy->AttachComponent<BasicAI>(enemy->GetID(), lavaPlanetEntity->GetID(), playerEntity->GetID());
				enemy->AttachComponent<EntityType>() = EntityType::NEROTIST;
				enemy->AttachComponent<StaticRenderer>(cameraEntity->GetID(), enemy->GetID(), *m_meshes[int(EnemyMesh::NEROTISTU1)], nullptr);
				enemy->GetComponent<Transform>().SetWHD(glm::vec3(m_meshes[int(EnemyMesh::NEROTISTU1)]->GetWidth(), m_meshes[int(EnemyMesh::NEROTISTU1)]->GetHeight(), m_meshes[int(EnemyMesh::NEROTISTU1)]->GetDepth()));

			}
			//Rockplanet enemies
			for (int i = 0; i < 10; i++) {
				auto enemy = GameObject::Allocate();
				enemy->AttachComponent<Transform>();
				enemy->AttachComponent<BasicAI>(enemy->GetID(), rockPlanetEntity->GetID(), playerEntity->GetID());
				enemy->AttachComponent<EntityType>() = EntityType::NEROTIST;
				enemy->AttachComponent<StaticRenderer>(cameraEntity->GetID(), enemy->GetID(), *m_meshes[int(EnemyMesh::NEROTISTU1)], nullptr);
				enemy->GetComponent<Transform>().SetWHD(glm::vec3(m_meshes[int(EnemyMesh::NEROTISTU1)]->GetWidth(), m_meshes[int(EnemyMesh::NEROTISTU1)]->GetHeight(), m_meshes[int(EnemyMesh::NEROTISTU1)]->GetDepth()));
			}
			for (int i = 0; i < 10; i++) {
				auto enemy = GameObject::Allocate();
				enemy->AttachComponent<Transform>();
				enemy->AttachComponent<KamakaziAI>(enemy->GetID(), rockPlanetEntity->GetID(), playerEntity->GetID());
				enemy->AttachComponent<EntityType>() = EntityType::KAMAKAZI;
				enemy->AttachComponent<StaticRenderer>(cameraEntity->GetID(), enemy->GetID(), *m_meshes[int(EnemyMesh::KAMAKAZI)], nullptr);
				enemy->GetComponent<Transform>().SetWHD(glm::vec3(m_meshes[int(EnemyMesh::KAMAKAZI)]->GetWidth(), m_meshes[int(EnemyMesh::KAMAKAZI)]->GetHeight(), m_meshes[int(EnemyMesh::KAMAKAZI)]->GetDepth()));
			}
			//Kamikaze
			for (int i = 0; i < 20; i++) {
				auto enemy = GameObject::Allocate();
				enemy->AttachComponent<Transform>().SetLocalPos(glm::vec3(Random::Range1f(400, 900), 0, Random::Range1f(-800, -100)));
				enemy->AttachComponent<KamakaziAI>(enemy->GetID(), enemy->GetID(), playerEntity->GetID());
				enemy->AttachComponent<EntityType>() = EntityType::KAMAKAZI;
				enemy->AttachComponent<StaticRenderer>(cameraEntity->GetID(), enemy->GetID(), *m_meshes[int(EnemyMesh::KAMAKAZI)], nullptr);
				enemy->GetComponent<Transform>().SetWHD(glm::vec3(m_meshes[int(EnemyMesh::KAMAKAZI)]->GetWidth(), m_meshes[int(EnemyMesh::KAMAKAZI)]->GetHeight(), m_meshes[int(EnemyMesh::KAMAKAZI)]->GetDepth()));
			}
			//Bombaridier
			for (int i = 0; i < 20; i++) {
				auto enemy = GameObject::Allocate();
				enemy->AttachComponent<Transform>().SetLocalPos(glm::vec3(Random::Range1f(-400, -900), 0, Random::Range1f(-500, 500)));
				enemy->AttachComponent<BombardierAI>(enemy->GetID(), enemy->GetID(), playerEntity->GetID());
				enemy->AttachComponent<EntityType>() = EntityType::BOMBARDIER;
				enemy->AttachComponent<StaticRenderer>(cameraEntity->GetID(), enemy->GetID(), *m_meshes[int(EnemyMesh::BOMBARDIER)], nullptr);
				enemy->GetComponent<Transform>().SetWHD(glm::vec3(m_meshes[int(EnemyMesh::BOMBARDIER)]->GetWidth(), m_meshes[int(EnemyMesh::BOMBARDIER)]->GetHeight(), m_meshes[int(EnemyMesh::BOMBARDIER)]->GetDepth()));
			}
			
			//Asteroids
			{
				//Asteroid belt
				auto asteroidBelt = GameObject::Allocate();
				AsteroidBeltID = asteroidBelt->GetID();
				asteroidBelt->AttachComponent<Transform>().SetLocalPos(glm::vec3(0));
				asteroidBelt->GetComponent<Transform>().SetLocalScale(glm::vec3(50, 50, 50));
				//asteroidBelt->GetComponent<Transform>();
				asteroidBelt->AttachComponent<StaticRenderer>(cameraEntity->GetID(), asteroidBelt->GetID(), *m_meshes[int(PlanetMesh::COMET) + 4], nullptr);
				asteroidBelt->GetComponent<Transform>().SetParent(&m_solarSystem[Universe19SS::SVC]);
					
				//Asteroid Clusters
				auto cluster1 = GameObject::Allocate();
				cluster1->AttachComponent<Transform>().SetLocalPos(glm::vec3(0));
				cluster1->GetComponent<Transform>().SetLocalScale(glm::vec3(50, 50, 50));
				cluster1->AttachComponent<StaticRenderer>(cameraEntity->GetID(), cluster1->GetID(), *m_meshes[int(PlanetMesh::COMET) + 5], nullptr);
				cluster1->GetComponent<Transform>().SetParent(&m_solarSystem[Universe19SS::SKRC]);

				auto cluster2 = GameObject::Allocate();
				cluster2->AttachComponent<Transform>().SetLocalPos(glm::vec3(0));
				cluster2->GetComponent<Transform>().SetLocalScale(glm::vec3(50, 50, 50));
				cluster2->AttachComponent<StaticRenderer>(cameraEntity->GetID(), cluster2->GetID(), *m_meshes[int(PlanetMesh::COMET) + 6], nullptr);
				cluster2->GetComponent<Transform>().SetParent(&m_solarSystem[Universe19SS::SKRC]);

				auto cluster3 = GameObject::Allocate();
				cluster3->AttachComponent<Transform>().SetLocalPos(glm::vec3(0));
				cluster3->GetComponent<Transform>().SetLocalScale(glm::vec3(50, 50, 50));
				cluster3->AttachComponent<StaticRenderer>(cameraEntity->GetID(), cluster3->GetID(), *m_meshes[int(PlanetMesh::COMET) + 7], nullptr);
				cluster3->GetComponent<Transform>().SetParent(&m_solarSystem[Universe19SS::SKRC]);

				auto cluster4 = GameObject::Allocate();
				cluster4->AttachComponent<Transform>().SetLocalPos(glm::vec3(0));
				cluster4->GetComponent<Transform>().SetLocalScale(glm::vec3(50, 50, 50));
				cluster4->AttachComponent<StaticRenderer>(cameraEntity->GetID(), cluster4->GetID(), *m_meshes[int(PlanetMesh::COMET) + 8], nullptr);
				cluster4->GetComponent<Transform>().SetParent(&m_solarSystem[Universe19SS::SKRC]);

				auto cluster5 = GameObject::Allocate();
				cluster5->AttachComponent<Transform>().SetLocalPos(glm::vec3(0));
				cluster5->GetComponent<Transform>().SetLocalScale(glm::vec3(50, 50, 50));
				cluster5->AttachComponent<StaticRenderer>(cameraEntity->GetID(), cluster5->GetID(), *m_meshes[int(PlanetMesh::COMET) + 9], nullptr);
				cluster5->GetComponent<Transform>().SetParent(&m_solarSystem[Universe19SS::SKRC]);

				auto cluster6 = GameObject::Allocate();
				cluster6->AttachComponent<Transform>().SetLocalPos(glm::vec3(0));
				cluster6->GetComponent<Transform>().SetLocalScale(glm::vec3(50, 50, 50));
				cluster6->AttachComponent<StaticRenderer>(cameraEntity->GetID(), cluster6->GetID(), *m_meshes[int(PlanetMesh::COMET) + 10], nullptr);
				cluster6->GetComponent<Transform>().SetParent(&m_solarSystem[Universe19SS::SKRC]);

				auto cluster7 = GameObject::Allocate();
				cluster7->AttachComponent<Transform>().SetLocalPos(glm::vec3(0));
				cluster7->GetComponent<Transform>().SetLocalScale(glm::vec3(50, 50, 50));
				cluster7->AttachComponent<StaticRenderer>(cameraEntity->GetID(), cluster7->GetID(), *m_meshes[int(PlanetMesh::COMET) + 11], nullptr);
				cluster7->GetComponent<Transform>().SetParent(&m_solarSystem[Universe19SS::SKRC]);

				auto cluster8 = GameObject::Allocate();
				cluster8->AttachComponent<Transform>().SetLocalPos(glm::vec3(0));
				cluster8->GetComponent<Transform>().SetLocalScale(glm::vec3(50, 50, 50));
				cluster8->AttachComponent<StaticRenderer>(cameraEntity->GetID(), cluster8->GetID(), *m_meshes[int(PlanetMesh::COMET) + 12], nullptr);
				cluster8->GetComponent<Transform>().SetParent(&m_solarSystem[Universe19SS::SKRC]);
			}

			//DEBRIS
			for (int i = 0; i <= 50; i++) {
				auto debris = GameObject::Allocate();
				Transform refrence;
				
				if (i <= 10)
				{
					//Barrel top
					debris->AttachComponent<Transform>().SetLocalPos(glm::vec3(Random::Range1f(-1500, 1500), Random::Range1f(10, 200), Random::Range1f(1100, 3800)));
					debris->GetComponent<Transform>().SetLocalScale(glm::vec3(2, 2, 2));
					debris->GetComponent<Transform>().SetLocalRot(Random::Range3f(-300, 300));
					debris->AttachComponent<StaticRenderer>(cameraEntity->GetID(), debris->GetID(), *m_meshes[int(PlanetMesh::COMET) + 1], nullptr);
					debris->GetComponent<Transform>().SetParent(&m_solarSystem[Universe19SS::SVC]);
				}
				else if (i > 10 && i <= 20)
				{
					//Barrel bot
					debris->AttachComponent<Transform>().SetLocalPos(glm::vec3(Random::Range1f(-1500, 1500), Random::Range1f(-200, -10), Random::Range1f(1100, 3800)));
					debris->GetComponent<Transform>().SetLocalScale(glm::vec3(2, 2, 2));
					debris->GetComponent<Transform>().SetLocalRot(Random::Range3f(-300, 300));
					debris->AttachComponent<StaticRenderer>(cameraEntity->GetID(), debris->GetID(), *m_meshes[int(PlanetMesh::COMET) + 1], nullptr);
					debris->GetComponent<Transform>().SetParent(&m_solarSystem[Universe19SS::SVC]);
				}
				else if (i > 20 && i <= 30)
				{
					//Bottle top
					debris->AttachComponent<Transform>().SetLocalPos(glm::vec3(Random::Range1f(-2000, 2000), Random::Range1f(10, 300), Random::Range1f(1200, 4000)));
					debris->GetComponent<Transform>().SetLocalScale(glm::vec3(4, 4, 4));
					debris->GetComponent<Transform>().SetLocalRot(Random::Range3f(-300, 300));
					debris->AttachComponent<StaticRenderer>(cameraEntity->GetID(), debris->GetID(), *m_meshes[int(PlanetMesh::COMET) + 2], nullptr);
					debris->GetComponent<Transform>().SetParent(&m_solarSystem[Universe19SS::SVC]);
				}
				else if (i > 30 && i <= 50)
				{
					//Box bot
					debris->AttachComponent<Transform>().SetLocalPos(glm::vec3(Random::Range1f(-2000, 2000), Random::Range1f(-300, -10), Random::Range1f(1200, 4000)));
					debris->GetComponent<Transform>().SetLocalScale(glm::vec3(4, 4, 4));
					debris->GetComponent<Transform>().SetLocalRot(Random::Range3f(-300, 300));
					debris->AttachComponent<StaticRenderer>(cameraEntity->GetID(), debris->GetID(), *m_meshes[int(PlanetMesh::COMET) + 3], nullptr);
					debris->GetComponent<Transform>().SetParent(&m_solarSystem[Universe19SS::SVC]);
				}
			}

		}
		else if (m_name == "Universe_27") {

			m_SceneResumeNo = int(ScenesNum::UNIVERSE_27);

			// Solar System Centerpoint
			auto LDmCEntity = GameObject::Allocate();
			m_solarSystem.push_back(LDmCEntity->GetID());
			LDmCEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 0));
			auto LTCEntity = GameObject::Allocate();
			m_solarSystem.push_back(LTCEntity->GetID());
			LTCEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 0));
			auto LKCEntity = GameObject::Allocate();
			m_solarSystem.push_back(LKCEntity->GetID());
			LKCEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 0));
			auto LPCEntity = GameObject::Allocate();
			m_solarSystem.push_back(LPCEntity->GetID());
			LPCEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 0));
			auto LDrCEntity = GameObject::Allocate();
			m_solarSystem.push_back(LDrCEntity->GetID());
			LDrCEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 0));
			auto LRCEntity = GameObject::Allocate();
			m_solarSystem.push_back(LRCEntity->GetID());
			LRCEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 0));
			auto LMCEntity = GameObject::Allocate();
			m_solarSystem.push_back(LMCEntity->GetID());
			LMCEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 0));

			// Lutero
			auto sunEntity = GameObject::Allocate();
			m_solarSystem.push_back(sunEntity->GetID());
			sunEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 0));
			sunEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), sunEntity->GetID(), *m_meshes[int(PlanetMesh::LUTERO)], nullptr, true);
			sunEntity->GetComponent<Transform>().SetRadius(3 * (m_meshes[int(PlanetMesh::LUTERO)]->GetWidth() / 2));
			sunEntity->GetComponent<Transform>().SetLocalScale(glm::vec3(3.0));

			// Dedmos
			auto nullPlanetOneEntity = GameObject::Allocate();
			m_solarSystem.push_back(nullPlanetOneEntity->GetID());
			nullPlanetOneEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 750));
			nullPlanetOneEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), nullPlanetOneEntity->GetID(), *m_meshes[int(PlanetMesh::DEDMOS)], nullptr);

			// Titanius
			auto nullPlanetTwoEntity = GameObject::Allocate();
			m_solarSystem.push_back(nullPlanetTwoEntity->GetID());
			nullPlanetTwoEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(1500, 0, 0));
			nullPlanetTwoEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), nullPlanetTwoEntity->GetID(), *m_meshes[int(PlanetMesh::TITANIUS)], nullptr);

			// Kreillo
			auto nullPlanetThreeEntity = GameObject::Allocate();
			m_solarSystem.push_back(nullPlanetThreeEntity->GetID());
			nullPlanetThreeEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, -2250));
			nullPlanetThreeEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), nullPlanetThreeEntity->GetID(), *m_meshes[int(PlanetMesh::KREILLO)], nullptr);

			// Paxallus
			auto citadelEntity = GameObject::Allocate();
			m_solarSystem.push_back(citadelEntity->GetID());
			citadelEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(-2875, 0, 0));
			citadelEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), citadelEntity->GetID(), *m_meshes[int(PlanetMesh::PAXALLUS)], nullptr);

			// Derangi
			auto mushroomPlanetEntity = GameObject::Allocate();
			m_solarSystem.push_back(mushroomPlanetEntity->GetID());
			mushroomPlanetEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 3500));
			mushroomPlanetEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), mushroomPlanetEntity->GetID(), *m_meshes[int(PlanetMesh::DERANGI)], nullptr);

			// Rhetoid
			auto gasPlanetOneEntity = GameObject::Allocate();
			m_solarSystem.push_back(gasPlanetOneEntity->GetID());
			gasPlanetOneEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(4250, 0, 0));
			gasPlanetOneEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), gasPlanetOneEntity->GetID(), *m_meshes[int(PlanetMesh::RHETOID)], nullptr);

			// Magaantu
			auto gasPlanetTwoEntity = GameObject::Allocate();
			m_solarSystem.push_back(gasPlanetTwoEntity->GetID());
			gasPlanetTwoEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, -5000));
			gasPlanetTwoEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), gasPlanetTwoEntity->GetID(), *m_meshes[int(PlanetMesh::MAGAANTU)], nullptr);
		}
		else if (m_name == "Universe_5") {
			m_SceneResumeNo = int(ScenesNum::UNIVERSE_5);
		}


	

		//Setting Parent/Childe
		cameraEntity->GetComponent<Transform>().SetParent(new entt::entity(playerEntity->GetID()));
		healthent->GetComponent<Transform>().SetParent(camentity);
		abilities->GetComponent<Transform>().SetParent(camentity);
		powerUp->GetComponent<Transform>().SetParent(camentity);
		score->GetComponent<Transform>().SetParent(camentity);

		
	}

	Skybox::Init(m_name);
}

void Universe::Update(float deltaTime)
{
	m_deltaTime = deltaTime;

	// Key Input
	KeyInput();
	GamepadInput();

	// Camera Update 
	camera->Update();

	playerController->Update(deltaTime);

	// Solar System Rotation (IN-PROGRESS) 
	SolarSystemUpdate();

	// Transform Update
	m_sceneReg->view<Transform>().each([=](Transform& transform) { transform.UpdateGlobal(); });
	m_sceneReg->view<AnimationHandler>().each([=](AnimationHandler& anim) { anim.Update(deltaTime); });

	//Particle
	for (int i = 0; i <= particles.size() - 1; i++)
	{
		if (particles[i]->getEmitter()->getDone())
		{
			delete particles[i];
			particles.erase(particles.begin() + i);
		}
		else
			particles[i]->update(deltaTime, camera->GetProj(), camera->GetView());
	}

	//particles[2]->update(deltaTime, camera->GetProj(), camera->GetView(), glm::mat4(1));

#pragma region Collision

	//Enemy collisions with the player as well as bullets
	std::vector<BasicAI*> AI;
	for (auto enemy : m_sceneReg->view<EntityType>()) {
		int index = AI.size();
		EntityType type = GameObject::GetComponent<EntityType>(enemy);

		if (type == EntityType::NEROTIST) {
			AI.push_back(&GameObject::GetComponent<BasicAI>(enemy));
			AI[index]->Update(deltaTime);
		}
		else if (type == EntityType::KAMAKAZI) {
			AI.push_back(&GameObject::GetComponent<KamakaziAI>(enemy));
			AI[index]->Update(deltaTime);

			if (isBoxCollide(GameObject::GetComponent<Transform>(enemy), GameObject::GetComponent<Transform>(MainPlayerID))) {
				AI.pop_back();
				particleTemp = new ParticleController(2, GameObject::GetComponent<Transform>(enemy).GetLocalPos(), m_textures[int(TextureType::ORANGE)], enemy);
				particleTemp->setSize(10);
				particleTemp->getEmitter()->setLifetime(0.2, 0.2);
				particleTemp->getEmitter()->setSpeed(100);
				particleTemp->getEmitter()->init();
				particleTemp->setModelMatrix(GameObject::GetComponent<Transform>(enemy).UpdateGlobal());
				particles.push_back(particleTemp);
				m_sceneReg->destroy(enemy);
				m_PlayerHealth -= m_PlayerHealth > 0 ? 1 : 0;
				GameObject::GetComponent<AnimationHandler>(health).SetActiveAnim(m_PlayerHealth);
			}
		}
		else if (type == EntityType::SCAVENGER) {
			AI.push_back(&GameObject::GetComponent<ScavengerAI>(enemy));
			AI[index]->Update(deltaTime);
		}
		else if (type == EntityType::BOMBARDIER) {
			AI.push_back(&GameObject::GetComponent<BombardierAI>(enemy));
			AI[index]->Update(deltaTime);
		}

		else if (type == EntityType::KAMABULLET) {
			GameObject::GetComponent<KamakaziBullet>(enemy).Update(deltaTime);

			if (GameObject::GetComponent<KamakaziBullet>(enemy).GetDestroyed() || isBoxCollide(GameObject::GetComponent<Transform>(enemy), GameObject::GetComponent<Transform>(MainPlayerID))) {
				particleTemp = new ParticleController(2, GameObject::GetComponent<Transform>(enemy).GetLocalPos(), m_textures[int(TextureType::ORANGE)], enemy);
				particleTemp->setSize(10);
				particleTemp->getEmitter()->setLifetime(0.2, 0.2);
				particleTemp->getEmitter()->setSpeed(100);
				particleTemp->getEmitter()->init();
				GameObject::GetComponent<Transform>(enemy).SetLocalScale(glm::vec3(1));
				particleTemp->setModelMatrix(GameObject::GetComponent<Transform>(enemy).UpdateGlobal());
				GameObject::GetComponent<Transform>(enemy).SetLocalScale(glm::vec3(5))->UpdateGlobal();
				particles.push_back(particleTemp);
				m_sceneReg->destroy(enemy);
				m_PlayerHealth -= m_PlayerHealth > 0 ? 1 : 0;
				GameObject::GetComponent<AnimationHandler>(health).SetActiveAnim(m_PlayerHealth);
			}
		}
	}

	//Bullet and enemy collision update
	for (auto Bulletentity : m_sceneReg->view<Projectile>()) {
		

		GameObject::GetComponent<Projectile>(Bulletentity).Update(deltaTime);

		if (GameObject::GetComponent<Projectile>(Bulletentity).GetDestroyed()) {
			m_sceneReg->destroy(Bulletentity);
			continue;
		}

		if (GameObject::GetComponent<EntityType>(Bulletentity) == EntityType::PLAYER) {
			for (int i = 0; i < AI.size(); i++) {
				entt::entity enemy = AI[i]->GetID();
				if (isBoxCollide(GameObject::GetComponent<Transform>(Bulletentity), GameObject::GetComponent<Transform>(AI[i]->GetID())))
				{
					particleTemp = new ParticleController(2, GameObject::GetComponent<Transform>(enemy).GetLocalPos(), m_textures[int(TextureType::ORANGE)], enemy);
					particleTemp->setSize(10);
					particleTemp->getEmitter()->setLifetime(0.2, 0.2);
					particleTemp->getEmitter()->setSpeed(100);
					particleTemp->getEmitter()->init();
					particleTemp->setModelMatrix(GameObject::GetComponent<Transform>(enemy).UpdateGlobal());
					particles.push_back(particleTemp);

					if (GameObject::GetComponent<EntityType>(enemy) == EntityType::KAMAKAZI)
						m_score->GetComponent<ScoreHandler>().Add(1);
					else if (GameObject::GetComponent<EntityType>(enemy) == EntityType::NEROTIST)
						m_score->GetComponent<ScoreHandler>().Add(5);
					else if (GameObject::GetComponent<EntityType>(enemy) == EntityType::BOMBARDIER)
						m_score->GetComponent<ScoreHandler>().Add(10);

					m_sceneReg->destroy(enemy);
					AI.erase(AI.begin() + i);
					
					
			
					
					m_sceneReg->destroy(Bulletentity);
					break;
				}
			}
		}

		else if (GameObject::GetComponent<EntityType>(Bulletentity) == EntityType::ENEMY) {
			if (isBoxCollide(GameObject::GetComponent<Transform>(Bulletentity), GameObject::GetComponent<Transform>(MainPlayerID)))
			{
				m_sceneReg->destroy(Bulletentity);
				m_PlayerHealth -= m_PlayerHealth > 0 ? 1 : 0;
				GameObject::GetComponent<AnimationHandler>(health).SetActiveAnim(m_PlayerHealth);
			}
		}
		
	}


	if (m_name == "Universe_19") {
		//collisions of player with planets
		for (int i = Universe19SS::SOLARI + 1; i <= Universe19SS::KEMINTH; i++) {
			if (isBoxCircleCollide(GameObject::GetComponent<Transform>(MainPlayerID), GameObject::GetComponent<Transform>(m_solarSystem[i]))) {
				m_PlayerHealth = 0;
				GameObject::GetComponent<AnimationHandler>(health).SetActiveAnim(m_PlayerHealth);
			}
		}

		if (m_score->GetComponent<ScoreHandler>().GetScore() >= 100 && !m_isBossSpawn)
		{
			// JELLYFIH BOSS
			auto jellyEntity = GameObject::Allocate();
			
			jellyEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0));
			auto& jellypos = jellyEntity->GetComponent<Transform>();

			particleTemp = new ParticleController(1, glm::vec3(jellypos.GetLocalPos().x - 6, jellypos.GetLocalPos().y + 13.0, jellypos.GetLocalPos().z + 6), m_textures[int(TextureType::YELLOW)], m_textures[int(TextureType::ORANGE)], jellyEntity->GetID());
			particleTemp->setRotation(glm::vec3(90, 0, 0));
			particleTemp->getEmitter()->setRadius(1.0);
			particleTemp->setSize(2);
			particleTemp->getEmitter()->setAngleDeg(10);
			particleTemp->getEmitter()->setLifetime(0.1f, 1.5f);
			particleTemp->getEmitter()->setSpeed(10);
			particleTemp->getEmitter()->init();
			particles.push_back(particleTemp);
	
			particleTemp = new ParticleController(1, glm::vec3(jellypos.GetLocalPos().x + 6, jellypos.GetLocalPos().y + 13.0, jellypos.GetLocalPos().z + 6), m_textures[int(TextureType::YELLOW)], m_textures[int(TextureType::ORANGE)], jellyEntity->GetID());
			particleTemp->setRotation(glm::vec3(90, 0, 0));
			particleTemp->getEmitter()->setRadius(1.0);
			particleTemp->setSize(2);
			particleTemp->getEmitter()->setAngleDeg(10);
			particleTemp->getEmitter()->setLifetime(0.1f, 1.5f);
			particleTemp->getEmitter()->setSpeed(10);
			particleTemp->getEmitter()->init();
			particles.push_back(particleTemp);

			particleTemp = new ParticleController(1, glm::vec3(jellypos.GetLocalPos().x - 6, jellypos.GetLocalPos().y + 13.0, jellypos.GetLocalPos().z - 6), m_textures[int(TextureType::YELLOW)], m_textures[int(TextureType::ORANGE)], jellyEntity->GetID());
			particleTemp->setRotation(glm::vec3(90, 0, 0));
			particleTemp->getEmitter()->setRadius(1.0);
			particleTemp->setSize(2);
			particleTemp->getEmitter()->setAngleDeg(10);
			particleTemp->getEmitter()->setLifetime(0.1f, 1.5f);
			particleTemp->getEmitter()->setSpeed(10);
			particleTemp->getEmitter()->init();
			particles.push_back(particleTemp);
			
			particleTemp = new ParticleController(1, glm::vec3(jellypos.GetLocalPos().x + 6, jellypos.GetLocalPos().y + 13.0, jellypos.GetLocalPos().z - 6), m_textures[int(TextureType::YELLOW)], m_textures[int(TextureType::ORANGE)], jellyEntity->GetID());
			particleTemp->setRotation(glm::vec3(90, 0, 0));
			particleTemp->getEmitter()->setRadius(1.0);
			particleTemp->setSize(2);
			particleTemp->getEmitter()->setAngleDeg(10);
			particleTemp->getEmitter()->setLifetime(0.1f, 1.5f);
			particleTemp->getEmitter()->setSpeed(10);
			particleTemp->getEmitter()->init();
			particles.push_back(particleTemp);

			glm::vec3 temp = glm::vec3(GameObject::GetComponent<Transform>(MainPlayerID).GetLocalPos().x + 100,
				GameObject::GetComponent<Transform>(MainPlayerID).GetLocalPos().y - 30,
				GameObject::GetComponent<Transform>(MainPlayerID).GetLocalPos().z + 100);

			jellypos.MoveLocalPos(temp);
			jellyEntity->AttachComponent<StaticRenderer>(CamID, jellyEntity->GetID(), *m_meshes[int(EnemyMesh::JELLYFISH)], nullptr, true);
			jellyEntity->GetComponent<Transform>().SetLocalScale(glm::vec3(3.0));


			m_isBossSpawn = true;
		}
	}

	else if(m_name == "Universe_27")
	{

	}

	else if (m_name == "Universe_5") {

	}
#pragma endregion
}

void Universe::Render(float deltaTime)
{
	//effect->GetComponent<PostEffect>().Clear();
	//effect->GetComponent<ColorCorrectionEffect>().Clear();

	//effect->GetComponent<PostEffect>().BindBuffer(0);

	m_sceneReg->view<StaticRenderer>().each([=](StaticRenderer& renderer) { renderer.Draw(); });
	m_sceneReg->view<DynamicRenderer>().each([=](DynamicRenderer& renderer) { renderer.Draw(); });
	Skybox::Draw(camera->GetView(), camera->GetProj());

	//Particles
	for (auto& i : this->particles)
		i->draw();

	m_sceneReg->view<Sprite2D>().each([=](Sprite2D& renderer) {renderer.Draw(camera); });
	
	//effect->GetComponent<PostEffect>().UnbindBuffer();
	
	//effect->GetComponent<ColorCorrectionEffect>().ApplyEffect(&effect->GetComponent<PostEffect>());
	//effect->GetComponent<ColorCorrectionEffect>().DrawToScreen();
	//effect->GetComponent<PostEffect>().DrawToScreen();

}

void Universe::KeyInput()
{
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		*switchIt = true;
		*SceneNo = int(ScenesNum::PAUSE_MENU);
	}

	// Scene Switching //
	if (glfwGetKey(m_window, GLFW_KEY_0) == GLFW_PRESS) {
		*switchIt = true;
		*SceneNo = int(ScenesNum::START_SCREEN);
	}
	if (glfwGetKey(m_window, GLFW_KEY_1) == GLFW_PRESS) {
		*switchIt = true;
		*SceneNo = int(ScenesNum::UNIVERSE_19);
	}
	if (glfwGetKey(m_window, GLFW_KEY_2) == GLFW_PRESS) {
		*switchIt = true;
		*SceneNo = int(ScenesNum::UNIVERSE_27);
	}

	// Player Movement //
	auto& playerEnt = GameObject::GetComponent<Transform>(MainPlayerID);
	auto& camEnt = GameObject::GetComponent<Transform>(CamID);
	auto& moonEnt = GameObject::GetComponent<Transform>(MoonID);

	if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		glm::vec3 temp = glm::vec3(0, 0, -6);
		playerEnt.MoveLocalPos(temp);
	}
	else if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		glm::vec3 temp = glm::vec3(0, 0, 6);
		playerEnt.MoveLocalPos(temp);
	}
	if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		glm::vec3 temp = glm::vec3(0.0f, 1.0f, 0.0f);
		playerEnt.RotateLocal(temp);
	}
	else if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		glm::vec3 temp = glm::vec3(0.0f, -1.0f, 0.0f);
		playerEnt.RotateLocal(temp);
	}

	//CamMove
	if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
	{
		glm::vec3 temp = glm::vec3(0, 0, -1.5);
		camEnt.MoveLocalPos(temp);
	}
	else if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
	{
		glm::vec3 temp = glm::vec3(0, 0, 1.5);
		camEnt.MoveLocalPos(temp);
	}
	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
	{
		glm::vec3 temp = glm::vec3(0.0f, .5f, 0.0f);
		camEnt.RotateLocal(temp);
	}
	else if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
	{
		glm::vec3 temp = glm::vec3(0.0f, -.5f, 0.0f);
		camEnt.RotateLocal(temp);
	}
	if (glfwGetKey(m_window, GLFW_KEY_Z) == GLFW_PRESS)
	{
		glm::vec3 temp = glm::vec3(0.0f, 2.5f, 0.0f);
		camEnt.MoveLocalPos(temp);
	}
	else if (glfwGetKey(m_window, GLFW_KEY_C) == GLFW_PRESS)
	{
		glm::vec3 temp = glm::vec3(0.0f, -2.5f, 0.0f);
		camEnt.MoveLocalPos(temp);
	}
	if (glfwGetKey(m_window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		glm::vec3 temp = glm::vec3(-1.0f, 0.0f, 0.0f);
		camEnt.MoveLocalPos(temp);
	}
	else if (glfwGetKey(m_window, GLFW_KEY_E) == GLFW_PRESS)
	{
		glm::vec3 temp = glm::vec3(1.0f, 0.0f, 0.0f);
		camEnt.MoveLocalPos(temp);
	}
	if (glfwGetKey(m_window, GLFW_KEY_F) == GLFW_PRESS && !isPlayerAnim) {
		playerController->SetAnimate(true);
		isPlayerAnim = true;
	}
	if (glfwGetKey(m_window, GLFW_KEY_F) == GLFW_RELEASE) {
		isPlayerAnim = false;
	}

	if (glfwGetKey(m_window, GLFW_KEY_P) == GLFW_PRESS && !isexplode) {
		//explosion
		particleTemp = new ParticleController(2, moonEnt.GetLocalPos(), new Texture("Resource Files/Textures/red.png"), MoonID);
		particleTemp->setSize(10);
		particleTemp->getEmitter()->setLifetime(.8, .8);
		particleTemp->getEmitter()->setSpeed(100);
		particleTemp->getEmitter()->init();
		particles.push_back(particleTemp);
		isexplode = true;

	}
	if (glfwGetKey(m_window, GLFW_KEY_P) == GLFW_RELEASE) {
		isexplode = false;
	}

	if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		
		if (glfwGetTime() - m_startTime >= m_fireRate) {
			// Shoot Bullet Right
			

			auto RightBullet = GameObject::Allocate();
			RightBullet->AttachComponent<Projectile>(&MainPlayerID, entt::entity(0), RightBullet.get(), *m_meshes[int(PlayerMesh::PLAYERBULLET)]).SetID(RightBullet->GetID());
			RightBullet->GetComponent<Projectile>().SetSpeed(500);
			RightBullet->GetComponent<Projectile>().SetVelocity(glm::vec3(0, 0, -1));
			RightBullet->AttachComponent<EntityType>() = EntityType::PLAYER;
			glm::vec3 offset1 = glm::vec3(3, 0, -10);
			RightBullet->GetComponent<Transform>().MoveLocalPos(offset1);
			RightBullet->GetComponent<Transform>().SetLocalScale(glm::vec3(3));

            // Shoot Bullet Left
			auto LeftBullet = GameObject::Allocate();
			LeftBullet->AttachComponent<Projectile>(&MainPlayerID, entt::entity(0), LeftBullet.get(), *m_meshes[int(PlayerMesh::PLAYERBULLET)]).SetID(LeftBullet->GetID());
			LeftBullet->GetComponent<Projectile>().SetSpeed(500);
			LeftBullet->AttachComponent<EntityType>() = EntityType::PLAYER;
			LeftBullet->GetComponent<Projectile>().SetVelocity(glm::vec3(0, 0, -1));
			glm::vec3 offset2 = glm::vec3(-3, 0, -10);
			LeftBullet->GetComponent<Transform>().MoveLocalPos(offset2);
			LeftBullet->GetComponent<Transform>().SetLocalScale(glm::vec3(3));

			//Reset time to fire
			m_resetTime = true;
		}

		if (m_resetTime) {
			m_startTime = glfwGetTime();
			m_resetTime = false;
		}
	}

	// Toggle Textures //
	if (glfwGetKey(m_window, GLFW_KEY_T) == GLFW_PRESS && !texTglPressed) {
		m_sceneReg->view<StaticRenderer>().each([=](StaticRenderer& renderer) { renderer.toggleTexture(); });
		texTglPressed = true;
	}
	if (glfwGetKey(m_window, GLFW_KEY_T) == GLFW_RELEASE) {
		texTglPressed = false;
	}
}

void Universe::GamepadInput()
{

	if (gamepad.getGamepadInput()) {
		auto& playerEnt = GameObject::GetComponent<Transform>(MainPlayerID);
		glm::vec3 temp; 
		

		if (gamepad.getGamepadInput()) {
			//button input
			if (gamepad.buttons.A) {

			}
			else if (gamepad.buttons.B) {

			}
			else if (gamepad.buttons.X) {
				if (gamepad.axes[Joystick::LEFT].X < -0.17f)
				{
					tempDir = -1;
					isDodge = true;
				}
				if (gamepad.axes[Joystick::LEFT].X > 0.17f)
				{
					tempDir = 1;
					isDodge = true;
				}			
			}
			else if (gamepad.buttons.Y) {

			}

			if (!isDodge)
			{
				//Left Trigger
				{

					if (gamepad.trigger.LT > -1 && gamepad.trigger.LT < -0.8) {
						temp = glm::vec3(0, 0, -0.2);
						playerEnt.MoveLocalPos(temp);

					}
					else if (gamepad.trigger.LT > -0.8 && gamepad.trigger.LT < -0.6) {
						temp = glm::vec3(0, 0, -0.4);
						playerEnt.MoveLocalPos(temp);
					}
					else if (gamepad.trigger.LT > -0.6 && gamepad.trigger.LT < -0.4) {
						temp = glm::vec3(0, 0, -0.6);
						playerEnt.MoveLocalPos(temp);
					}
					else if (gamepad.trigger.LT > -0.4 && gamepad.trigger.LT < -0.2) {
						temp = glm::vec3(0, 0, -0.8);
						playerEnt.MoveLocalPos(temp);
					}
					else if (gamepad.trigger.LT > -0.2 && gamepad.trigger.LT < 0) {
						temp = glm::vec3(0, 0, -1.0);
						playerEnt.MoveLocalPos(temp);
					}
					else if (gamepad.trigger.LT > 0 && gamepad.trigger.LT < 0.2) {
						temp = glm::vec3(0, 0, -1.2);
						playerEnt.MoveLocalPos(temp);
					}
					else if (gamepad.trigger.LT > 0.2 && gamepad.trigger.LT < 0.4) {
						temp = glm::vec3(0, 0, -1.4);
						playerEnt.MoveLocalPos(temp);
					}
					else if (gamepad.trigger.LT > 1 && gamepad.trigger.LT < 0.6) {
						temp = glm::vec3(0, 0, -1.6);
						playerEnt.MoveLocalPos(temp);
					}
					else if (gamepad.trigger.LT > 1 && gamepad.trigger.LT < 0.8) {
						temp = glm::vec3(0, 0, -1.8);
						playerEnt.MoveLocalPos(temp);
					}
					else if (gamepad.trigger.LT > 0.8) {
						temp = glm::vec3(0, 0, -2);
						playerEnt.MoveLocalPos(temp);
					}

				}

				//Right Trigger
				{

					if (gamepad.trigger.RT > -0.8) {
						if (glfwGetTime() - m_startTime >= m_fireRate) {
							// Shoot Bullet Right
							auto RightBullet = GameObject::Allocate();
							RightBullet->AttachComponent<Projectile>(&MainPlayerID, entt::entity(0), RightBullet.get(), *m_meshes[int(PlayerMesh::PLAYERBULLET)]).SetID(RightBullet->GetID());
							RightBullet->GetComponent<Projectile>().SetSpeed(500);
							RightBullet->GetComponent<Projectile>().SetVelocity(glm::vec3(0, 0, -1));
							RightBullet->AttachComponent<EntityType>() = EntityType::PLAYER;
							glm::vec3 offset1 = glm::vec3(3, 0, -10);
							RightBullet->GetComponent<Transform>().MoveLocalPos(offset1);
							RightBullet->GetComponent<Transform>().SetLocalScale(glm::vec3(3));

							// Shoot Bullet Left
							auto LeftBullet = GameObject::Allocate();
							LeftBullet->AttachComponent<Projectile>(&MainPlayerID, entt::entity(0), LeftBullet.get(), *m_meshes[int(PlayerMesh::PLAYERBULLET)]).SetID(LeftBullet->GetID());
							LeftBullet->GetComponent<Projectile>().SetSpeed(500);
							LeftBullet->AttachComponent<EntityType>() = EntityType::PLAYER;
							LeftBullet->GetComponent<Projectile>().SetVelocity(glm::vec3(0, 0, -1));
							glm::vec3 offset2 = glm::vec3(-3, 0, -10);
							LeftBullet->GetComponent<Transform>().MoveLocalPos(offset2);
							LeftBullet->GetComponent<Transform>().SetLocalScale(glm::vec3(3));

							//Reset time to fire
							m_resetTime = true;
						}

						if (m_resetTime) {
							m_startTime = glfwGetTime();
							m_resetTime = false;
						}
					}
				}

				//Left Joystick
				{
					float tempSpeed = 0.0f;

					//ROTATE LEFT
					if (gamepad.axes[Joystick::LEFT].X < -0.17f && gamepad.axes[Joystick::LEFT].X > -0.33) {
						temp = glm::vec3(0.0f, 0.5f, 0.0f);
						playerEnt.RotateLocal(temp);
					}
					else if (gamepad.axes[Joystick::LEFT].X < -0.3 && gamepad.axes[Joystick::LEFT].X > -0.55f) {
						temp = glm::vec3(0.0f, 1.0f, 0.0f);
						playerEnt.RotateLocal(temp);
					}
					else if (gamepad.axes[Joystick::LEFT].X < -0.77) {
						temp = glm::vec3(0.0f, 1.25f, 0.0f);
						playerEnt.RotateLocal(temp);
					}
					else if (gamepad.axes[Joystick::LEFT].X < -0.99) {
						temp = glm::vec3(0.0f, 2.0f, 0.0f);
						playerEnt.RotateLocal(temp);
					}
					//ROTATE RIGHT
					if (gamepad.axes[Joystick::LEFT].X > 0.17f && gamepad.axes[Joystick::LEFT].X < 0.33) {
						temp = glm::vec3(0.0f, -0.5f, 0.0f);
						playerEnt.RotateLocal(temp);
					}
					else if (gamepad.axes[Joystick::LEFT].X > 0.3 && gamepad.axes[Joystick::LEFT].X < 0.55f) {
						temp = glm::vec3(0.0f, -1.0f, 0.0f);
						playerEnt.RotateLocal(temp);
					}
					else if (gamepad.axes[Joystick::LEFT].X > 0.77) {
						temp = glm::vec3(0.0f, -1.25f, 0.0f);
						playerEnt.RotateLocal(temp);
					}
					else if (gamepad.axes[Joystick::LEFT].X > 0.99) {
						temp = glm::vec3(0.0f, -2.0f, 0.0f);
						playerEnt.RotateLocal(temp);
					}
					//MOVE FORWARD
					if (gamepad.axes[Joystick::LEFT].Y > -0.17) {
						if (isWingOpen && !playerController->getAnimate())
						{
							playerController->SetAnimate(true);
							isPlayerAnim = true;
							isWingOpen = false;
						}
					}
					else if (gamepad.axes[Joystick::LEFT].Y < -0.17 && gamepad.axes[Joystick::LEFT].Y > -0.25) {
						temp = glm::vec3(0, 0, -0.8);
						playerEnt.MoveLocalPos(temp);
						tempSpeed = 0.8f;
					}
					else if (gamepad.axes[Joystick::LEFT].Y < -0.25 && gamepad.axes[Joystick::LEFT].Y > -0.45) {
						temp = glm::vec3(0, 0, -1.6);
						playerEnt.MoveLocalPos(temp);
						tempSpeed = 1.6f;
					}
					else if (gamepad.axes[Joystick::LEFT].Y < -0.45 && gamepad.axes[Joystick::LEFT].Y > -0.65) {
						temp = glm::vec3(0, 0, -2.4);
						playerEnt.MoveLocalPos(temp);
						tempSpeed = 2.4f;
					}
					else if (gamepad.axes[Joystick::LEFT].Y < -0.65 && gamepad.axes[Joystick::LEFT].Y > -0.85) {
						temp = glm::vec3(0, 0, -3.2);
						playerEnt.MoveLocalPos(temp);
						tempSpeed = 3.2f;
					}
					else if (gamepad.axes[Joystick::LEFT].Y < -0.85) {
						temp = glm::vec3(0, 0, -4.0);
						playerEnt.MoveLocalPos(temp);
						tempSpeed = 4.0f;

						if (!isWingOpen && !playerController->getAnimate())
						{
							playerController->SetAnimate(true);
							isPlayerAnim = true;
							isWingOpen = true;
						}
					}
					 

					particles[ParticleName::PLAYER_CENTER_LEFT]->getEmitter()->setSpeed(tempSpeed);
					particles[ParticleName::PLAYER_CENTER_RIGHT]->getEmitter()->setSpeed(tempSpeed);
				}
			}
			else if(isDodge)
			{
				if (dodgeCount == 0.5)
				{
					particleTemp = new ParticleController(4, playerEnt.GetLocalPos(), m_textures[int(TextureType::BLINK)], MainPlayerID);
					particleTemp->setSize(5);
					particleTemp->getEmitter()->setLifetime(1.0, 1.0);
					particleTemp->getEmitter()->setSpeed(10);
					particleTemp->getEmitter()->init();
					particleTemp->setModelMatrix(playerEnt.UpdateGlobal());
					particles.push_back(particleTemp);

				}

				dodgeCount -= m_deltaTime;
				temp = glm::vec3(10 * tempDir, 0, 0);
				playerEnt.MoveLocalPos(temp);

				
			
				
			}
			if (dodgeCount < 0)
			{
				dodgeCount = 0.5;
				isDodge = false;
			}

		}
	}
	//else
	//	std::cout << "No controller connected" << std::endl;
}

void Universe::SolarSystemUpdate() {
	if (m_name == "Universe_19") {
		// Rotation
		GameObject::GetComponent<Transform>(m_solarSystem[Universe19SS::SOLARI]).RotateLocal(sunRotation);
		GameObject::GetComponent<Transform>(m_solarSystem[Universe19SS::VERASTEN]).RotateLocal(planetRotation);
		GameObject::GetComponent<Transform>(m_solarSystem[Universe19SS::YECHIN]).RotateLocal(planetRotation);
		GameObject::GetComponent<Transform>(m_solarSystem[Universe19SS::KERANTIA]).RotateLocal(planetRotation);
		GameObject::GetComponent<Transform>(m_solarSystem[Universe19SS::GUERISTIS]).RotateLocal(planetRotation);
		GameObject::GetComponent<Transform>(m_solarSystem[Universe19SS::KEMINTH]).RotateLocal(planetRotation);

		// Orbit
		GameObject::GetComponent<Transform>(m_solarSystem[Universe19SS::HPC]).RotateLocal(moonOrbit);
		GameObject::GetComponent<Transform>(m_solarSystem[Universe19SS::SVC]).RotateLocal(verastenOrbit);
		GameObject::GetComponent<Transform>(m_solarSystem[Universe19SS::SYC]).RotateLocal(yechinOrbit);
		GameObject::GetComponent<Transform>(m_solarSystem[Universe19SS::SKRC]).RotateLocal(kerantiaOrbit);
		GameObject::GetComponent<Transform>(m_solarSystem[Universe19SS::SGC]).RotateLocal(gueristisOrbit); 
		GameObject::GetComponent<Transform>(m_solarSystem[Universe19SS::SKEC]).RotateLocal(keminthOrbit);

		GameObject::GetComponent<Transform>(AsteroidBeltID).RotateLocal(asteroidOrbit);
		GameObject::GetComponent<Transform>(CometID).MoveLocalPos(cometVelocity);
		
	}
	else if (m_name == "Universe_27") {

	}
}

bool Universe::isBoxCollide(Transform Obj1, Transform Obj2)
{
	//X axis collision
	bool collisionX = Obj1.GetLocalPos().x + Obj1.GetWHD().x >= Obj2.GetLocalPos().x &&
		Obj2.GetLocalPos().x + Obj2.GetWHD().x >= Obj1.GetLocalPos().x;

	//Y axis collision
	bool collisionY = Obj1.GetLocalPos().y + Obj1.GetWHD().y >= Obj2.GetLocalPos().y &&
		Obj2.GetLocalPos().y + Obj2.GetWHD().y >= Obj1.GetLocalPos().y;

	//Z axis collision
	bool collisionZ = Obj1.GetLocalPos().z + Obj1.GetWHD().z >= Obj2.GetLocalPos().z &&
		Obj2.GetLocalPos().z + Obj2.GetWHD().z >= Obj1.GetLocalPos().z;

	// collision only if on all axis
	return collisionX && collisionY && collisionZ;
}

bool Universe::isBoxCircleCollide(Transform objBox, Transform objCircle)
{
	bool collisionX = objBox.GetLocalPos().x + objBox.GetWHD().x >= objCircle.GetLocalPos().x 
		&& objCircle.GetLocalPos().x + objCircle.GetRadius()>=objBox.GetLocalPos().x;

	bool collisionZ = objBox.GetLocalPos().z + objBox.GetWHD().z >= objCircle.GetLocalPos().z
		&& objCircle.GetLocalPos().z + objCircle.GetRadius() >= objBox.GetLocalPos().z;

	return collisionX && collisionZ;
}
