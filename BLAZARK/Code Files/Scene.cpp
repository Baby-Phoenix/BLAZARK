#include "Scene.h"


int Universe::m_PlayerHealth = 0;
entt::entity Universe::health = entt::entity(0);

enum class TextureType { START = 3, RESUME, CONTROLS, EXIT, BACKGROUND, CONTROLSMENU, GAMEOVER, WIN, SCORE, SCORENUM, COMET, FIRE, BLINK, YELLOW, ORANGE, SKY27, SKY5 };

enum class PlayerMesh { PLAYERSHIPXWINGS, PLAYERSHIPXWINGE = 2, PLAYERSHIPPENCIL, PLAYERSHIPBAT, PLAYERBULLET };

enum class EnemyMesh { SCAVENGERU1S = 6, SCAVENGERU1E = 8, NEROTISTU1, KAMAKAZI, BOMBARDIER };

enum class PlanetMesh {
	SOLARI = int(EnemyMesh::BOMBARDIER) + 1, VERASTEN, YECHIN, KERANTIA, LUNARI, GUERISTIS, KEMINTH,
	LUTERO, DEDMOS, TITANIUS, KREILLO, PAXALLUS, DERANGI, RHETOID, MAGAANTU,
	RALEI, DARKP, DUALP, PINKP, SPIKEP, WHITEP, COMET, ARROW = COMET + 32
};

enum Universe19SS { SVC, SYC, SKRC, SGC, SKEC, SOLARI, VERASTEN, YECHIN, KERANTIA, GUERISTIS, KEMINTH, KEMINTHRING, HPC };
enum Universe27SS { LDMC, LTC, LKC, LPC, LDRC, LRC, LMC, LUTERO, DEDMOS, TITANIUS, KREILLO, PAXALLUS, DERANGI, RHETOID, MEGAANTU };
enum Universe5SS { RDKC, RDLC, RPC, RSC, RWC, RALEI, DARKP, DUALP, PINKP, SPIKEP, WHITEP };

std::vector<Mesh*> Scene::m_meshes;
std::vector<Texture*> Scene::m_textures;

std::unique_ptr<GameObject> BufferEntity;

float dodgeCount = 0.5;
float tempDir = 1;

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
bool isRotate = false;
bool isSceneSwitch = false;
bool isFMODINIT = false;

// Pixelation Transition
bool isTransitionActive = false;
bool wasTransitionActive = false;
bool wasSceneSwitched = false;
float intensity = 0.0;


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
		m_textures.push_back(new Texture("Resource Files/Textures/Menu/Game_Over_Screen.png"));
		m_textures.push_back(new Texture("Resource Files/Textures/Menu/Win_Screen.png"));

		m_textures.push_back(new Texture("Resource Files/Textures/HUD/SCORE_LABEL.png"));
		m_textures.push_back(new Texture("Resource Files/Textures/HUD/ScoreAnim.png"));

		m_textures.push_back(new Texture("Resource Files/Textures/CometTexture2.png"));
		m_textures.push_back(new Texture("Resource Files/Textures/red.png"));
		m_textures.push_back(new Texture("Resource Files/Textures/blink.png"));
		m_textures.push_back(new Texture("Resource Files/Textures/yellow.png"));
		m_textures.push_back(new Texture("Resource Files/Textures/orange.png"));
		m_textures.push_back(new Texture("Resource Files/Textures/sky27.png"));
		m_textures.push_back(new Texture("Resource Files/Textures/sky5.png"));

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
		// Universe-5 //
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Universe-5/Planets/blacksun.obj", *m_meshes[int(PlanetMesh::RALEI)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Universe-5/Planets/darkPlanet.obj", *m_meshes[int(PlanetMesh::DARKP)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Universe-5/Planets/dualPlanet.obj", *m_meshes[int(PlanetMesh::DUALP)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Universe-5/Planets/pinkPlanet.obj", *m_meshes[int(PlanetMesh::PINKP)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Universe-5/Planets/spikePlanet.obj", *m_meshes[int(PlanetMesh::SPIKEP)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Universe-5/Planets/whitePlanet.obj", *m_meshes[int(PlanetMesh::WHITEP)]);

		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Misc/Comet2.obj", *m_meshes[int(PlanetMesh::COMET)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Misc/Debris/Barrel.obj", *m_meshes[int(PlanetMesh::COMET) + 1]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Misc/Debris/Bottle.obj", *m_meshes[int(PlanetMesh::COMET) + 2]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Misc/Debris/BoxContainer.obj", *m_meshes[int(PlanetMesh::COMET) + 3]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Misc/Asteroids/19/Asteroids_RING.obj", *m_meshes[int(PlanetMesh::COMET) + 4]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Misc/Asteroids/19/Asteroids_Cluster_1.obj", *m_meshes[int(PlanetMesh::COMET) + 5]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Misc/Asteroids/19/Asteroids_Cluster_2.obj", *m_meshes[int(PlanetMesh::COMET) + 6]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Misc/Asteroids/19/Asteroids_Cluster_3.obj", *m_meshes[int(PlanetMesh::COMET) + 7]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Misc/Asteroids/19/Asteroids_Cluster_4.obj", *m_meshes[int(PlanetMesh::COMET) + 8]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Misc/Asteroids/19/Asteroids_Cluster_5.obj", *m_meshes[int(PlanetMesh::COMET) + 9]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Misc/Asteroids/19/Asteroids_Cluster_6.obj", *m_meshes[int(PlanetMesh::COMET) + 10]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Misc/Asteroids/19/Asteroids_Cluster_7.obj", *m_meshes[int(PlanetMesh::COMET) + 11]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Misc/Asteroids/19/Asteroids_Cluster_8.obj", *m_meshes[int(PlanetMesh::COMET) + 12]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Universe-19/Planets/KeminthRing.obj", *m_meshes[int(PlanetMesh::COMET) + 13]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Misc/Asteroids/27/Asteroids_RING2.obj", *m_meshes[int(PlanetMesh::COMET) + 14]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Misc/Asteroids/27/Asteroids_Cluster_1G.obj", *m_meshes[int(PlanetMesh::COMET) + 15]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Misc/Asteroids/27/Asteroids_Cluster_2G.obj", *m_meshes[int(PlanetMesh::COMET) + 16]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Misc/Asteroids/27/Asteroids_Cluster_3G.obj", *m_meshes[int(PlanetMesh::COMET) + 17]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Misc/Asteroids/27/Asteroids_Cluster_4G.obj", *m_meshes[int(PlanetMesh::COMET) + 18]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Misc/Asteroids/27/Asteroids_Cluster_5G.obj", *m_meshes[int(PlanetMesh::COMET) + 19]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Misc/Asteroids/27/Asteroids_Cluster_6G.obj", *m_meshes[int(PlanetMesh::COMET) + 20]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Misc/Asteroids/27/Asteroids_Cluster_7G.obj", *m_meshes[int(PlanetMesh::COMET) + 21]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Misc/Asteroids/27/Asteroids_Cluster_8G.obj", *m_meshes[int(PlanetMesh::COMET) + 22]);

		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Misc/Asteroids/5/Asteroids_RING3.obj", *m_meshes[int(PlanetMesh::COMET) + 23]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Misc/Asteroids/5/Asteroids_Cluster_1R.obj", *m_meshes[int(PlanetMesh::COMET) + 24]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Misc/Asteroids/5/Asteroids_Cluster_2R.obj", *m_meshes[int(PlanetMesh::COMET) + 25]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Misc/Asteroids/5/Asteroids_Cluster_3R.obj", *m_meshes[int(PlanetMesh::COMET) + 26]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Misc/Asteroids/5/Asteroids_Cluster_4R.obj", *m_meshes[int(PlanetMesh::COMET) + 27]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Misc/Asteroids/5/Asteroids_Cluster_5R.obj", *m_meshes[int(PlanetMesh::COMET) + 28]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Misc/Asteroids/5/Asteroids_Cluster_6R.obj", *m_meshes[int(PlanetMesh::COMET) + 29]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Misc/Asteroids/5/Asteroids_Cluster_7R.obj", *m_meshes[int(PlanetMesh::COMET) + 30]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Misc/Asteroids/5/Asteroids_Cluster_8R.obj", *m_meshes[int(PlanetMesh::COMET) + 31]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Misc/Arrow.obj", *m_meshes[int(PlanetMesh::ARROW)]);
	}
}

std::string Scene::GetName()
{
	return m_name;
}

void Scene::InitScene(int Prescore)
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

int Scene::GetSceneScore()
{
	return 0;
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

void Scene::InitFMOD()
{
	//Setup FMOD
	AudioEngine& engine = AudioEngine::Instance();
	engine.Init();
	//Bank
	engine.LoadBank("Master");

	//Bus
	engine.LoadBus("Music", "{64e20265-cc94-4556-8628-c67fb15f5402}");
	engine.LoadBus("BGM", "{b6953820-8992-4a3e-b540-c97560b8bb5f}");


	//Create event
	AudioEvent& menuMusic = engine.CreateEvent("Main Menu", "{4c283484-756b-493b-95e2-731eec9e557f}");
	AudioEvent& bgmMusic = engine.CreateEvent("BGM", "{f5fb466c-7e26-4fe4-9371-b846dc746337}");

	//Get reference to bus
	AudioBus& musicBus = engine.GetBus("Music");
	musicBus.SetVolume(0.1);

	//PLay music event
	menuMusic.SetParameter("Exit", 1.0f);
	menuMusic.Play();
}

void Scene::UpdateFMOD()
{
}

Menu::Menu(std::string name, unsigned int* num, bool* change)
	:Scene(name)
{
	SceneNo = num;
	switchIt = change;
}

void Menu::InitScene(int Prescore)
{

	//creating a new registry for the scene when initialised
	if (m_sceneReg == nullptr)
		m_sceneReg = new entt::registry();

	//Giving the ECS the same registry as the current scene
	GameObject::SetRegistry(m_sceneReg);

	if (GameObject::IsEmpty()) {

		auto cameraEntity = GameObject::Allocate();
		cameraEntity->AttachComponent<Transform>();
		entt::entity* camentity = new entt::entity(cameraEntity->GetID());
		camera = &cameraEntity->AttachComponent<Camera>(int(cameraEntity->GetID()));
		cameraEntity->GetComponent<Transform>().SetLocalPos(glm::vec3(0.0f, 0.0f, 100.0f));

		if (m_name == "Start_Screen") {
			auto titleScreen = GameObject::Allocate();
			titleScreen->AttachComponent<Sprite2D>(m_textures[int(TextureType::START)], titleScreen->GetID(), 100, 100);
			titleScreen->AttachComponent<Transform>();

			//Setup FMOD
			AudioEngine& engine = AudioEngine::Instance();
			engine.Init();
			engine.LoadBank("Master");
			engine.LoadBus("Music", "{64e20265-cc94-4556-8628-c67fb15f5402}");
			engine.LoadBus("BGM", "{b6953820-8992-4a3e-b540-c97560b8bb5f}");

			//Create event
			AudioEvent& menuMusic = engine.CreateEvent("Main Menu", "{4c283484-756b-493b-95e2-731eec9e557f}");
			AudioEvent& bgmMusic = engine.CreateEvent("BGM", "{f5fb466c-7e26-4fe4-9371-b846dc746337}");

			//Get reference to bus
			AudioBus& musicBus = engine.GetBus("Music");
			musicBus.SetVolume(0.1);

			//PLay music event
			menuMusic.SetParameter("Exit", 1.0f);
			menuMusic.Play();

		}
		else if (m_name == "Game_Over") {
			std::unique_ptr<GameObject> m_score = GameObject::Allocate();
			m_score->AttachComponent<Transform>().SetLocalPos(-15, -27, 10);
			m_score->AttachComponent<ScoreHandler>(m_score->GetComponent<Transform>().GetLocalPos(), m_textures[int(TextureType::SCORENUM)]).Add(Prescore);

			auto loseScreen = GameObject::Allocate();
            loseScreen->AttachComponent<Sprite2D>(m_textures[int(TextureType::GAMEOVER)], loseScreen->GetID(), 100, 100);
            loseScreen->AttachComponent<Transform>().SetLocalPos(0, 0, -5);
		}
		else if (m_name == "Win") {
			std::unique_ptr<GameObject> m_score = GameObject::Allocate();
			m_score->AttachComponent<Transform>().SetLocalPos(-30, -20, 10);
			m_score->AttachComponent<ScoreHandler>(m_score->GetComponent<Transform>().GetLocalPos(), m_textures[int(TextureType::SCORENUM)]).Add(Prescore);

			auto winScreen = GameObject::Allocate();
			winScreen->AttachComponent<Sprite2D>(m_textures[int(TextureType::WIN)], winScreen->GetID(), 100, 100);
			winScreen->AttachComponent<Transform>();

		}
		else if (m_name == "Pause_Menu") {

			// Resume
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

			// Controls
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

			// Exit
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

			// Controls background
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

	AudioEngine& engine = AudioEngine::Instance();
	AudioEvent& musicEvent = engine.GetEvent("Main Menu");

	if (isSceneSwitch)
	{
		musicEvent.SetParameter("Exit", 0);
	}

	engine.Update();
}

unsigned int Menu::GetSceneResumeNumber()
{
	return m_SceneResumeNo;
}

void Menu::SetSceneResumeNo(unsigned int sceneno)
{
	m_SceneResumeNo = sceneno;
}

int Menu::GetSceneScore()
{
	return 0;
}

void Menu::KeyInput()
{
	// Scene Switching //
	/*if (glfwGetKey(m_window, GLFW_KEY_0) == GLFW_PRESS) {
		*switchIt = true;
		*SceneNo = int(ScenesNum::START_SCREEN);
	}*/
	if (m_name == "Start_Screen" && glfwGetKey(m_window, GLFW_KEY_ENTER) == GLFW_PRESS) {
		*switchIt = true;
		*SceneNo = int(ScenesNum::UNIVERSE_19);
		isSceneSwitch = true;
	}
	/*if (glfwGetKey(m_window, GLFW_KEY_2) == GLFW_PRESS) {
		*switchIt = true;
		*SceneNo = int(ScenesNum::UNIVERSE_27);
	}*/


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
	else
	{
		if (m_name == "Game_Over") {
			if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			{
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

void Universe::InitScene(int Prescore)
{
	//creating a new registry for the scene when initialised
	if (m_sceneReg == nullptr)
		m_sceneReg = new entt::registry();

	//Giving the ECS the same registry as the current scene
	GameObject::SetRegistry(m_sceneReg);

	//Setup FMOD
	AudioEngine& engine = AudioEngine::Instance();
	//engine.Init();
	//engine.LoadBank("Master");

	if (!isFMODINIT)
	{
		//Playershooting
		engine.LoadBus("Playershooting", "{91ac6419-101f-49ef-b413-9e849b361ecc}");
		AudioBus& playerShootingBus = engine.GetBus("Playershooting");
		playerShootingBus.SetVolume(0.1);

		//Engine
		engine.LoadBus("Engine", "{6b1d9408-d98a-4a5a-be17-8648864d619a}");
		AudioBus& playerEngineBus = engine.GetBus("Engine");
		playerEngineBus.SetVolume(0.01);

		engine.LoadBus("Blink dodge", "{484066e7-2e58-4e83-9319-dad7fac4f3a9}");
		AudioBus& blinkBus = engine.GetBus("Blink dodge");
		//blinkBus.SetVolume(0.01);

		engine.LoadBus("Centipede", "{ad0a3379-c400-48c0-a55e-db508ea6dde3}");
		AudioBus& centiBus = engine.GetBus("Centipede");
		//centiBus.SetVolume(0.01);

		engine.LoadBus("Enemyshooting", "{d5290cc9-82d0-4f6a-8d04-35131e2696b0}");
		AudioBus& enemyShootingBus = engine.GetBus("Enemyshooting");
		enemyShootingBus.SetVolume(0.5);

		engine.LoadBus("Explosion", "{ffd81674-9e40-4812-ac01-5335642949d5}");
		AudioBus& explosionBus = engine.GetBus("Explosion");
		explosionBus.SetVolume(0.07);

		engine.LoadBus("Low HP", "{b22e3224-a5af-4b04-bd3d-3c80a1791e63}");
		AudioBus& hpBus = engine.GetBus("Low HP");
		//hpBus.SetVolume(0.01);

		engine.LoadBus("Announcement", "{ec2bdc5c-fc91-4bfd-ab42-d2da9a4876d9}");
		AudioBus& announcementBus = engine.GetBus("Announcement");
		//announcementBus.SetVolume(0.01);

		//Create event
		AudioEvent& playershootSFX = engine.CreateEvent("Playershooting", "{355b48d6-9121-4ca3-9ae2-70bf11df03c0}");
		AudioEvent& engineSFX = engine.CreateEvent("Engine", "{5988f7a2-021b-4a3e-a655-ff05f2eb2ee7}");
		AudioEvent& blinkSFX = engine.CreateEvent("Blink Dodge", "{5182038f-722e-4afa-9591-8faa8e8818ff}");
		AudioEvent& centipedeSFX = engine.CreateEvent("Centipede", "{cabe3bb6-0df5-4a0f-8115-4c8082bbd01e}");
		AudioEvent& enemyshootSFX = engine.CreateEvent("Enemy Shooting", "{b0e036c8-617f-46ad-9852-a82c1a8677c1}");
		AudioEvent& explosionSFX = engine.CreateEvent("Explosion", "{708794b1-3ece-4f4b-9a62-6dfbdca75b24}");
		AudioEvent& lowHPSFX = engine.CreateEvent("Low HP", "{a8245ca6-16d7-4d55-a0a7-5e134671eaf3}");

		AudioEvent& jellyannouncementSFX = engine.CreateEvent("JellyFishAnnouncement", "{b8b3e52d-5896-47ef-b992-fdb5ed0d5c17}");
		AudioEvent& centiannouncementSFX = engine.CreateEvent("CentipedeAnnouncement", "{86b9f91a-3896-4529-8d61-2550a8944f93}");
		AudioEvent& hiveannouncementSFX = engine.CreateEvent("HivemindAnnouncement", "{46667b7c-3a8d-43c3-bf42-6137ea78f24c}");
		isFMODINIT = true;
	}
	AudioBus& bgmBus = engine.GetBus("BGM");
	bgmBus.SetVolume(0.7);


	//AudioEvent& bgmMusic = engine.CreateEvent("BGM", "{f5fb466c-7e26-4fe4-9371-b846dc746337}");
	//Get reference to bus
	//AudioBus& musicBus = engine.GetBus("Main Menu");
	//musicBus.SetVolume(0.1);
	//PLay music event
	//menuMusic.Play();

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
		playerEntity->GetComponent<Transform>().SetWHD(glm::vec3(m_meshes[int(PlayerMesh::PLAYERSHIPXWINGS/*EnemyMesh::SCAVENGERU1S*/)]->GetWidth() * 0.5, m_meshes[int(PlayerMesh::PLAYERSHIPXWINGS/*EnemyMesh::SCAVENGERU1S*/)]->GetHeight(), m_meshes[int(PlayerMesh::PLAYERSHIPXWINGS/*EnemyMesh::SCAVENGERU1S*/)]->GetDepth() * 0.5));

		playerEntity->AttachComponent<MorphAnimController>(int(MainPlayerID)).SetFrames(m_meshes, int(PlayerMesh::PLAYERSHIPXWINGS/*EnemyMesh::SCAVENGERU1S*/), int(PlayerMesh::PLAYERSHIPXWINGE/*EnemyMesh::SCAVENGERU1E*/), false);

		//Player Thrusters
		//Left - 0
		glm::vec3 playerPos = GameObject::GetComponent<Transform>(MainPlayerID).GetLocalPos();
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

		glm::vec3 scorePos;
		auto score = GameObject::Allocate();
		score->AttachComponent<Sprite2D>(m_textures[int(TextureType::SCORE)], score->GetID(), 10, 5);
		score->AttachComponent<Transform>().SetLocalPos(-80, 80, -10);

		scorePos = score->GetComponent<Transform>().GetLocalPos();

		m_score = GameObject::Allocate();
		m_score->AttachComponent<ScoreHandler>(scorePos, m_textures[int(TextureType::SCORENUM)], camentity).Add(Prescore);

		// Effects
		BufferEntity = GameObject::Allocate();
		BufferEntity->AttachComponent<PostEffect>().Init(Application::GetWindowWidth(), Application::GetWindowHeight());
		BufferEntity->AttachComponent<PixelationEffect>().Init(Application::GetWindowWidth(), Application::GetWindowHeight());

		AudioEngine& engine = AudioEngine::Instance();
		AudioEvent& musicEvent = engine.GetEvent("BGM");
		AudioEvent& playerEngineEvent = engine.GetEvent("Engine");

		musicEvent.Play();
		playerEngineEvent.Play();

		if (m_name == "Universe_19") {

			JellyFishBoss::Init();

			m_SceneResumeNo = int(ScenesNum::UNIVERSE_19);

			isSceneSwitch = false;



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
			particleTemp = new ParticleController(1, glm::vec3(cometPos.x - 5, cometPos.y, cometPos.z), m_textures[int(TextureType::YELLOW)], m_textures[int(TextureType::FIRE)], CometID);
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
				enemy->AttachComponent<KamikaziAI>(enemy->GetID(), icePlanetEntity->GetID(), playerEntity->GetID()).SetBulletMesh(m_meshes[int(PlayerMesh::PLAYERBULLET)]);;
				enemy->AttachComponent<EntityType>() = EntityType::KAMIKAZI;
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
				enemy->AttachComponent<KamikaziAI>(enemy->GetID(), rockPlanetEntity->GetID(), playerEntity->GetID());
				enemy->AttachComponent<EntityType>() = EntityType::KAMIKAZI;
				enemy->AttachComponent<StaticRenderer>(cameraEntity->GetID(), enemy->GetID(), *m_meshes[int(EnemyMesh::KAMAKAZI)], nullptr);
				enemy->GetComponent<Transform>().SetWHD(glm::vec3(m_meshes[int(EnemyMesh::KAMAKAZI)]->GetWidth(), m_meshes[int(EnemyMesh::KAMAKAZI)]->GetHeight(), m_meshes[int(EnemyMesh::KAMAKAZI)]->GetDepth()));
			}
			//Kamikaze
			for (int i = 0; i < 20; i++) {
				auto enemy = GameObject::Allocate();
				enemy->AttachComponent<Transform>().SetLocalPos(glm::vec3(Random::Range1f(400, 900), 0, Random::Range1f(-800, -100)));
				enemy->AttachComponent<KamikaziAI>(enemy->GetID(), enemy->GetID(), playerEntity->GetID());
				enemy->AttachComponent<EntityType>() = EntityType::KAMIKAZI;
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
			CentipedeBoss::Init();
			m_SceneResumeNo = int(ScenesNum::UNIVERSE_27);

			//m_meshes.push_back(new Mesh());
			loadOBJ("Resource Files/OBJFiles/Universe-27/EnemyShips/Nerotist.obj", *m_meshes[int(EnemyMesh::NEROTISTU1)]);

			//m_meshes.push_back(new Mesh());
			loadOBJ("Resource Files/OBJFiles/Universe-27/EnemyShips/Kamikazi.obj", *m_meshes[int(EnemyMesh::KAMAKAZI)]);

			//m_meshes.push_back(new Mesh());
			loadOBJ("Resource Files/OBJFiles/Universe-27/EnemyShips/Bombardier.obj", *m_meshes[int(EnemyMesh::BOMBARDIER)]);

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
			nullPlanetOneEntity->GetComponent<Transform>().SetRadius((m_meshes[int(PlanetMesh::DEDMOS)]->GetWidth() / 2));

			// Titanius
			auto nullPlanetTwoEntity = GameObject::Allocate();
			m_solarSystem.push_back(nullPlanetTwoEntity->GetID());
			nullPlanetTwoEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(1500, 0, 0));
			nullPlanetTwoEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), nullPlanetTwoEntity->GetID(), *m_meshes[int(PlanetMesh::TITANIUS)], nullptr);
			nullPlanetTwoEntity->GetComponent<Transform>().SetRadius((m_meshes[int(PlanetMesh::TITANIUS)]->GetWidth() / 2));

			// Kreillo
			auto nullPlanetThreeEntity = GameObject::Allocate();
			m_solarSystem.push_back(nullPlanetThreeEntity->GetID());
			nullPlanetThreeEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, -2250));
			nullPlanetThreeEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), nullPlanetThreeEntity->GetID(), *m_meshes[int(PlanetMesh::KREILLO)], nullptr);
			nullPlanetThreeEntity->GetComponent<Transform>().SetRadius((m_meshes[int(PlanetMesh::KREILLO)]->GetWidth() / 2));

			// Paxallus
			auto citadelEntity = GameObject::Allocate();
			m_solarSystem.push_back(citadelEntity->GetID());
			citadelEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(-2875, 0, 0));
			citadelEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), citadelEntity->GetID(), *m_meshes[int(PlanetMesh::PAXALLUS)], nullptr);
			citadelEntity->GetComponent<Transform>().SetRadius((m_meshes[int(PlanetMesh::PAXALLUS)]->GetWidth() / 2));
			citadelEntity->GetComponent<Transform>().SetLocalScale(glm::vec3(2));

			// Derangi
			auto mushroomPlanetEntity = GameObject::Allocate();
			m_solarSystem.push_back(mushroomPlanetEntity->GetID());
			mushroomPlanetEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 3500));
			mushroomPlanetEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), mushroomPlanetEntity->GetID(), *m_meshes[int(PlanetMesh::DERANGI)], nullptr);
			mushroomPlanetEntity->GetComponent<Transform>().SetRadius((m_meshes[int(PlanetMesh::DERANGI)]->GetWidth() / 2));

			// Rhetoid
			auto gasPlanetOneEntity = GameObject::Allocate();
			m_solarSystem.push_back(gasPlanetOneEntity->GetID());
			gasPlanetOneEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(4250, 0, 0));
			gasPlanetOneEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), gasPlanetOneEntity->GetID(), *m_meshes[int(PlanetMesh::RHETOID)], nullptr);
			gasPlanetOneEntity->GetComponent<Transform>().SetRadius((m_meshes[int(PlanetMesh::RHETOID)]->GetWidth() / 2));

			// Magaantu
			auto gasPlanetTwoEntity = GameObject::Allocate();
			m_solarSystem.push_back(gasPlanetTwoEntity->GetID());
			gasPlanetTwoEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, -5000));
			gasPlanetTwoEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), gasPlanetTwoEntity->GetID(), *m_meshes[int(PlanetMesh::MAGAANTU)], nullptr);
			gasPlanetTwoEntity->GetComponent<Transform>().SetRadius((m_meshes[int(PlanetMesh::MAGAANTU)]->GetWidth() / 2));

			//Setting Solar System Parent/Childe	
			nullPlanetOneEntity->GetComponent<Transform>().SetParent(&m_solarSystem[Universe27SS::LDMC]);
			nullPlanetTwoEntity->GetComponent<Transform>().SetParent(&m_solarSystem[Universe27SS::LTC]);
			nullPlanetThreeEntity->GetComponent<Transform>().SetParent(&m_solarSystem[Universe27SS::LKC]);
			citadelEntity->GetComponent<Transform>().SetParent(&m_solarSystem[Universe27SS::LPC]);
			mushroomPlanetEntity->GetComponent<Transform>().SetParent(&m_solarSystem[Universe27SS::LDRC]);
			gasPlanetOneEntity->GetComponent<Transform>().SetParent(&m_solarSystem[Universe27SS::LRC]);
			gasPlanetTwoEntity->GetComponent<Transform>().SetParent(&m_solarSystem[Universe27SS::LMC]);

			//Ice planet enemies
			for (int i = 0; i < 15; i++) {
				auto enemy = GameObject::Allocate();
				enemy->AttachComponent<Transform>();
				enemy->AttachComponent<KamikaziAI>(enemy->GetID(), nullPlanetOneEntity->GetID(), playerEntity->GetID()).SetBulletMesh(m_meshes[int(PlayerMesh::PLAYERBULLET)]);;
				enemy->AttachComponent<EntityType>() = EntityType::KAMIKAZI;
				enemy->AttachComponent<StaticRenderer>(cameraEntity->GetID(), enemy->GetID(), *m_meshes[int(EnemyMesh::KAMAKAZI)], nullptr);
				enemy->GetComponent<Transform>().SetWHD(glm::vec3(m_meshes[int(EnemyMesh::KAMAKAZI)]->GetWidth(), m_meshes[int(EnemyMesh::KAMAKAZI)]->GetHeight(), m_meshes[int(EnemyMesh::KAMAKAZI)]->GetDepth()));

			}
			//Lava planet enemies
			for (int i = 0; i < 10; i++) {
				auto enemy = GameObject::Allocate();
				enemy->AttachComponent<Transform>();
				enemy->AttachComponent<BasicAI>(enemy->GetID(), nullPlanetTwoEntity->GetID(), playerEntity->GetID());
				enemy->AttachComponent<EntityType>() = EntityType::NEROTIST;
				enemy->AttachComponent<StaticRenderer>(cameraEntity->GetID(), enemy->GetID(), *m_meshes[int(EnemyMesh::NEROTISTU1)], nullptr);
				enemy->GetComponent<Transform>().SetWHD(glm::vec3(m_meshes[int(EnemyMesh::NEROTISTU1)]->GetWidth(), m_meshes[int(EnemyMesh::NEROTISTU1)]->GetHeight(), m_meshes[int(EnemyMesh::NEROTISTU1)]->GetDepth()));

			}
			//Rockplanet enemies
			for (int i = 0; i < 10; i++) {
				auto enemy = GameObject::Allocate();
				enemy->AttachComponent<Transform>();
				enemy->AttachComponent<BasicAI>(enemy->GetID(), mushroomPlanetEntity->GetID(), playerEntity->GetID());
				enemy->AttachComponent<EntityType>() = EntityType::NEROTIST;
				enemy->AttachComponent<StaticRenderer>(cameraEntity->GetID(), enemy->GetID(), *m_meshes[int(EnemyMesh::NEROTISTU1)], nullptr);
				enemy->GetComponent<Transform>().SetWHD(glm::vec3(m_meshes[int(EnemyMesh::NEROTISTU1)]->GetWidth(), m_meshes[int(EnemyMesh::NEROTISTU1)]->GetHeight(), m_meshes[int(EnemyMesh::NEROTISTU1)]->GetDepth()));
			}
			for (int i = 0; i < 10; i++) {
				auto enemy = GameObject::Allocate();
				enemy->AttachComponent<Transform>();
				enemy->AttachComponent<KamikaziAI>(enemy->GetID(), citadelEntity->GetID(), playerEntity->GetID());
				enemy->AttachComponent<EntityType>() = EntityType::KAMIKAZI;
				enemy->AttachComponent<StaticRenderer>(cameraEntity->GetID(), enemy->GetID(), *m_meshes[int(EnemyMesh::KAMAKAZI)], nullptr);
				enemy->GetComponent<Transform>().SetWHD(glm::vec3(m_meshes[int(EnemyMesh::KAMAKAZI)]->GetWidth(), m_meshes[int(EnemyMesh::KAMAKAZI)]->GetHeight(), m_meshes[int(EnemyMesh::KAMAKAZI)]->GetDepth()));
			}
			//Kamikaze
			for (int i = 0; i < 20; i++) {
				auto enemy = GameObject::Allocate();
				enemy->AttachComponent<Transform>().SetLocalPos(glm::vec3(Random::Range1f(400, 900), 0, Random::Range1f(-800, -100)));
				enemy->AttachComponent<KamikaziAI>(enemy->GetID(), enemy->GetID(), playerEntity->GetID());
				enemy->AttachComponent<EntityType>() = EntityType::KAMIKAZI;
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
				asteroidBelt->AttachComponent<StaticRenderer>(cameraEntity->GetID(), asteroidBelt->GetID(), *m_meshes[int(PlanetMesh::COMET) + 14], nullptr);
				asteroidBelt->GetComponent<Transform>().SetParent(&m_solarSystem[Universe27SS::LDMC]);

				//Asteroid Clusters
				auto cluster1 = GameObject::Allocate();
				cluster1->AttachComponent<Transform>().SetLocalPos(glm::vec3(0));
				cluster1->GetComponent<Transform>().SetLocalScale(glm::vec3(50, 50, 50));
				cluster1->AttachComponent<StaticRenderer>(cameraEntity->GetID(), cluster1->GetID(), *m_meshes[int(PlanetMesh::COMET) + 15], nullptr);
				cluster1->GetComponent<Transform>().SetParent(&m_solarSystem[Universe27SS::LKC]);

				auto cluster2 = GameObject::Allocate();
				cluster2->AttachComponent<Transform>().SetLocalPos(glm::vec3(0));
				cluster2->GetComponent<Transform>().SetLocalScale(glm::vec3(50, 50, 50));
				cluster2->AttachComponent<StaticRenderer>(cameraEntity->GetID(), cluster2->GetID(), *m_meshes[int(PlanetMesh::COMET) + 16], nullptr);
				cluster2->GetComponent<Transform>().SetParent(&m_solarSystem[Universe27SS::LKC]);

				auto cluster3 = GameObject::Allocate();
				cluster3->AttachComponent<Transform>().SetLocalPos(glm::vec3(0));
				cluster3->GetComponent<Transform>().SetLocalScale(glm::vec3(50, 50, 50));
				cluster3->AttachComponent<StaticRenderer>(cameraEntity->GetID(), cluster3->GetID(), *m_meshes[int(PlanetMesh::COMET) + 17], nullptr);
				cluster3->GetComponent<Transform>().SetParent(&m_solarSystem[Universe27SS::LKC]);

				auto cluster4 = GameObject::Allocate();
				cluster4->AttachComponent<Transform>().SetLocalPos(glm::vec3(0));
				cluster4->GetComponent<Transform>().SetLocalScale(glm::vec3(50, 50, 50));
				cluster4->AttachComponent<StaticRenderer>(cameraEntity->GetID(), cluster4->GetID(), *m_meshes[int(PlanetMesh::COMET) + 18], nullptr);
				cluster4->GetComponent<Transform>().SetParent(&m_solarSystem[Universe27SS::LKC]);

				auto cluster5 = GameObject::Allocate();
				cluster5->AttachComponent<Transform>().SetLocalPos(glm::vec3(0));
				cluster5->GetComponent<Transform>().SetLocalScale(glm::vec3(50, 50, 50));
				cluster5->AttachComponent<StaticRenderer>(cameraEntity->GetID(), cluster5->GetID(), *m_meshes[int(PlanetMesh::COMET) + 19], nullptr);
				cluster5->GetComponent<Transform>().SetParent(&m_solarSystem[Universe27SS::LKC]);

				auto cluster6 = GameObject::Allocate();
				cluster6->AttachComponent<Transform>().SetLocalPos(glm::vec3(0));
				cluster6->GetComponent<Transform>().SetLocalScale(glm::vec3(50, 50, 50));
				cluster6->AttachComponent<StaticRenderer>(cameraEntity->GetID(), cluster6->GetID(), *m_meshes[int(PlanetMesh::COMET) + 20], nullptr);
				cluster6->GetComponent<Transform>().SetParent(&m_solarSystem[Universe27SS::LKC]);

				auto cluster7 = GameObject::Allocate();
				cluster7->AttachComponent<Transform>().SetLocalPos(glm::vec3(0));
				cluster7->GetComponent<Transform>().SetLocalScale(glm::vec3(50, 50, 50));
				cluster7->AttachComponent<StaticRenderer>(cameraEntity->GetID(), cluster7->GetID(), *m_meshes[int(PlanetMesh::COMET) + 21], nullptr);
				cluster7->GetComponent<Transform>().SetParent(&m_solarSystem[Universe27SS::LKC]);

				auto cluster8 = GameObject::Allocate();
				cluster8->AttachComponent<Transform>().SetLocalPos(glm::vec3(0));
				cluster8->GetComponent<Transform>().SetLocalScale(glm::vec3(50, 50, 50));
				cluster8->AttachComponent<StaticRenderer>(cameraEntity->GetID(), cluster8->GetID(), *m_meshes[int(PlanetMesh::COMET) + 22], nullptr);
				cluster8->GetComponent<Transform>().SetParent(&m_solarSystem[Universe27SS::LKC]);
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
					debris->GetComponent<Transform>().SetParent(&m_solarSystem[Universe27SS::LDMC]);
				}
				else if (i > 10 && i <= 20)
				{
					//Barrel bot
					debris->AttachComponent<Transform>().SetLocalPos(glm::vec3(Random::Range1f(-1500, 1500), Random::Range1f(-200, -10), Random::Range1f(1100, 3800)));
					debris->GetComponent<Transform>().SetLocalScale(glm::vec3(2, 2, 2));
					debris->GetComponent<Transform>().SetLocalRot(Random::Range3f(-300, 300));
					debris->AttachComponent<StaticRenderer>(cameraEntity->GetID(), debris->GetID(), *m_meshes[int(PlanetMesh::COMET) + 1], nullptr);
					debris->GetComponent<Transform>().SetParent(&m_solarSystem[Universe27SS::LDMC]);
				}
				else if (i > 20 && i <= 30)
				{
					//Bottle top
					debris->AttachComponent<Transform>().SetLocalPos(glm::vec3(Random::Range1f(-2000, 2000), Random::Range1f(10, 300), Random::Range1f(1200, 4000)));
					debris->GetComponent<Transform>().SetLocalScale(glm::vec3(4, 4, 4));
					debris->GetComponent<Transform>().SetLocalRot(Random::Range3f(-300, 300));
					debris->AttachComponent<StaticRenderer>(cameraEntity->GetID(), debris->GetID(), *m_meshes[int(PlanetMesh::COMET) + 2], nullptr);
					debris->GetComponent<Transform>().SetParent(&m_solarSystem[Universe27SS::LDMC]);
				}
				else if (i > 30 && i <= 50)
				{
					//Box bot
					debris->AttachComponent<Transform>().SetLocalPos(glm::vec3(Random::Range1f(-2000, 2000), Random::Range1f(-300, -10), Random::Range1f(1200, 4000)));
					debris->GetComponent<Transform>().SetLocalScale(glm::vec3(4, 4, 4));
					debris->GetComponent<Transform>().SetLocalRot(Random::Range3f(-300, 300));
					debris->AttachComponent<StaticRenderer>(cameraEntity->GetID(), debris->GetID(), *m_meshes[int(PlanetMesh::COMET) + 3], nullptr);
					debris->GetComponent<Transform>().SetParent(&m_solarSystem[Universe27SS::LDMC]);
				}
			}
		}
		else if (m_name == "Universe_5") {
			HiveMindBoss::Init();
			m_SceneResumeNo = int(ScenesNum::UNIVERSE_5);

			//m_meshes.push_back(new Mesh());
			loadOBJ("Resource Files/OBJFiles/Universe-5/EnemyShips/Nerotist.obj", *m_meshes[int(EnemyMesh::NEROTISTU1)]);

			//m_meshes.push_back(new Mesh());
			loadOBJ("Resource Files/OBJFiles/Universe-5/EnemyShips/Kamikazi.obj", *m_meshes[int(EnemyMesh::KAMAKAZI)]);

			//m_meshes.push_back(new Mesh());
			loadOBJ("Resource Files/OBJFiles/Universe-5/EnemyShips/Bombardier.obj", *m_meshes[int(EnemyMesh::BOMBARDIER)]);

			// Solar System Centerpoint
			auto DarkPCEntity = GameObject::Allocate();
			m_solarSystem.push_back(DarkPCEntity->GetID());
			DarkPCEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 0));
			auto DualPCEntity = GameObject::Allocate();
			m_solarSystem.push_back(DualPCEntity->GetID());
			DualPCEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 0));
			auto PinkPCEntity = GameObject::Allocate();
			m_solarSystem.push_back(PinkPCEntity->GetID());
			PinkPCEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 0));
			auto SpikePCEntity = GameObject::Allocate();
			m_solarSystem.push_back(SpikePCEntity->GetID());
			SpikePCEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 0));
			auto WhitePCEntity = GameObject::Allocate();
			m_solarSystem.push_back(WhitePCEntity->GetID());
			WhitePCEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 0));


			// Ra'lei - Black Hole/Sun
			auto sunEntity = GameObject::Allocate();
			m_solarSystem.push_back(sunEntity->GetID());
			sunEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 0));
			sunEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), sunEntity->GetID(), *m_meshes[int(PlanetMesh::RALEI)], nullptr, true);
			sunEntity->GetComponent<Transform>().SetRadius(3 * (m_meshes[int(PlanetMesh::RALEI)]->GetWidth() / 2));
			sunEntity->GetComponent<Transform>().SetLocalScale(glm::vec3(3.0));

			// Dark planet
			auto darkPEntity = GameObject::Allocate();
			m_solarSystem.push_back(darkPEntity->GetID());
			darkPEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 1750));
			darkPEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), darkPEntity->GetID(), *m_meshes[int(PlanetMesh::DARKP)], nullptr);
			darkPEntity->GetComponent<Transform>().SetRadius((m_meshes[int(PlanetMesh::DARKP)]->GetWidth() / 2));

			// Dual planet
			auto dualPEntity = GameObject::Allocate();
			m_solarSystem.push_back(dualPEntity->GetID());
			dualPEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(1500, 0, 0));
			dualPEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), dualPEntity->GetID(), *m_meshes[int(PlanetMesh::DUALP)], nullptr);
			dualPEntity->GetComponent<Transform>().SetRadius((m_meshes[int(PlanetMesh::DUALP)]->GetWidth() / 2));

			// Pink planet
			auto pinkPEntity = GameObject::Allocate();
			m_solarSystem.push_back(pinkPEntity->GetID());
			pinkPEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, -2250));
			pinkPEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), pinkPEntity->GetID(), *m_meshes[int(PlanetMesh::PINKP)], nullptr);
			pinkPEntity->GetComponent<Transform>().SetRadius((m_meshes[int(PlanetMesh::PINKP)]->GetWidth() / 2));

			// Spike planet
			auto spikePEntity = GameObject::Allocate();
			m_solarSystem.push_back(spikePEntity->GetID());
			spikePEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(-1500, 0, 2000));
			spikePEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), spikePEntity->GetID(), *m_meshes[int(PlanetMesh::SPIKEP)], nullptr);
			spikePEntity->GetComponent<Transform>().SetRadius((m_meshes[int(PlanetMesh::SPIKEP)]->GetWidth() / 2));

			// White planet
			auto whitePEntity = GameObject::Allocate();
			m_solarSystem.push_back(whitePEntity->GetID());
			whitePEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(-2875, 0, 0));
			whitePEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), whitePEntity->GetID(), *m_meshes[int(PlanetMesh::WHITEP)], nullptr);
			whitePEntity->GetComponent<Transform>().SetRadius((m_meshes[int(PlanetMesh::WHITEP)]->GetWidth() / 2));

			//Setting Solar System Parent/Childe	
			darkPEntity->GetComponent<Transform>().SetParent(&m_solarSystem[Universe5SS::RDKC]);
			dualPEntity->GetComponent<Transform>().SetParent(&m_solarSystem[Universe5SS::RDLC]);
			pinkPEntity->GetComponent<Transform>().SetParent(&m_solarSystem[Universe5SS::RPC]);
			spikePEntity->GetComponent<Transform>().SetParent(&m_solarSystem[Universe5SS::RSC]);
			whitePEntity->GetComponent<Transform>().SetParent(&m_solarSystem[Universe5SS::RWC]);

			//Dark planet enemies
			for (int i = 0; i < 15; i++) {
				auto enemy = GameObject::Allocate();
				enemy->AttachComponent<Transform>();
				enemy->AttachComponent<KamikaziAI>(enemy->GetID(), darkPEntity->GetID(), playerEntity->GetID()).SetBulletMesh(m_meshes[int(PlayerMesh::PLAYERBULLET)]);;
				enemy->AttachComponent<EntityType>() = EntityType::KAMIKAZI;
				enemy->AttachComponent<StaticRenderer>(cameraEntity->GetID(), enemy->GetID(), *m_meshes[int(EnemyMesh::KAMAKAZI)], nullptr);
				enemy->GetComponent<Transform>().SetWHD(glm::vec3(m_meshes[int(EnemyMesh::KAMAKAZI)]->GetWidth(), m_meshes[int(EnemyMesh::KAMAKAZI)]->GetHeight(), m_meshes[int(EnemyMesh::KAMAKAZI)]->GetDepth()));

			}
			//White planet enemies
			for (int i = 0; i < 10; i++) {
				auto enemy = GameObject::Allocate();
				enemy->AttachComponent<Transform>();
				enemy->AttachComponent<BasicAI>(enemy->GetID(), whitePEntity->GetID(), playerEntity->GetID());
				enemy->AttachComponent<EntityType>() = EntityType::NEROTIST;
				enemy->AttachComponent<StaticRenderer>(cameraEntity->GetID(), enemy->GetID(), *m_meshes[int(EnemyMesh::NEROTISTU1)], nullptr);
				enemy->GetComponent<Transform>().SetWHD(glm::vec3(m_meshes[int(EnemyMesh::NEROTISTU1)]->GetWidth(), m_meshes[int(EnemyMesh::NEROTISTU1)]->GetHeight(), m_meshes[int(EnemyMesh::NEROTISTU1)]->GetDepth()));

			}
			//Dual Planet enemies
			for (int i = 0; i < 10; i++) {
				auto enemy = GameObject::Allocate();
				enemy->AttachComponent<Transform>();
				enemy->AttachComponent<BasicAI>(enemy->GetID(), dualPEntity->GetID(), playerEntity->GetID());
				enemy->AttachComponent<EntityType>() = EntityType::NEROTIST;
				enemy->AttachComponent<StaticRenderer>(cameraEntity->GetID(), enemy->GetID(), *m_meshes[int(EnemyMesh::NEROTISTU1)], nullptr);
				enemy->GetComponent<Transform>().SetWHD(glm::vec3(m_meshes[int(EnemyMesh::NEROTISTU1)]->GetWidth(), m_meshes[int(EnemyMesh::NEROTISTU1)]->GetHeight(), m_meshes[int(EnemyMesh::NEROTISTU1)]->GetDepth()));
			}
			for (int i = 0; i < 10; i++) {
				auto enemy = GameObject::Allocate();
				enemy->AttachComponent<Transform>();
				enemy->AttachComponent<KamikaziAI>(enemy->GetID(), spikePEntity->GetID(), playerEntity->GetID());
				enemy->AttachComponent<EntityType>() = EntityType::KAMIKAZI;
				enemy->AttachComponent<StaticRenderer>(cameraEntity->GetID(), enemy->GetID(), *m_meshes[int(EnemyMesh::KAMAKAZI)], nullptr);
				enemy->GetComponent<Transform>().SetWHD(glm::vec3(m_meshes[int(EnemyMesh::KAMAKAZI)]->GetWidth(), m_meshes[int(EnemyMesh::KAMAKAZI)]->GetHeight(), m_meshes[int(EnemyMesh::KAMAKAZI)]->GetDepth()));
			}
			//Kamikaze
			for (int i = 0; i < 20; i++) {
				auto enemy = GameObject::Allocate();
				enemy->AttachComponent<Transform>().SetLocalPos(glm::vec3(Random::Range1f(400, 900), 0, Random::Range1f(-800, -100)));
				enemy->AttachComponent<KamikaziAI>(enemy->GetID(), enemy->GetID(), playerEntity->GetID());
				enemy->AttachComponent<EntityType>() = EntityType::KAMIKAZI;
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
				asteroidBelt->AttachComponent<StaticRenderer>(cameraEntity->GetID(), asteroidBelt->GetID(), *m_meshes[int(PlanetMesh::COMET) + 23], nullptr);
				asteroidBelt->GetComponent<Transform>().SetParent(&m_solarSystem[Universe5SS::RDKC]);

				//Asteroid Clusters
				auto cluster1 = GameObject::Allocate();
				cluster1->AttachComponent<Transform>().SetLocalPos(glm::vec3(0));
				cluster1->GetComponent<Transform>().SetLocalScale(glm::vec3(50, 50, 50));
				cluster1->AttachComponent<StaticRenderer>(cameraEntity->GetID(), cluster1->GetID(), *m_meshes[int(PlanetMesh::COMET) + 24], nullptr);
				cluster1->GetComponent<Transform>().SetParent(&m_solarSystem[Universe5SS::RPC]);

				auto cluster2 = GameObject::Allocate();
				cluster2->AttachComponent<Transform>().SetLocalPos(glm::vec3(0));
				cluster2->GetComponent<Transform>().SetLocalScale(glm::vec3(50, 50, 50));
				cluster2->AttachComponent<StaticRenderer>(cameraEntity->GetID(), cluster2->GetID(), *m_meshes[int(PlanetMesh::COMET) + 25], nullptr);
				cluster2->GetComponent<Transform>().SetParent(&m_solarSystem[Universe5SS::RPC]);

				auto cluster3 = GameObject::Allocate();
				cluster3->AttachComponent<Transform>().SetLocalPos(glm::vec3(0));
				cluster3->GetComponent<Transform>().SetLocalScale(glm::vec3(50, 50, 50));
				cluster3->AttachComponent<StaticRenderer>(cameraEntity->GetID(), cluster3->GetID(), *m_meshes[int(PlanetMesh::COMET) + 26], nullptr);
				cluster3->GetComponent<Transform>().SetParent(&m_solarSystem[Universe5SS::RPC]);

				auto cluster4 = GameObject::Allocate();
				cluster4->AttachComponent<Transform>().SetLocalPos(glm::vec3(0));
				cluster4->GetComponent<Transform>().SetLocalScale(glm::vec3(50, 50, 50));
				cluster4->AttachComponent<StaticRenderer>(cameraEntity->GetID(), cluster4->GetID(), *m_meshes[int(PlanetMesh::COMET) + 27], nullptr);
				cluster4->GetComponent<Transform>().SetParent(&m_solarSystem[Universe5SS::RPC]);

				auto cluster5 = GameObject::Allocate();
				cluster5->AttachComponent<Transform>().SetLocalPos(glm::vec3(0));
				cluster5->GetComponent<Transform>().SetLocalScale(glm::vec3(50, 50, 50));
				cluster5->AttachComponent<StaticRenderer>(cameraEntity->GetID(), cluster5->GetID(), *m_meshes[int(PlanetMesh::COMET) + 28], nullptr);
				cluster5->GetComponent<Transform>().SetParent(&m_solarSystem[Universe5SS::RPC]);

				auto cluster6 = GameObject::Allocate();
				cluster6->AttachComponent<Transform>().SetLocalPos(glm::vec3(0));
				cluster6->GetComponent<Transform>().SetLocalScale(glm::vec3(50, 50, 50));
				cluster6->AttachComponent<StaticRenderer>(cameraEntity->GetID(), cluster6->GetID(), *m_meshes[int(PlanetMesh::COMET) + 29], nullptr);
				cluster6->GetComponent<Transform>().SetParent(&m_solarSystem[Universe5SS::RPC]);

				auto cluster7 = GameObject::Allocate();
				cluster7->AttachComponent<Transform>().SetLocalPos(glm::vec3(0));
				cluster7->GetComponent<Transform>().SetLocalScale(glm::vec3(50, 50, 50));
				cluster7->AttachComponent<StaticRenderer>(cameraEntity->GetID(), cluster7->GetID(), *m_meshes[int(PlanetMesh::COMET) + 30], nullptr);
				cluster7->GetComponent<Transform>().SetParent(&m_solarSystem[Universe5SS::RPC]);

				auto cluster8 = GameObject::Allocate();
				cluster8->AttachComponent<Transform>().SetLocalPos(glm::vec3(0));
				cluster8->GetComponent<Transform>().SetLocalScale(glm::vec3(50, 50, 50));
				cluster8->AttachComponent<StaticRenderer>(cameraEntity->GetID(), cluster8->GetID(), *m_meshes[int(PlanetMesh::COMET) + 31], nullptr);
				cluster8->GetComponent<Transform>().SetParent(&m_solarSystem[Universe5SS::RPC]);
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
					debris->GetComponent<Transform>().SetParent(&m_solarSystem[Universe27SS::LDMC]);
				}
				else if (i > 10 && i <= 20)
				{
					//Barrel bot
					debris->AttachComponent<Transform>().SetLocalPos(glm::vec3(Random::Range1f(-1500, 1500), Random::Range1f(-200, -10), Random::Range1f(1100, 3800)));
					debris->GetComponent<Transform>().SetLocalScale(glm::vec3(2, 2, 2));
					debris->GetComponent<Transform>().SetLocalRot(Random::Range3f(-300, 300));
					debris->AttachComponent<StaticRenderer>(cameraEntity->GetID(), debris->GetID(), *m_meshes[int(PlanetMesh::COMET) + 1], nullptr);
					debris->GetComponent<Transform>().SetParent(&m_solarSystem[Universe27SS::LDMC]);
				}
				else if (i > 20 && i <= 30)
				{
					//Bottle top
					debris->AttachComponent<Transform>().SetLocalPos(glm::vec3(Random::Range1f(-2000, 2000), Random::Range1f(10, 300), Random::Range1f(1200, 4000)));
					debris->GetComponent<Transform>().SetLocalScale(glm::vec3(4, 4, 4));
					debris->GetComponent<Transform>().SetLocalRot(Random::Range3f(-300, 300));
					debris->AttachComponent<StaticRenderer>(cameraEntity->GetID(), debris->GetID(), *m_meshes[int(PlanetMesh::COMET) + 2], nullptr);
					debris->GetComponent<Transform>().SetParent(&m_solarSystem[Universe27SS::LDMC]);
				}
				else if (i > 30 && i <= 50)
				{
					//Box bot
					debris->AttachComponent<Transform>().SetLocalPos(glm::vec3(Random::Range1f(-2000, 2000), Random::Range1f(-300, -10), Random::Range1f(1200, 4000)));
					debris->GetComponent<Transform>().SetLocalScale(glm::vec3(4, 4, 4));
					debris->GetComponent<Transform>().SetLocalRot(Random::Range3f(-300, 300));
					debris->AttachComponent<StaticRenderer>(cameraEntity->GetID(), debris->GetID(), *m_meshes[int(PlanetMesh::COMET) + 3], nullptr);
					debris->GetComponent<Transform>().SetParent(&m_solarSystem[Universe27SS::LDMC]);
				}
			}
		}

		//Setting Parent/Childe
		cameraEntity->GetComponent<Transform>().SetParent(new entt::entity(playerEntity->GetID()));
		healthent->GetComponent<Transform>().SetParent(camentity);
		score->GetComponent<Transform>().SetParent(camentity);


	}

	Skybox::Init(m_name);
}

void Universe::Update(float deltaTime)
{
	if (isTransitionActive) {
		intensity += 0.01;

		if (intensity >= 1.0) {
			*switchIt = true;

			if (m_SceneResumeNo == int(ScenesNum::UNIVERSE_19))
				*SceneNo = int(ScenesNum::UNIVERSE_27);

			else if (m_SceneResumeNo == int(ScenesNum::UNIVERSE_27))
				*SceneNo = int(ScenesNum::UNIVERSE_5);

			isTransitionActive = false;
			wasSceneSwitched = true;
		}
	}
	else if (wasTransitionActive) {
		intensity -= 0.01;

		if (intensity <= 0.0) {
			intensity = 0.0;
			wasTransitionActive = false;
			wasSceneSwitched = false;
		}
	}
	else {
		m_deltaTime = deltaTime;

		// Key Input
		KeyInput();
		GamepadInput();

		// Camera Update 
		camera->Update();

		// Solar System Rotation (IN-PROGRESS) 
		SolarSystemUpdate();

		//playerController->Update(deltaTime);

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

		// Transform Update
		m_sceneReg->view<Transform>().each([=](Transform& transform) { transform.UpdateGlobal(); });
		m_sceneReg->view<AnimationHandler>().each([=](AnimationHandler& anim) { anim.Update(deltaTime); });
		m_sceneReg->view<MorphAnimController>().each([=](MorphAnimController& anim) { anim.Update(deltaTime); });

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
			else if (type == EntityType::KAMIKAZI) {
				AI.push_back(&GameObject::GetComponent<KamikaziAI>(enemy));
				AI[index]->Update(deltaTime);

				if (isBoxCollide(GameObject::GetComponent<Transform>(enemy), GameObject::GetComponent<Transform>(MainPlayerID))) {
					AI.pop_back();

					AudioEngine& engine = AudioEngine::Instance();
					//Create event
					AudioEvent& explosionEvent = engine.GetEvent("Explosion");

					if (!explosionEvent.isPlaying())
						explosionEvent.Play();
					else
					{
						explosionEvent.Stop();
						explosionEvent.Play();
					}

					particleTemp = new ParticleController(2, GameObject::GetComponent<Transform>(enemy).GetLocalPos(), m_textures[int(TextureType::YELLOW)], enemy);
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

			else if (type == EntityType::KAMIBULLET) {
				GameObject::GetComponent<KamakaziBullet>(enemy).Update(deltaTime);

				if (GameObject::GetComponent<KamakaziBullet>(enemy).GetDestroyed())
				{
					particleTemp = new ParticleController(2, GameObject::GetComponent<Transform>(enemy).GetLocalPos(), m_textures[int(TextureType::YELLOW)], enemy);
					particleTemp->setSize(10);
					particleTemp->getEmitter()->setLifetime(0.2, 0.2);
					particleTemp->getEmitter()->setSpeed(100);
					particleTemp->getEmitter()->init();
					GameObject::GetComponent<Transform>(enemy).SetLocalScale(glm::vec3(1));
					particleTemp->setModelMatrix(GameObject::GetComponent<Transform>(enemy).UpdateGlobal());
					GameObject::GetComponent<Transform>(enemy).SetLocalScale(glm::vec3(5))->UpdateGlobal();
					particles.push_back(particleTemp);
					m_sceneReg->destroy(enemy);

					AudioEngine& engine = AudioEngine::Instance();
					//Create event
					AudioEvent& explosionEvent = engine.GetEvent("Explosion");

					if (!explosionEvent.isPlaying())
						explosionEvent.Play();
					else
					{
						explosionEvent.Stop();
						explosionEvent.Play();
					}
				}
				else if (isBoxCollide(GameObject::GetComponent<Transform>(enemy), GameObject::GetComponent<Transform>(MainPlayerID))) {
					particleTemp = new ParticleController(2, GameObject::GetComponent<Transform>(enemy).GetLocalPos(), m_textures[int(TextureType::YELLOW)], enemy);
					particleTemp->setSize(10);
					particleTemp->getEmitter()->setLifetime(0.2, 0.2);
					particleTemp->getEmitter()->setSpeed(100);
					particleTemp->getEmitter()->init();
					GameObject::GetComponent<Transform>(enemy).SetLocalScale(glm::vec3(1));
					particleTemp->setModelMatrix(GameObject::GetComponent<Transform>(enemy).UpdateGlobal());
					GameObject::GetComponent<Transform>(enemy).SetLocalScale(glm::vec3(5))->UpdateGlobal();
					particles.push_back(particleTemp);
					m_sceneReg->destroy(enemy);

					AudioEngine& engine = AudioEngine::Instance();
					//Create event
					AudioEvent& explosionEvent = engine.GetEvent("Explosion");

					if (!explosionEvent.isPlaying())
						explosionEvent.Play();
					else
					{
						explosionEvent.Stop();
						explosionEvent.Play();
					}

					m_PlayerHealth -= m_PlayerHealth > 0 ? 1 : 0;
					GameObject::GetComponent<AnimationHandler>(health).SetActiveAnim(m_PlayerHealth);
				}
			}

			else if (type == EntityType::JELLY) {
				AI.push_back(&GameObject::GetComponent<JellyFishBoss>(enemy));
				AI[index]->Update(deltaTime);

				if (m_arrowTotheBoss.get() != nullptr) {

					glm::vec3 playerPos = GameObject::GetComponent<Transform>(MainPlayerID).GetLocalPos();
					playerPos += glm::vec3(0, 20, 0);

					m_arrowTotheBoss->GetComponent<Transform>().SetLocalPos(playerPos);

					glm::vec3 rotationvector = glm::degrees(glm::eulerAngles(glm::quat_cast(glm::transpose(glm::lookAt(playerPos, GameObject::GetComponent<Transform>(enemy).GetLocalPos(), glm::vec3(0, 1, 0))))));

					m_arrowTotheBoss->GetComponent<Transform>().SetLocalRot(rotationvector);
				}
			}

			else if (type == EntityType::CENTIPEDE) {
				AI.push_back(&GameObject::GetComponent<CentipedeBoss>(enemy));
				AI[index]->Update(deltaTime);

				if (m_arrowTotheBoss.get() != nullptr) {

					glm::vec3 playerPos = GameObject::GetComponent<Transform>(MainPlayerID).GetLocalPos();
					playerPos += glm::vec3(0, 20, 0);

					m_arrowTotheBoss->GetComponent<Transform>().SetLocalPos(playerPos);

					glm::vec3 rotationvector = glm::degrees(glm::eulerAngles(glm::quat_cast(glm::transpose(glm::lookAt(playerPos, GameObject::GetComponent<Transform>(enemy).GetLocalPos(), glm::vec3(0, 1, 0))))));

					m_arrowTotheBoss->GetComponent<Transform>().SetLocalRot(rotationvector);
				}

			}

			else if (type == EntityType::HIVEMIND) {
				AI.push_back(&GameObject::GetComponent<HiveMindBoss>(enemy));

				if (GameObject::GetComponent<HiveMindBoss>(enemy).Phases() == 0 && m_isBossDead)
				{
					GameObject::GetComponent<HiveMindBoss>(enemy).JellyFishDefeated();
					m_isBossDead = false;
				}
				else if (GameObject::GetComponent<HiveMindBoss>(enemy).Phases() == 1 && m_isBossDead)
				{
					GameObject::GetComponent<HiveMindBoss>(enemy).CentipedeDefeated();
					m_isBossDead = false;
				}
				AI[index]->Update(deltaTime);

				if (GameObject::GetComponent<HiveMindBoss>(enemy).Phases() == 0 && !GameObject::GetComponent<HiveMindBoss>(enemy).m_JellySpawned) {
					auto jellyEntity = GameObject::Allocate();
					jellyEntity->AttachComponent<JellyFishBoss>().m_enemyMesh = m_meshes[int(EnemyMesh::NEROTISTU1)];
					jellyEntity->GetComponent<JellyFishBoss>().Init(jellyEntity->GetID(), MainPlayerID);

					jellyEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0));
					auto& jellypos = jellyEntity->GetComponent<Transform>();

					particleTemp = new ParticleController(1, glm::vec3(jellypos.GetLocalPos().x - 6, jellypos.GetLocalPos().y + 13.0, jellypos.GetLocalPos().z + 6), m_textures[int(TextureType::YELLOW)], m_textures[int(TextureType::FIRE)], jellyEntity->GetID());
					particleTemp->setRotation(glm::vec3(90, 0, 0));
					particleTemp->getEmitter()->setRadius(1.0);
					particleTemp->setSize(2);
					particleTemp->getEmitter()->setAngleDeg(10);
					particleTemp->getEmitter()->setLifetime(0.1f, 1.5f);
					particleTemp->getEmitter()->setSpeed(10);
					particleTemp->getEmitter()->init();
					particles.push_back(particleTemp);
					jellyEntity->GetComponent<JellyFishBoss>().m_particles.push_back(particleTemp);

					particleTemp = new ParticleController(1, glm::vec3(jellypos.GetLocalPos().x + 6, jellypos.GetLocalPos().y + 13.0, jellypos.GetLocalPos().z + 6), m_textures[int(TextureType::YELLOW)], m_textures[int(TextureType::FIRE)], jellyEntity->GetID());
					particleTemp->setRotation(glm::vec3(90, 0, 0));
					particleTemp->getEmitter()->setRadius(1.0);
					particleTemp->setSize(2);
					particleTemp->getEmitter()->setAngleDeg(10);
					particleTemp->getEmitter()->setLifetime(0.1f, 1.5f);
					particleTemp->getEmitter()->setSpeed(10);
					particleTemp->getEmitter()->init();
					particles.push_back(particleTemp);
					jellyEntity->GetComponent<JellyFishBoss>().m_particles.push_back(particleTemp);

					particleTemp = new ParticleController(1, glm::vec3(jellypos.GetLocalPos().x - 6, jellypos.GetLocalPos().y + 13.0, jellypos.GetLocalPos().z - 6), m_textures[int(TextureType::YELLOW)], m_textures[int(TextureType::FIRE)], jellyEntity->GetID());
					particleTemp->setRotation(glm::vec3(90, 0, 0));
					particleTemp->getEmitter()->setRadius(1.0);
					particleTemp->setSize(2);
					particleTemp->getEmitter()->setAngleDeg(10);
					particleTemp->getEmitter()->setLifetime(0.1f, 1.5f);
					particleTemp->getEmitter()->setSpeed(10);
					particleTemp->getEmitter()->init();
					particles.push_back(particleTemp);
					jellyEntity->GetComponent<JellyFishBoss>().m_particles.push_back(particleTemp);

					particleTemp = new ParticleController(1, glm::vec3(jellypos.GetLocalPos().x + 6, jellypos.GetLocalPos().y + 13.0, jellypos.GetLocalPos().z - 6), m_textures[int(TextureType::YELLOW)], m_textures[int(TextureType::FIRE)], jellyEntity->GetID());
					particleTemp->setRotation(glm::vec3(90, 0, 0));
					particleTemp->getEmitter()->setRadius(1.0);
					particleTemp->setSize(2);
					particleTemp->getEmitter()->setAngleDeg(10);
					particleTemp->getEmitter()->setLifetime(0.1f, 1.5f);
					particleTemp->getEmitter()->setSpeed(10);
					particleTemp->getEmitter()->init();
					particles.push_back(particleTemp);
					jellyEntity->GetComponent<JellyFishBoss>().m_particles.push_back(particleTemp);

					glm::vec3 temp = GameObject::GetComponent<Transform>(enemy).GetLocalPos() + glm::vec3(30, -30, 50);
					jellypos.MoveLocalPos(temp);

					jellyEntity->AttachComponent<DynamicRenderer>(CamID, jellyEntity->GetID(), *jellyEntity->GetComponent<JellyFishBoss>().m_meshes[0], nullptr, false);
					jellyEntity->AttachComponent<MorphAnimController>(int(jellyEntity->GetID())).SetFrames(jellyEntity->GetComponent<JellyFishBoss>().m_meshes, 0, 24);
					jellyEntity->GetComponent<Transform>().SetLocalScale(glm::vec3(3.0));
					jellyEntity->GetComponent<Transform>().SetWHD(glm::vec3(jellyEntity->GetComponent<JellyFishBoss>().m_meshes[0]->GetWidth(), jellyEntity->GetComponent<JellyFishBoss>().m_meshes[0]->GetHeight() * 3, jellyEntity->GetComponent<JellyFishBoss>().m_meshes[0]->GetDepth()));
					jellyEntity->GetComponent<Transform>().SetRadius((jellyEntity->GetComponent<JellyFishBoss>().m_meshes[0]->GetWidth() * 3) / 2);
					jellyEntity->AttachComponent<EntityType>() = EntityType::JELLY;

					GameObject::GetComponent<HiveMindBoss>(enemy).m_JellySpawned = true;
				}

				else if (GameObject::GetComponent<HiveMindBoss>(enemy).Phases() == 1 && !GameObject::GetComponent<HiveMindBoss>(enemy).m_CentipedeSpawned) {

					auto Centipede = GameObject::Allocate();
					Centipede->AttachComponent<CentipedeBoss>().SetBulletMesh(m_meshes[int(PlayerMesh::PLAYERBULLET)]);
					Centipede->GetComponent<CentipedeBoss>().Init(Centipede->GetID(), MainPlayerID);

					Centipede->AttachComponent<Transform>().SetLocalPos(glm::vec3(0));
					auto& Centipedeypos = Centipede->GetComponent<Transform>();

					glm::vec3 temp = GameObject::GetComponent<Transform>(enemy).GetLocalPos() + glm::vec3(30, 0, 50);

					Centipedeypos.MoveLocalPos(temp);

					Centipede->AttachComponent<DynamicRenderer>(CamID, Centipede->GetID(), *Centipede->GetComponent<CentipedeBoss>().m_meshes[0], nullptr, false);
					Centipede->AttachComponent<MorphAnimController>(int(Centipede->GetID())).SetFrames(Centipede->GetComponent<CentipedeBoss>().m_meshes, 0, 4);
					Centipede->GetComponent<Transform>().SetLocalScale(glm::vec3(3.0));
					Centipede->GetComponent<Transform>().SetWHD(glm::vec3(Centipede->GetComponent<CentipedeBoss>().m_meshes[0]->GetWidth(), Centipede->GetComponent<CentipedeBoss>().m_meshes[0]->GetHeight() * 3, Centipede->GetComponent<CentipedeBoss>().m_meshes[0]->GetDepth()));
					Centipede->GetComponent<Transform>().SetRadius((Centipede->GetComponent<CentipedeBoss>().m_meshes[0]->GetWidth() * 3) / 2);
					Centipede->AttachComponent<EntityType>() = EntityType::CENTIPEDE;

					GameObject::GetComponent<HiveMindBoss>(enemy).m_CentipedeSpawned = true;
				}

				if (m_arrowTotheBoss.get() != nullptr) {

					glm::vec3 playerPos = GameObject::GetComponent<Transform>(MainPlayerID).GetLocalPos();
					playerPos += glm::vec3(0, 20, 0);

					m_arrowTotheBoss->GetComponent<Transform>().SetLocalPos(playerPos);

					glm::vec3 rotationvector = glm::degrees(glm::eulerAngles(glm::quat_cast(glm::transpose(glm::lookAt(playerPos, GameObject::GetComponent<Transform>(enemy).GetLocalPos(), glm::vec3(0, 1, 0))))));

					m_arrowTotheBoss->GetComponent<Transform>().SetLocalRot(rotationvector);
				}

			}
		}

		if (m_PlayerHealth <= 0) {
			*switchIt = true;
			*SceneNo = int(ScenesNum::GAME_OVER);
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
					EntityType type = GameObject::GetComponent<EntityType>(enemy);
					if (isBoxCollide(GameObject::GetComponent<Transform>(Bulletentity), GameObject::GetComponent<Transform>(AI[i]->GetID())))
					{

						if (type == EntityType::KAMIKAZI)
						{
							particleTemp = new ParticleController(2, GameObject::GetComponent<Transform>(enemy).GetLocalPos(), m_textures[int(TextureType::YELLOW)], enemy);
							particleTemp->setSize(10);
							particleTemp->getEmitter()->setLifetime(0.2, 0.2);
							particleTemp->getEmitter()->setSpeed(100);
							particleTemp->getEmitter()->init();
							particleTemp->setModelMatrix(GameObject::GetComponent<Transform>(enemy).UpdateGlobal());
							particles.push_back(particleTemp);

							m_score->GetComponent<ScoreHandler>().Add(1);
							m_sceneReg->destroy(enemy);
							AI.erase(AI.begin() + i);

							AudioEngine& engine = AudioEngine::Instance();
							//Create event
							AudioEvent& explosionEvent = engine.GetEvent("Explosion");

							if (!explosionEvent.isPlaying())
								explosionEvent.Play();
							else
							{
								explosionEvent.Stop();
								explosionEvent.Play();
							}
						}
						else if (type == EntityType::NEROTIST) {
							particleTemp = new ParticleController(2, GameObject::GetComponent<Transform>(enemy).GetLocalPos(), m_textures[int(TextureType::YELLOW)], enemy);
							particleTemp->setSize(10);
							particleTemp->getEmitter()->setLifetime(0.2, 0.2);
							particleTemp->getEmitter()->setSpeed(100);
							particleTemp->getEmitter()->init();
							particleTemp->setModelMatrix(GameObject::GetComponent<Transform>(enemy).UpdateGlobal());
							particles.push_back(particleTemp);

							m_score->GetComponent<ScoreHandler>().Add(5);
							m_sceneReg->destroy(enemy);
							AI.erase(AI.begin() + i);

							AudioEngine& engine = AudioEngine::Instance();
							//Create event
							AudioEvent& explosionEvent = engine.GetEvent("Explosion");

							if (!explosionEvent.isPlaying())
								explosionEvent.Play();
							else
							{
								explosionEvent.Stop();
								explosionEvent.Play();
							}
						}
						else if (type == EntityType::BOMBARDIER)
						{
							particleTemp = new ParticleController(2, GameObject::GetComponent<Transform>(enemy).GetLocalPos(), m_textures[int(TextureType::YELLOW)], enemy);
							particleTemp->setSize(10);
							particleTemp->getEmitter()->setLifetime(0.2, 0.2);
							particleTemp->getEmitter()->setSpeed(100);
							particleTemp->getEmitter()->init();
							particleTemp->setModelMatrix(GameObject::GetComponent<Transform>(enemy).UpdateGlobal());
							particles.push_back(particleTemp);

							m_score->GetComponent<ScoreHandler>().Add(10);
							m_sceneReg->destroy(enemy);
							AI.erase(AI.begin() + i);

							AudioEngine& engine = AudioEngine::Instance();
							//Create event
							AudioEvent& explosionEvent = engine.GetEvent("Explosion");

							if (!explosionEvent.isPlaying())
								explosionEvent.Play();
							else
							{
								explosionEvent.Stop();
								explosionEvent.Play();
							}
						}

						if (type == EntityType::JELLY) {
							if (!GameObject::GetComponent<JellyFishBoss>(enemy).m_isImmune) {
								GameObject::GetComponent<JellyFishBoss>(enemy).m_health--;

								glm::vec3 temp = glm::vec3(0, 30, 0);

								GameObject::GetComponent<Transform>(enemy).MoveLocalPosFixed(temp);
								GameObject::GetComponent<Transform>(enemy).SetLocalScale(glm::vec3(1.0));
								particleTemp = new ParticleController(2, GameObject::GetComponent<Transform>(enemy).GetLocalPos() + glm::vec3(0, 30, 0), m_textures[int(TextureType::YELLOW)], enemy);
								particleTemp->setSize(10);
								particleTemp->getEmitter()->setLifetime(0.2, 0.2);
								particleTemp->getEmitter()->setSpeed(100);
								particleTemp->getEmitter()->init();
								particleTemp->setModelMatrix(GameObject::GetComponent<Transform>(enemy).UpdateGlobal());
								GameObject::GetComponent<Transform>(enemy).SetLocalScale(glm::vec3(3.0));
								temp = glm::vec3(0, -30, 0);
								GameObject::GetComponent<Transform>(enemy).MoveLocalPosFixed(temp);
								particles.push_back(particleTemp);

								AudioEngine& engine = AudioEngine::Instance();
								//Create event
								AudioEvent& explosionEvent = engine.GetEvent("Explosion");

								if (!explosionEvent.isPlaying())
									explosionEvent.Play();
								else
								{
									explosionEvent.Stop();
									explosionEvent.Play();
								}
							}
							if (GameObject::GetComponent<JellyFishBoss>(enemy).m_health <= 0) {

								if (m_name == "Universe_19") {
									auto tempEnt = GameObject::Allocate();
									tempEnt->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 0));
									tempEnt->GetComponent<Transform>().SetWHD(glm::vec3(10, 30, 5));
									tempEnt->AttachComponent<EntityType>() = EntityType::PORTAL;
									PortalID = tempEnt->GetID();

									//PORTAL
									particleTemp = new ParticleController(3, glm::vec3(0, 0, 0), m_textures[int(TextureType::SKY27)], tempEnt->GetID());
									particleTemp->getEmitter()->setRadius(30);
									particleTemp->setSize(5);
									particleTemp->getEmitter()->setLifetime(0.1f, 1.5f);
									particleTemp->getEmitter()->setSpeed(0.5);
									particleTemp->getEmitter()->init();
									particles.push_back(particleTemp);
									glm::vec3 tempv3 = GameObject::GetComponent<Transform>(enemy).GetLocalPos() + glm::vec3(0, 30, 0);
									GameObject::GetComponent<Transform>(PortalID).MoveLocalPos(tempv3);


								}

								m_isBossDead = true;

								m_sceneReg->destroy(enemy);
								AI.erase(AI.begin() + i);
							}
						}
						else if (type == EntityType::CENTIPEDE) {
							if (!GameObject::GetComponent<CentipedeBoss>(enemy).m_isImmune)
							{
								GameObject::GetComponent<CentipedeBoss>(enemy).m_health--;

								particleTemp = new ParticleController(2, GameObject::GetComponent<Transform>(enemy).GetLocalPos(), m_textures[int(TextureType::YELLOW)], enemy);
								particleTemp->setSize(10);
								particleTemp->getEmitter()->setLifetime(0.3, 0.3);
								particleTemp->getEmitter()->setSpeed(100);
								particleTemp->getEmitter()->init();
								particleTemp->setModelMatrix(GameObject::GetComponent<Transform>(enemy).UpdateGlobal());
								particles.push_back(particleTemp);
							}
							AudioEngine& engine = AudioEngine::Instance();
							//Create event
							AudioEvent& explosionEvent = engine.GetEvent("Explosion");

							if (!explosionEvent.isPlaying())
								explosionEvent.Play();
							else
							{
								explosionEvent.Stop();
								explosionEvent.Play();
							}
							if (GameObject::GetComponent<CentipedeBoss>(enemy).m_health <= 0) {

								if (m_name == "Universe_27") {
									auto tempEnt = GameObject::Allocate();
									tempEnt->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 0));
									tempEnt->GetComponent<Transform>().SetWHD(glm::vec3(10, 30, 5));
									tempEnt->AttachComponent<EntityType>() = EntityType::PORTAL;
									PortalID = tempEnt->GetID();

									//PORTAL
									particleTemp = new ParticleController(3, glm::vec3(0, 0, 0), m_textures[int(TextureType::SKY5)], tempEnt->GetID());
									particleTemp->getEmitter()->setRadius(30);
									particleTemp->setSize(5);
									particleTemp->getEmitter()->setLifetime(0.1f, 1.5f);
									particleTemp->getEmitter()->setSpeed(0.5);
									particleTemp->getEmitter()->init();
									particles.push_back(particleTemp);
									glm::vec3 tempv3 = GameObject::GetComponent<Transform>(enemy).GetLocalPos();
									GameObject::GetComponent<Transform>(PortalID).MoveLocalPos(tempv3);
								}
								m_isBossDead = true;


								m_sceneReg->destroy(enemy);
								AI.erase(AI.begin() + i);
							}


						}

						else if (type == EntityType::HIVEMIND) {
							if (!GameObject::GetComponent<HiveMindBoss>(enemy).m_isImmune)
							{
								GameObject::GetComponent<HiveMindBoss>(enemy).m_health--;

								particleTemp = new ParticleController(2, GameObject::GetComponent<Transform>(enemy).GetLocalPos(), m_textures[int(TextureType::YELLOW)], enemy);
								particleTemp->setSize(10);
								particleTemp->getEmitter()->setLifetime(0.3, 0.3);
								particleTemp->getEmitter()->setSpeed(100);
								particleTemp->getEmitter()->init();
								particleTemp->setModelMatrix(GameObject::GetComponent<Transform>(enemy).UpdateGlobal());
								particles.push_back(particleTemp);
							}
							AudioEngine& engine = AudioEngine::Instance();
							//Create event
							AudioEvent& explosionEvent = engine.GetEvent("Explosion");

							if (!explosionEvent.isPlaying())
								explosionEvent.Play();
							else
							{
								explosionEvent.Stop();
								explosionEvent.Play();
							}
							if (GameObject::GetComponent<HiveMindBoss>(enemy).m_health <= 0) {

								m_isBossDead = true;
								m_sceneReg->destroy(enemy);
								AI.erase(AI.begin() + i);

								*switchIt = true;
								*SceneNo = int(ScenesNum::WIN);
							}
						}

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
			////collisions of player with planets
			//for (int i = Universe19SS::SOLARI; i <= Universe19SS::KEMINTH; i++) {
			//	if (isBoxCircleCollide(GameObject::GetComponent<Transform>(MainPlayerID), GameObject::GetComponent<Transform>(m_solarSystem[i]))) {
			//		m_PlayerHealth = 0;
			//		GameObject::GetComponent<AnimationHandler>(health).SetActiveAnim(m_PlayerHealth);
			//	}
			//}

			if (m_isBossDead)
			{
				if (isBoxCollide(GameObject::GetComponent<Transform>(MainPlayerID), GameObject::GetComponent<Transform>(PortalID)))
				{
					isTransitionActive = true;
				}
			}

			if (m_score->GetComponent<ScoreHandler>().GetScore() >= 300 && !m_isBossSpawn)
			{
				//// JELLYFIH BOSS
				{
					AudioEngine& engine = AudioEngine::Instance();
					//Create event
					AudioEvent& announcement = engine.GetEvent("JellyFishAnnouncement");
					announcement.Play();

					auto jellyEntity = GameObject::Allocate();
					jellyEntity->AttachComponent<JellyFishBoss>().m_enemyMesh = m_meshes[int(EnemyMesh::NEROTISTU1)];
					jellyEntity->GetComponent<JellyFishBoss>().Init(jellyEntity->GetID(), MainPlayerID);

					jellyEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0));
					auto& jellypos = jellyEntity->GetComponent<Transform>();

					particleTemp = new ParticleController(1, glm::vec3(jellypos.GetLocalPos().x - 6, jellypos.GetLocalPos().y + 13.0, jellypos.GetLocalPos().z + 6), m_textures[int(TextureType::YELLOW)], m_textures[int(TextureType::FIRE)], jellyEntity->GetID());
					particleTemp->setRotation(glm::vec3(90, 0, 0));
					particleTemp->getEmitter()->setRadius(1.0);
					particleTemp->setSize(2);
					particleTemp->getEmitter()->setAngleDeg(10);
					particleTemp->getEmitter()->setLifetime(0.1f, 1.5f);
					particleTemp->getEmitter()->setSpeed(10);
					particleTemp->getEmitter()->init();
					particles.push_back(particleTemp);
					jellyEntity->GetComponent<JellyFishBoss>().m_particles.push_back(particleTemp);

					particleTemp = new ParticleController(1, glm::vec3(jellypos.GetLocalPos().x + 6, jellypos.GetLocalPos().y + 13.0, jellypos.GetLocalPos().z + 6), m_textures[int(TextureType::YELLOW)], m_textures[int(TextureType::FIRE)], jellyEntity->GetID());
					particleTemp->setRotation(glm::vec3(90, 0, 0));
					particleTemp->getEmitter()->setRadius(1.0);
					particleTemp->setSize(2);
					particleTemp->getEmitter()->setAngleDeg(10);
					particleTemp->getEmitter()->setLifetime(0.1f, 1.5f);
					particleTemp->getEmitter()->setSpeed(10);
					particleTemp->getEmitter()->init();
					particles.push_back(particleTemp);
					jellyEntity->GetComponent<JellyFishBoss>().m_particles.push_back(particleTemp);

					particleTemp = new ParticleController(1, glm::vec3(jellypos.GetLocalPos().x - 6, jellypos.GetLocalPos().y + 13.0, jellypos.GetLocalPos().z - 6), m_textures[int(TextureType::YELLOW)], m_textures[int(TextureType::FIRE)], jellyEntity->GetID());
					particleTemp->setRotation(glm::vec3(90, 0, 0));
					particleTemp->getEmitter()->setRadius(1.0);
					particleTemp->setSize(2);
					particleTemp->getEmitter()->setAngleDeg(10);
					particleTemp->getEmitter()->setLifetime(0.1f, 1.5f);
					particleTemp->getEmitter()->setSpeed(10);
					particleTemp->getEmitter()->init();
					particles.push_back(particleTemp);
					jellyEntity->GetComponent<JellyFishBoss>().m_particles.push_back(particleTemp);

					particleTemp = new ParticleController(1, glm::vec3(jellypos.GetLocalPos().x + 6, jellypos.GetLocalPos().y + 13.0, jellypos.GetLocalPos().z - 6), m_textures[int(TextureType::YELLOW)], m_textures[int(TextureType::FIRE)], jellyEntity->GetID());
					particleTemp->setRotation(glm::vec3(90, 0, 0));
					particleTemp->getEmitter()->setRadius(1.0);
					particleTemp->setSize(2);
					particleTemp->getEmitter()->setAngleDeg(10);
					particleTemp->getEmitter()->setLifetime(0.1f, 1.5f);
					particleTemp->getEmitter()->setSpeed(10);
					particleTemp->getEmitter()->init();
					particles.push_back(particleTemp);
					jellyEntity->GetComponent<JellyFishBoss>().m_particles.push_back(particleTemp);

					glm::vec3 temp = glm::vec3(0, -30, 2500);
					jellypos.MoveLocalPos(temp);

					jellyEntity->AttachComponent<DynamicRenderer>(CamID, jellyEntity->GetID(), *jellyEntity->GetComponent<JellyFishBoss>().m_meshes[0], nullptr, false);
					jellyEntity->AttachComponent<MorphAnimController>(int(jellyEntity->GetID())).SetFrames(jellyEntity->GetComponent<JellyFishBoss>().m_meshes, 0, 24);
					jellyEntity->GetComponent<Transform>().SetLocalScale(glm::vec3(3.0));
					jellyEntity->GetComponent<Transform>().SetWHD(glm::vec3(jellyEntity->GetComponent<JellyFishBoss>().m_meshes[0]->GetWidth(), jellyEntity->GetComponent<JellyFishBoss>().m_meshes[0]->GetHeight() * 3, jellyEntity->GetComponent<JellyFishBoss>().m_meshes[0]->GetDepth()));
					jellyEntity->GetComponent<Transform>().SetRadius((jellyEntity->GetComponent<JellyFishBoss>().m_meshes[0]->GetWidth() * 3) / 2);
					jellyEntity->AttachComponent<EntityType>() = EntityType::JELLY;

					m_isBossSpawn = true;
				}

				//arrow to direct to the boss
				{
					m_arrowTotheBoss = GameObject::Allocate();

					m_arrowTotheBoss->AttachComponent<Transform>().SetLocalPos(glm::vec3(0));
					m_arrowTotheBoss->AttachComponent<StaticRenderer>(CamID, m_arrowTotheBoss->GetID(), *m_meshes[int(PlanetMesh::ARROW)], nullptr);
				}
			}
		}

		else if (m_name == "Universe_27")
		{
			////collisions of player with planets
			//for (int i = Universe27SS::LUTERO; i <= Universe27SS::MEGAANTU; i++) {
			//	if (isBoxCircleCollide(GameObject::GetComponent<Transform>(MainPlayerID), GameObject::GetComponent<Transform>(m_solarSystem[i]))) {
			//		m_PlayerHealth = 0;
			//		GameObject::GetComponent<AnimationHandler>(health).SetActiveAnim(m_PlayerHealth);
			//	}
			//}

			if (m_isBossDead)
			{
				if (isBoxCollide(GameObject::GetComponent<Transform>(MainPlayerID), GameObject::GetComponent<Transform>(PortalID)))
				{
					isTransitionActive = true;
				}
			}

			if (wasSceneSwitched)
				wasTransitionActive = true;

			if (m_score->GetComponent<ScoreHandler>().GetScore() >= 600 && !m_isBossSpawn)
			{
				////BOSS
				AudioEngine& engine = AudioEngine::Instance();
				//Create event
				AudioEvent& announcement = engine.GetEvent("CentipedeAnnouncement");
				announcement.Play();

				auto Centipede = GameObject::Allocate();
				Centipede->AttachComponent<CentipedeBoss>().SetBulletMesh(m_meshes[int(PlayerMesh::PLAYERBULLET)]);
				Centipede->GetComponent<CentipedeBoss>().Init(Centipede->GetID(), MainPlayerID);

				Centipede->AttachComponent<Transform>().SetLocalPos(glm::vec3(0));
				auto& Centipedeypos = Centipede->GetComponent<Transform>();

				glm::vec3 temp = glm::vec3(0, 0, 2000);

				Centipedeypos.MoveLocalPos(temp);

				Centipede->AttachComponent<DynamicRenderer>(CamID, Centipede->GetID(), *Centipede->GetComponent<CentipedeBoss>().m_meshes[0], nullptr, false);
				Centipede->AttachComponent<MorphAnimController>(int(Centipede->GetID())).SetFrames(Centipede->GetComponent<CentipedeBoss>().m_meshes, 0, 4);
				Centipede->GetComponent<Transform>().SetLocalScale(glm::vec3(3.0));
				Centipede->GetComponent<Transform>().SetWHD(glm::vec3(3 * Centipede->GetComponent<CentipedeBoss>().m_meshes[0]->GetWidth(), 3 * Centipede->GetComponent<CentipedeBoss>().m_meshes[0]->GetHeight(), 3 * Centipede->GetComponent<CentipedeBoss>().m_meshes[0]->GetDepth()));
				Centipede->GetComponent<Transform>().SetRadius((Centipede->GetComponent<CentipedeBoss>().m_meshes[0]->GetWidth() * 3) / 2);
				Centipede->AttachComponent<EntityType>() = EntityType::CENTIPEDE;

				m_isBossSpawn = true;
				{
					m_arrowTotheBoss = GameObject::Allocate();

					m_arrowTotheBoss->AttachComponent<Transform>().SetLocalPos(glm::vec3(0));
					m_arrowTotheBoss->AttachComponent<StaticRenderer>(CamID, m_arrowTotheBoss->GetID(), *m_meshes[int(PlanetMesh::ARROW)], nullptr);
				}
			}
		}

		else if (m_name == "Universe_5")
		{
			////collisions of player with planets
			//for (int i = Universe5SS::RALEI; i <= Universe5SS::WHITEP; i++) {
			//	if (isBoxCircleCollide(GameObject::GetComponent<Transform>(MainPlayerID), GameObject::GetComponent<Transform>(m_solarSystem[i]))) {
			//		m_PlayerHealth = 0;
			//		GameObject::GetComponent<AnimationHandler>(health).SetActiveAnim(m_PlayerHealth);
			//	}
			//}	

			if (m_score->GetComponent<ScoreHandler>().GetScore() >= 900 && !m_isBossSpawn)
			{
				AudioEngine& engine = AudioEngine::Instance();
				//Create event
				AudioEvent& announcement = engine.GetEvent("HivemindAnnouncement");
				announcement.Play();

				auto HiveMind = GameObject::Allocate();
				HiveMind->AttachComponent<HiveMindBoss>().SetBulletMesh(m_meshes[int(PlayerMesh::PLAYERBULLET)]);
				HiveMind->GetComponent<HiveMindBoss>().Init(HiveMind->GetID(), MainPlayerID);

				HiveMind->AttachComponent<Transform>().SetLocalPos(glm::vec3(0));
				auto& Centipedeypos = HiveMind->GetComponent<Transform>();

				glm::vec3 temp = glm::vec3(0, 0, 2000);

				Centipedeypos.MoveLocalPos(temp);

				HiveMind->AttachComponent<DynamicRenderer>(CamID, HiveMind->GetID(), *HiveMind->GetComponent<HiveMindBoss>().m_meshes[0], nullptr, false);
				HiveMind->AttachComponent<MorphAnimController>(int(HiveMind->GetID())).SetFrames(HiveMind->GetComponent<HiveMindBoss>().m_meshes, 0, 3);
				HiveMind->GetComponent<Transform>().SetLocalScale(glm::vec3(10.0));
				HiveMind->GetComponent<Transform>().SetWHD(glm::vec3(10 * HiveMind->GetComponent<HiveMindBoss>().m_meshes[0]->GetWidth(), 10 * HiveMind->GetComponent<HiveMindBoss>().m_meshes[0]->GetHeight() * 3, 10 * HiveMind->GetComponent<HiveMindBoss>().m_meshes[0]->GetDepth()));
				HiveMind->GetComponent<Transform>().SetRadius((HiveMind->GetComponent<HiveMindBoss>().m_meshes[0]->GetWidth() * 3) / 2);
				HiveMind->AttachComponent<EntityType>() = EntityType::HIVEMIND;

				m_isBossSpawn = true;
				{
					m_arrowTotheBoss = GameObject::Allocate();

					m_arrowTotheBoss->AttachComponent<Transform>().SetLocalPos(glm::vec3(0));
					m_arrowTotheBoss->AttachComponent<StaticRenderer>(CamID, m_arrowTotheBoss->GetID(), *m_meshes[int(PlanetMesh::ARROW)], nullptr);
				}
			}


		}

#pragma endregion

		if (m_PlayerHealth == 1)
		{
			AudioEngine& engine = AudioEngine::Instance();

			//Create event
			AudioEvent& hp = engine.GetEvent("Low HP");

			hp.Play();
		}
	}

}

void Universe::Render(float deltaTime)
{
	BufferEntity->GetComponent<PostEffect>().Clear();
	BufferEntity->GetComponent<PixelationEffect>().Clear();

	if (isTransitionActive)
	{

		// Basic Effect //
		BufferEntity->GetComponent<PostEffect>().BindBuffer(0);
		{
			m_sceneReg->view<StaticRenderer>().each([=](StaticRenderer& renderer) { renderer.Draw(); });
			m_sceneReg->view<DynamicRenderer>().each([=](DynamicRenderer& renderer) { renderer.Draw(); });

			Skybox::Draw(camera->GetView(), camera->GetProj());

			// Particles
			{
				for (auto& i : this->particles)
					i->draw();
			}

			m_sceneReg->view<Sprite2D>().each([=](Sprite2D& renderer) {	renderer.Draw(camera); });
		}
		BufferEntity->GetComponent<PostEffect>().UnbindBuffer();

		if (isTransitionActive) {
			BufferEntity->GetComponent<PixelationEffect>().SetIntensity(intensity);
			BufferEntity->GetComponent<PixelationEffect>().ApplyEffect(&BufferEntity->GetComponent<PostEffect>());
			BufferEntity->GetComponent<PixelationEffect>().DrawToScreen();
		}
		else {
			BufferEntity->GetComponent<PostEffect>().DrawToScreen();
		}
	}
	else
	{
		m_sceneReg->view<StaticRenderer>().each([=](StaticRenderer& renderer) { renderer.Draw(); });
		m_sceneReg->view<DynamicRenderer>().each([=](DynamicRenderer& renderer) { renderer.Draw(); });

		Skybox::Draw(camera->GetView(), camera->GetProj());

		// Particles
		{
			for (auto& i : this->particles)
				i->draw();
		}

		m_sceneReg->view<Sprite2D>().each([=](Sprite2D& renderer) {	renderer.Draw(camera); });
	}
}

void Universe::KeyInput()
{
	AudioEngine& engine = AudioEngine::Instance();

	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		*switchIt = true;
		*SceneNo = int(ScenesNum::PAUSE_MENU);
	}

	// Player Movement //
	auto& playerEnt = GameObject::GetComponent<Transform>(MainPlayerID);
	auto& camEnt = GameObject::GetComponent<Transform>(CamID);
	auto& moonEnt = GameObject::GetComponent<Transform>(MoonID);
	glm::vec3 temp;

	if (!isDodge)
	{


		if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			temp = glm::vec3(0, 0, -4);
			playerEnt.MoveLocalPos(temp);
			if (!isWingOpen && !GameObject::GetComponent<MorphAnimController>(MainPlayerID).getAnimate())
			{
				GameObject::GetComponent<MorphAnimController>(MainPlayerID).SetAnimate(true);
				isPlayerAnim = true;
				isWingOpen = true;
			}
		}
		else if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_RELEASE)
		{
			if (isWingOpen && !GameObject::GetComponent<MorphAnimController>(MainPlayerID).getAnimate())
			{
				GameObject::GetComponent<MorphAnimController>(MainPlayerID).SetAnimate(true);
				isPlayerAnim = true;
				isWingOpen = false;
			}
		}
		else if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			temp = glm::vec3(0, 0, 4);
			playerEnt.MoveLocalPos(temp);
		}
		if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			temp = glm::vec3(0.0f, 2.5f, 0.0f);
			playerEnt.RotateLocal(temp);
		}
		else if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			temp = glm::vec3(0.0f, -2.5f, 0.0f);
			playerEnt.RotateLocal(temp);
		}

	}
	else if (isDodge)
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

			//Create event
			AudioEvent& dodgeEvent = engine.GetEvent("Blink Dodge");


			if (!dodgeEvent.isPlaying())
				dodgeEvent.Play();
			else
			{
				dodgeEvent.Stop();
				dodgeEvent.Play();
			}
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

	if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		tempDir = -1;
		isDodge = true;
	}
	else if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		tempDir = 1;
		isDodge = true;
	}

	if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		tempDir = -1;
		isDodge = true;
	}
	else if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		tempDir = 1;
		isDodge = true;
	}


	if (glfwGetKey(m_window, GLFW_KEY_F) == GLFW_PRESS && !isRotate)
	{
		glm::vec3 temp = glm::vec3(0, 180, 0);
		camEnt.SetLocalRot(temp);
		temp = glm::vec3(0, 13, -35);
		camEnt.SetLocalPos(temp);
		isRotate = true;
	}
	else if (glfwGetKey(m_window, GLFW_KEY_F) == GLFW_RELEASE && isRotate) {
		glm::vec3 temp = glm::vec3(0, 0, 0);
		camEnt.SetLocalRot(temp);
		temp = glm::vec3(0, 13, 25);
		camEnt.SetLocalPos(temp);
		isRotate = false;
	}


	if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{

		if (glfwGetTime() - m_startTime >= m_fireRate) {

			//Create event
			AudioEvent& shoot = engine.GetEvent("Playershooting");


			if (!shoot.isPlaying())
				shoot.Play();
			else
			{
				shoot.Stop();
				shoot.Play();
			}

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

	engine.Update();
}

void Universe::GamepadInput()
{
	AudioEngine& engine = AudioEngine::Instance();

	if (gamepad.getGamepadInput()) {
		auto& playerEnt = GameObject::GetComponent<Transform>(MainPlayerID);
		auto& camEnt = GameObject::GetComponent<Transform>(CamID);
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
				//Left Trigger LOOK BACK
				{

					if (gamepad.trigger.LT >= -1 && gamepad.trigger.LT < -0.8) {
						glm::vec3 temp = glm::vec3(0, 0, 0);
						camEnt.SetLocalRot(temp);
						temp = glm::vec3(0, 13, 25);
						camEnt.SetLocalPos(temp);
						isRotate = false;

					}
					else if (gamepad.trigger.LT > 0.8) {
						glm::vec3 temp = glm::vec3(0, 180, 0);
						camEnt.SetLocalRot(temp);
						temp = glm::vec3(0, 13, -35);
						camEnt.SetLocalPos(temp);
						isRotate = true;
					}

				}

				//Right Trigger SHOOTING
				{

					if (gamepad.trigger.RT > -0.8) {
						if (glfwGetTime() - m_startTime >= m_fireRate) {

							//Create event
							AudioEvent& shoot = engine.GetEvent("Playershooting");


							if (!shoot.isPlaying())
								shoot.Play();
							else
							{
								shoot.Stop();
								shoot.Play();
							}

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

						if (isWingOpen && !GameObject::GetComponent<MorphAnimController>(MainPlayerID).getAnimate())
						{
							GameObject::GetComponent<MorphAnimController>(MainPlayerID).SetAnimate(true);
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

						if (!isWingOpen && !GameObject::GetComponent<MorphAnimController>(MainPlayerID).getAnimate())
						{
							GameObject::GetComponent<MorphAnimController>(MainPlayerID).SetAnimate(true);
							isPlayerAnim = true;
							isWingOpen = true;
						}
					}


					particles[ParticleName::PLAYER_CENTER_LEFT]->getEmitter()->setSpeed(tempSpeed);
					particles[ParticleName::PLAYER_CENTER_RIGHT]->getEmitter()->setSpeed(tempSpeed);
				}
			}
			else if (isDodge)
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

					//Create event
					AudioEvent& dodgeEvent = engine.GetEvent("Blink Dodge");


					if (!dodgeEvent.isPlaying())
						dodgeEvent.Play();
					else
					{
						dodgeEvent.Stop();
						dodgeEvent.Play();
					}
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
		// Rotation
		GameObject::GetComponent<Transform>(m_solarSystem[Universe27SS::LUTERO]).RotateLocal(sunRotation);
		GameObject::GetComponent<Transform>(m_solarSystem[Universe27SS::DEDMOS]).RotateLocal(planetRotation);
		GameObject::GetComponent<Transform>(m_solarSystem[Universe27SS::TITANIUS]).RotateLocal(planetRotation);
		GameObject::GetComponent<Transform>(m_solarSystem[Universe27SS::KREILLO]).RotateLocal(planetRotation);
		GameObject::GetComponent<Transform>(m_solarSystem[Universe27SS::PAXALLUS]).RotateLocal(planetRotation);
		GameObject::GetComponent<Transform>(m_solarSystem[Universe27SS::DERANGI]).RotateLocal(planetRotation);
		GameObject::GetComponent<Transform>(m_solarSystem[Universe27SS::RHETOID]).RotateLocal(planetRotation);
		GameObject::GetComponent<Transform>(m_solarSystem[Universe27SS::MEGAANTU]).RotateLocal(planetRotation);

		// Orbit
		GameObject::GetComponent<Transform>(m_solarSystem[Universe27SS::LDMC]).RotateLocal(moonOrbit);
		GameObject::GetComponent<Transform>(m_solarSystem[Universe27SS::LDRC]).RotateLocal(verastenOrbit);
		GameObject::GetComponent<Transform>(m_solarSystem[Universe27SS::LKC]).RotateLocal(yechinOrbit);
		GameObject::GetComponent<Transform>(m_solarSystem[Universe27SS::LMC]).RotateLocal(kerantiaOrbit);
		GameObject::GetComponent<Transform>(m_solarSystem[Universe27SS::LPC]).RotateLocal(gueristisOrbit);
		GameObject::GetComponent<Transform>(m_solarSystem[Universe27SS::LRC]).RotateLocal(keminthOrbit);
		GameObject::GetComponent<Transform>(m_solarSystem[Universe27SS::LTC]).RotateLocal(keminthOrbit);

		GameObject::GetComponent<Transform>(AsteroidBeltID).RotateLocal(asteroidOrbit);
	}
	else if (m_name == "Universe_5") {
		// Rotation
		GameObject::GetComponent<Transform>(m_solarSystem[Universe5SS::RALEI]).RotateLocal(sunRotation);
		GameObject::GetComponent<Transform>(m_solarSystem[Universe5SS::DARKP]).RotateLocal(planetRotation);
		GameObject::GetComponent<Transform>(m_solarSystem[Universe5SS::DUALP]).RotateLocal(planetRotation);
		GameObject::GetComponent<Transform>(m_solarSystem[Universe5SS::PINKP]).RotateLocal(planetRotation);
		GameObject::GetComponent<Transform>(m_solarSystem[Universe5SS::SPIKEP]).RotateLocal(planetRotation);
		GameObject::GetComponent<Transform>(m_solarSystem[Universe5SS::WHITEP]).RotateLocal(planetRotation);

		// Orbit
		GameObject::GetComponent<Transform>(m_solarSystem[Universe5SS::RDKC]).RotateLocal(moonOrbit);
		GameObject::GetComponent<Transform>(m_solarSystem[Universe5SS::RDLC]).RotateLocal(verastenOrbit);
		GameObject::GetComponent<Transform>(m_solarSystem[Universe5SS::RPC]).RotateLocal(yechinOrbit);
		GameObject::GetComponent<Transform>(m_solarSystem[Universe5SS::RSC]).RotateLocal(kerantiaOrbit);
		GameObject::GetComponent<Transform>(m_solarSystem[Universe5SS::RWC]).RotateLocal(gueristisOrbit);

		GameObject::GetComponent<Transform>(AsteroidBeltID).RotateLocal(asteroidOrbit);
	}
}

int Universe::GetSceneScore()
{
	return m_score->GetComponent<ScoreHandler>().GetScore();
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
		&& objCircle.GetLocalPos().x + objCircle.GetRadius() >= objBox.GetLocalPos().x;

	bool collisionZ = objBox.GetLocalPos().z + objBox.GetWHD().z >= objCircle.GetLocalPos().z
		&& objCircle.GetLocalPos().z + objCircle.GetRadius() >= objBox.GetLocalPos().z;

	return collisionX && collisionZ;
}
