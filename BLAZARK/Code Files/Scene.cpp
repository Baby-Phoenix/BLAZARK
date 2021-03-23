#include "Scene.h"


enum class EntityType { PLAYER, ENEMY };

enum class TextureType { START = 3 , RESUME, CONTROLS, EXIT, BACKGROUND, CONTROLSMENU, SCORE, SCORENUM};

enum class PlayerMesh { PLAYERSHIPXWINGS, PLAYERSHIPXWINGE = 2, PLAYERSHIPPENCIL, PLAYERSHIPBAT, PLAYERBULLET };

enum class EnemyMesh { SCAVENGERU1S = 6, SCAVENGERU1E = 8, NEROTISTU1, JELLYFISHS, JELLYFISHE = 31 };

enum class PlanetMesh { SOLARI = 32, VERASTEN, YECHIN, KERANTIA, LUNARI, GUERISTIS, KEMINTH, 
						LUTERO, DEDMOS, TITANIUS, KREILLO, PAXALLUS, DERANGI, RHETOID, MAGAANTU };

enum Universe19SS { SVC, SYC, SKRC, SGC, SKEC, SOLARI, VERASTEN, YECHIN, KERANTIA, GUERISTIS, KEMINTH, HPC };
enum Universe27SS {};
enum Universe5SS {};

std::vector<Mesh*> Scene::m_meshes;
std::vector<Texture*> Scene::m_textures;

std::unique_ptr<GameObject> effect;

StaticRenderer tempEnemy;

// Solar System Rotation
glm::vec3 sunRotation = glm::vec3(0, -0.025, 0);
glm::vec3 planetRotation = glm::vec3(0, 0.045583, 0);
glm::vec3 moonOrbit = glm::vec3(0, 0.01022, 0);
glm::vec3 verastenOrbit = glm::vec3(0, 0.004787, 0);
glm::vec3 yechinOrbit = glm::vec3(0, 0.003502, 0);
glm::vec3 kerantiaOrbit = glm::vec3(0, 0.002978, 0);
glm::vec3 gueristisOrbit = glm::vec3(0, 0.0024077, 0);
glm::vec3 keminthOrbit = glm::vec3(0, 0.000543, 0);

// Key Toggles
bool texTglPressed = false;

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

		for (int i = 1; i <= 26; i++) {
			m_meshes.push_back(new Mesh());
			loadOBJ(("Resource Files/OBJFiles/Universe-19/EnemyShips/Morph/Boss/jellyfishBoss_" + std::to_string(i) + ".obj").c_str(), *m_meshes[i + int(EnemyMesh::NEROTISTU1)]);
		}

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

				m_StartOrResume[0]->AttachComponent<Sprite2D>(m_textures[int(TextureType::RESUME)], m_StartOrResume[0]->GetID(), 20, 10, false, tempAnim);
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


				m_StartOrResume[1]->AttachComponent<Sprite2D>(m_textures[int(TextureType::CONTROLS)], m_StartOrResume[1]->GetID(), 30, 10, true, tempAnim);
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


				m_StartOrResume[2]->AttachComponent<Sprite2D>(m_textures[int(TextureType::EXIT)], m_StartOrResume[2]->GetID(), 20, 10, true, tempAnim);
				m_StartOrResume[2]->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, -70, 10));
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

	//m_scoreTime -= deltaTime;

	//if (m_scoreTime <= 0) {
	//	m_score->GetComponent<ScoreHandler>().Add(20);
	//	m_scoreTime = 0.1f;
	//}

	// Key Input
	KeyInput();

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

	else
		std::cout << "No controller connected" << std::endl;

	

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
		particleTemp = new ParticleController(1, glm::vec3(playerPos.x - 0.6, playerPos.y - 0.0, playerPos.z + 2.2f));		
		//particleTemp->setRotation(glm::vec3(0, 180, 0));
		particleTemp->getEmitter()->setRadius(0.3);
		particleTemp->getEmitter()->setLifetime(0.1f, 1.5f);
		particleTemp->getEmitter()->setSpeed(2);	  
		particles.push_back(particleTemp);
		//Center Right - 1
		particleTemp = new ParticleController(1, glm::vec3(playerPos.x + 0.6, playerPos.y - 0.0, playerPos.z + 2.2f));
		//particleTemp->setRotation(glm::vec3(0, 180, 0));
		particleTemp->getEmitter()->setRadius(0.3);
		particleTemp->getEmitter()->setLifetime(0.1f, 1.5f);
		particleTemp->getEmitter()->setSpeed(2);
		particles.push_back(particleTemp);
			
		//HUD
		auto health = GameObject::Allocate();
		auto* tempAnim = &health->AttachComponent<AnimationHandler>();
		health->GetComponent<AnimationHandler>();
		auto& anim = health->GetComponent<AnimationHandler>();
		anim.InitUVS(m_textures[0]);
		Animation2D Oneclip;
		/*Oneclip.AddFrame(UVS(glm::vec2(706, 233), glm::vec2(939, 1)));
		Oneclip.AddFrame(UVS(glm::vec2(471, 233), glm::vec2(705, 1)));
		Oneclip.AddFrame(UVS(glm::vec2(236, 233), glm::vec2(470, 1)));
		Oneclip.AddFrame(UVS(glm::vec2(1, 233), glm::vec2(235, 1)));*/
		Oneclip.AddFrame(UVS(glm::vec2(1, 235), glm::vec2(235, 1)));
		Oneclip.AddFrame(UVS(glm::vec2(236, 235), glm::vec2(470, 1)));
		Oneclip.AddFrame(UVS(glm::vec2(471, 235), glm::vec2(705, 1)));
		Oneclip.AddFrame(UVS(glm::vec2(706, 235), glm::vec2(939, 1)));

		Oneclip.SetIsRepeating(true);
		Oneclip.SetSecPerFrame(1.0);
		anim.AddAnimation(Oneclip);
		anim.SetActiveAnim(0);

		health->AttachComponent<Sprite2D>(m_textures[0], health->GetID(), 15, 15, false, tempAnim);
		health->AttachComponent<Transform>().SetLocalPos(glm::vec3(-80, -80, -10));

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

			//testing for ai
			auto enemy = GameObject::Allocate();
			entt::entity enemyID = enemy->GetID();
			enemy->AttachComponent<Transform>();
			enemy->AttachComponent<BasicAI>(enemyID, sunEntity->GetID(), playerEntity->GetID());
			enemy->AttachComponent<EntityType>() = EntityType::ENEMY;
			enemy->AttachComponent<StaticRenderer>(cameraEntity->GetID(), enemy->GetID(), *m_meshes[int(EnemyMesh::NEROTISTU1)], nullptr);
			enemy->GetComponent<Transform>().SetWHD(glm::vec3(m_meshes[int(EnemyMesh::NEROTISTU1)]->GetWidth(), m_meshes[int(EnemyMesh::NEROTISTU1)]->GetHeight(), m_meshes[int(EnemyMesh::NEROTISTU1)]->GetDepth()));

			// Jellyfish
			auto jellyfishEntity = GameObject::Allocate();
			BossID = jellyfishEntity->GetID();
			jellyfishEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(700, -100, 0));
			jellyfishEntity->AttachComponent<DynamicRenderer>(cameraEntity->GetID(), BossID, *m_meshes[int(EnemyMesh::JELLYFISHS)], nullptr);
			jellyfishEntity->AttachComponent<EntityType>() = EntityType::ENEMY;
			jellyfishEntity->GetComponent<Transform>().SetLocalScale(glm::vec3(10.0));
			jellyfishEntity->GetComponent<Transform>().SetWHD(glm::vec3(m_meshes[int(EnemyMesh::JELLYFISHS)]->GetWidth(), m_meshes[int(EnemyMesh::JELLYFISHS)]->GetHeight(), m_meshes[int(EnemyMesh::JELLYFISHS)]->GetDepth()));

			jellyfishController = &jellyfishEntity->AttachComponent<MorphAnimController>(int(BossID));
			jellyfishController->SetFrames(m_meshes, int(EnemyMesh::JELLYFISHS), int(EnemyMesh::JELLYFISHE), false);

			// Verasten
			auto lavaPlanetEntity = GameObject::Allocate();
			m_solarSystem.push_back(lavaPlanetEntity->GetID());
			lavaPlanetEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 750));
			lavaPlanetEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), lavaPlanetEntity->GetID(), *m_meshes[int(PlanetMesh::VERASTEN)], nullptr);

			// Yechin
			auto desertPlanetEntity = GameObject::Allocate();
			m_solarSystem.push_back(desertPlanetEntity->GetID());
			desertPlanetEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(1500, 0, 0));
			desertPlanetEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), desertPlanetEntity->GetID(), *m_meshes[int(PlanetMesh::YECHIN)], nullptr);

			// Kerantia
			auto homePlanetEntity = GameObject::Allocate();
			m_solarSystem.push_back(homePlanetEntity->GetID());
			homePlanetEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, -2250));
			homePlanetEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), homePlanetEntity->GetID(), *m_meshes[int(PlanetMesh::KERANTIA)], nullptr);

			// Lunari
			auto moonEntity = GameObject::Allocate();
			moonEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 250));
			moonEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), moonEntity->GetID(), *m_meshes[int(PlanetMesh::LUNARI)], nullptr);

			// Gueristis
			auto rockPlanetEntity = GameObject::Allocate();
			m_solarSystem.push_back(rockPlanetEntity->GetID());
			rockPlanetEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(-2875, 0, 0));
			rockPlanetEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), rockPlanetEntity->GetID(), *m_meshes[int(PlanetMesh::GUERISTIS)], nullptr);

			// Keminth
			auto icePlanetEntity = GameObject::Allocate();
			m_solarSystem.push_back(icePlanetEntity->GetID());
			icePlanetEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 3500));
			icePlanetEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), icePlanetEntity->GetID(), *m_meshes[int(PlanetMesh::KEMINTH)], nullptr);

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
			HPCEntity->GetComponent<Transform>().SetParent(&m_solarSystem[Universe19SS::SKRC]);
		}
		else if (m_name == "Universe_27") {

			m_SceneResumeNo = int(ScenesNum::UNIVERSE_27);
			// Lutero
			auto sunEntity = GameObject::Allocate();
			sunEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 0));
			sunEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), sunEntity->GetID(), *m_meshes[int(PlanetMesh::LUTERO)], nullptr, true);
			sunEntity->GetComponent<Transform>().SetRadius(3 * (m_meshes[int(PlanetMesh::LUTERO)]->GetWidth() / 2));
			sunEntity->GetComponent<Transform>().SetLocalScale(glm::vec3(3.0));

			// Dedmos
			auto nullPlanetOneEntity = GameObject::Allocate();
			nullPlanetOneEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 750));
			nullPlanetOneEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), nullPlanetOneEntity->GetID(), *m_meshes[int(PlanetMesh::DEDMOS)], nullptr);

			// Titanius
			auto nullPlanetTwoEntity = GameObject::Allocate();
			nullPlanetTwoEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(1500, 0, 0));
			nullPlanetTwoEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), nullPlanetTwoEntity->GetID(), *m_meshes[int(PlanetMesh::TITANIUS)], nullptr);

			// Kreillo
			auto nullPlanetThreeEntity = GameObject::Allocate();
			nullPlanetThreeEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, -2250));
			nullPlanetThreeEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), nullPlanetThreeEntity->GetID(), *m_meshes[int(PlanetMesh::KREILLO)], nullptr);

			// Paxallus
			auto citadelEntity = GameObject::Allocate();
			citadelEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(-2875, 0, 0));
			citadelEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), citadelEntity->GetID(), *m_meshes[int(PlanetMesh::PAXALLUS)], nullptr);

			// Derangi
			auto mushroomPlanetEntity = GameObject::Allocate();
			mushroomPlanetEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 3500));
			mushroomPlanetEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), mushroomPlanetEntity->GetID(), *m_meshes[int(PlanetMesh::DERANGI)], nullptr);

			// Rhetoid
			auto gasPlanetOneEntity = GameObject::Allocate();
			gasPlanetOneEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(4250, 0, 0));
			gasPlanetOneEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), gasPlanetOneEntity->GetID(), *m_meshes[int(PlanetMesh::RHETOID)], nullptr);

			// Magaantu
			auto gasPlanetTwoEntity = GameObject::Allocate();
			gasPlanetTwoEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, -5000));
			gasPlanetTwoEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), gasPlanetTwoEntity->GetID(), *m_meshes[int(PlanetMesh::MAGAANTU)], nullptr);
		}
		else if (m_name == "Universe_5") {
			m_SceneResumeNo = int(ScenesNum::UNIVERSE_5);
		}

		//Setting Parent/Childe
		cameraEntity->GetComponent<Transform>().SetParent(new entt::entity(playerEntity->GetID()));
		health->GetComponent<Transform>().SetParent(camentity);
		abilities->GetComponent<Transform>().SetParent(camentity);
		powerUp->GetComponent<Transform>().SetParent(camentity);
		score->GetComponent<Transform>().SetParent(camentity);

		Skybox::Init();
	}
}

void Universe::Update(float deltaTime)
{
	// Key Input
	KeyInput();
	GamepadInput();

	// Camera Update 
	camera->Update();

	playerController->Update(deltaTime);
	jellyfishController->Update(deltaTime);
	
	m_sceneReg->view<BasicAI>().each([=](BasicAI& ai) {	ai.Update(deltaTime); });

	m_sceneReg->view<AnimationHandler>().each([=](AnimationHandler& anim) { anim.Update(deltaTime); });
	
	// Solar System Rotation (IN-PROGRESS) //
	SolarSystemUpdate();

	// Transform Update
	m_sceneReg->view<Transform>().each([=](Transform& transform) { transform.UpdateGlobal(); });

	//Particle
	for (auto& i : this->particles)
		i->update(deltaTime, camera->GetProj(), camera->GetView(), GameObject::GetComponent<Transform>(MainPlayerID).UpdateGlobal());

	for (auto Bulletentity : m_sceneReg->view<Projectile>()) {
		
		for (auto entity : m_sceneReg->view<StaticRenderer, EntityType>()) {
			if (isCollide(GameObject::GetComponent<Transform>(Bulletentity), GameObject::GetComponent<Transform>(entity)) && GameObject::GetComponent<EntityType>(entity) == EntityType::ENEMY)
			{
				m_sceneReg->destroy(entity);
				m_sceneReg->destroy(Bulletentity);
				m_score->GetComponent<ScoreHandler>().Add(1);
			}
		}

		GameObject::GetComponent<Projectile>(Bulletentity).Update(deltaTime);
	}
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

	if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		glm::vec3 temp = glm::vec3(0, 0, -2);
		playerEnt.MoveLocalPos(temp);
	}
	else if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		glm::vec3 temp = glm::vec3(0, 0, 2);
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
		glm::vec3 temp = glm::vec3(0, 0, -0.5);
		camEnt.MoveLocalPos(temp);
	}
	else if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
	{
		glm::vec3 temp = glm::vec3(0, 0, 0.5);
		camEnt.MoveLocalPos(temp);
	}
	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
	{
		glm::vec3 temp = glm::vec3(0.0f, 0.5f, 0.0f);
		camEnt.RotateLocal(temp);
	}
	else if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
	{
		glm::vec3 temp = glm::vec3(0.0f, -0.5f, 0.0f);
		camEnt.RotateLocal(temp);
	}
	if (glfwGetKey(m_window, GLFW_KEY_Z) == GLFW_PRESS)
	{
		glm::vec3 temp = glm::vec3(0.0f, 0.5f, 0.0f);
		camEnt.MoveLocalPos(temp);
	}
	else if (glfwGetKey(m_window, GLFW_KEY_C) == GLFW_PRESS)
	{
		glm::vec3 temp = glm::vec3(0.0f, -0.5f, 0.0f);
		camEnt.MoveLocalPos(temp);
	}
	if (glfwGetKey(m_window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		glm::vec3 temp = glm::vec3(-0.5f, 0.0f, 0.0f);
		camEnt.MoveLocalPos(temp);
	}
	else if (glfwGetKey(m_window, GLFW_KEY_E) == GLFW_PRESS)
	{
		glm::vec3 temp = glm::vec3(0.5f, 0.0f, 0.0f);
		camEnt.MoveLocalPos(temp);
	}

	if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		
		if (glfwGetTime() - m_startTime >= m_fireRate) {
			playerController->SetAnimate(true);
			playerController->SetReverse();
			// Shoot Bullet Right
			auto RightBullet = GameObject::Allocate();
			RightBullet->AttachComponent<Projectile>(&MainPlayerID, entt::entity(0), RightBullet.get(), *m_meshes[int(PlayerMesh::PLAYERBULLET)]).SetID(RightBullet->GetID());
			RightBullet->GetComponent<Projectile>().SetSpeed(2000);
			RightBullet->GetComponent<Projectile>().SetVelocity(glm::vec3(0, 0, -1));
			glm::vec3 offset1 = glm::vec3(3, 0, -10);
			RightBullet->GetComponent<Transform>().MoveLocalPos(offset1);
			RightBullet->GetComponent<Transform>().SetLocalScale(glm::vec3(3));

            // Shoot Bullet Left
			auto LeftBullet = GameObject::Allocate();
			LeftBullet->AttachComponent<Projectile>(&MainPlayerID, entt::entity(0), LeftBullet.get(), *m_meshes[int(PlayerMesh::PLAYERBULLET)]).SetID(LeftBullet->GetID());
			LeftBullet->GetComponent<Projectile>().SetSpeed(2000);
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
	float temp;

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

			}
			else if (gamepad.buttons.Y) {

			}

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
				else if (gamepad.trigger.LT > 0.8 ) {
					temp = glm::vec3(0, 0, -2);
					playerEnt.MoveLocalPos(temp);
				}		
			}

			//Right Trigger
			{
				if (glfwGetTime() - m_startTime >= m_fireRate) {
					if (gamepad.trigger.RT > -0.8) {
						// Shoot Bullet Right
						auto RightBullet = GameObject::Allocate();
						RightBullet->AttachComponent<Projectile>(&MainPlayerID, entt::entity(0), RightBullet.get(), *m_meshes[int(PlayerMesh::PLAYERBULLET)]).SetID(RightBullet->GetID());
						RightBullet->GetComponent<Projectile>().SetSpeed(2000);
						RightBullet->GetComponent<Projectile>().SetVelocity(glm::vec3(0, 0, -1));
						glm::vec3 offset1 = glm::vec3(3, 0, -10);
						RightBullet->GetComponent<Transform>().MoveLocalPos(offset1);
						RightBullet->GetComponent<Transform>().SetLocalScale(glm::vec3(3));

						// Shoot Bullet Left
						auto LeftBullet = GameObject::Allocate();
						LeftBullet->AttachComponent<Projectile>(&MainPlayerID, entt::entity(0), LeftBullet.get(), *m_meshes[int(PlayerMesh::PLAYERBULLET)]).SetID(LeftBullet->GetID());
						LeftBullet->GetComponent<Projectile>().SetSpeed(2000);
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
					temp = glm::vec3(0.0f, 0.25f, 0.0f);
					playerEnt.RotateLocal(temp);
				}
				else if (gamepad.axes[Joystick::LEFT].X < -0.3 && gamepad.axes[Joystick::LEFT].X > -0.55f) {
					temp = glm::vec3(0.0f, 0.5f, 0.0f);
					playerEnt.RotateLocal(temp);
				}
				else if (gamepad.axes[Joystick::LEFT].X < -0.77) {
					temp = glm::vec3(0.0f, 0.75f, 0.0f);
					playerEnt.RotateLocal(temp);
				}
				else if (gamepad.axes[Joystick::LEFT].X < -0.99) {
					temp = glm::vec3(0.0f, 1.0f, 0.0f);
					playerEnt.RotateLocal(temp);
				}
				//ROTATE RIGHT
				if (gamepad.axes[Joystick::LEFT].X > 0.17f && gamepad.axes[Joystick::LEFT].X < 0.33) {
					temp = glm::vec3(0.0f, -0.25f, 0.0f);
					playerEnt.RotateLocal(temp);
				}
				else if (gamepad.axes[Joystick::LEFT].X > 0.3 && gamepad.axes[Joystick::LEFT].X < 0.55f) {
					temp = glm::vec3(0.0f, -0.5f, 0.0f);
					playerEnt.RotateLocal(temp);
				}
				else if (gamepad.axes[Joystick::LEFT].X > 0.77) {
					temp = glm::vec3(0.0f, -0.75f, 0.0f);
					playerEnt.RotateLocal(temp);
				}
				else if (gamepad.axes[Joystick::LEFT].X > 0.99) {
					temp = glm::vec3(0.0f, -1.0f, 0.0f);
					playerEnt.RotateLocal(temp);
				}
				//MOVE FORWARD
				if (gamepad.axes[Joystick::LEFT].Y < -0.17 && gamepad.axes[Joystick::LEFT].Y > -0.25) {
					temp = glm::vec3(0, 0, -0.4);
					playerEnt.MoveLocalPos(temp);
					tempSpeed = 0.8f;
				}
				else if (gamepad.axes[Joystick::LEFT].Y < -0.25 && gamepad.axes[Joystick::LEFT].Y > -0.45) {
					temp = glm::vec3(0, 0, -0.8);
					playerEnt.MoveLocalPos(temp);
					tempSpeed = 1.6f;
				}
				else if (gamepad.axes[Joystick::LEFT].Y < -0.45 && gamepad.axes[Joystick::LEFT].Y > -0.65) {
					temp = glm::vec3(0, 0, -1.2);
					playerEnt.MoveLocalPos(temp);
					tempSpeed = 2.4f;
				}
				else if (gamepad.axes[Joystick::LEFT].Y < -0.65 && gamepad.axes[Joystick::LEFT].Y > -0.85) {
					temp = glm::vec3(0, 0, -1.6);
					playerEnt.MoveLocalPos(temp);
					tempSpeed = 3.2f;
				}
				else if (gamepad.axes[Joystick::LEFT].Y < -0.85) {
					temp = glm::vec3(0, 0, -2.0);
					playerEnt.MoveLocalPos(temp);
					tempSpeed = 4.0f;
				}

				particles[ParticleName::PLAYER_CENTER_LEFT]->getEmitter()->setSpeed(tempSpeed);
				particles[ParticleName::PLAYER_CENTER_RIGHT]->getEmitter()->setSpeed(tempSpeed);
			}


		}
	}
	else
		std::cout << "No controller connected" << std::endl;
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
	}
	else if (m_name == "Universe_27") {

	}
}

bool Universe::isCollide(Transform Obj1, Transform Obj2)
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
