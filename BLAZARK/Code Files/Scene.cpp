#include "Scene.h"


enum class EntityType { PLAYER, ENEMY };

enum class TextureType { START = 3 };

enum class PlayerMesh { PLAYERSHIPPENCIL, PLAYERSHIPBAT };

enum class PlanetMesh { SOLARI = 2, VERASTEN, YECHIN, KERANTIA, LUNARI, GUERISTIS, KEMINTH, SUN2, NP1, NP2, NP3, GP1, GP2, MP, CITADEL };

std::unique_ptr<GameObject> effect;

vector<Mesh*> Scene::m_meshes;
vector<Texture*> Scene::m_textures;

bool texTglPressed = false;

StaticRenderer tempEnemy;

Scene::Scene(string name)
	:m_name(name)
{

	if (m_textures.size() < 1) {
		// HUD //
		m_textures.push_back(new Texture("Resource Files/Textures/HUD/Health_4-4.png"));
		m_textures.push_back(new Texture("Resource Files/Textures/HUD/Abilities_Unavailable_Temp.png"));
		m_textures.push_back(new Texture("Resource Files/Textures/HUD/PowerUP_Unavailable_Temp.png"));
		/////////
		m_textures.push_back(new Texture("Resource Files/Textures/Menu/TitleScreen.png"));
		m_textures.push_back(new Texture("Resource Files/Textures/HUD/health_spritesheet.png"));
	}
	if (m_meshes.size() < 1) {

		//Universe-19
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Ships/PlayerShips/PlayerShipPencil.obj", *m_meshes[int(PlayerMesh::PLAYERSHIPPENCIL)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Ships/PlayerShips/PlayerShipBat.obj", *m_meshes[int(PlayerMesh::PLAYERSHIPBAT)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/UniverseOne/Planets/Solari.obj", *m_meshes[int(PlanetMesh::SOLARI)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/UniverseOne/Planets/Verasten.obj", *m_meshes[int(PlanetMesh::VERASTEN)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/UniverseOne/Planets/Yechin.obj", *m_meshes[int(PlanetMesh::YECHIN)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/UniverseOne/Planets/Kerantia.obj", *m_meshes[int(PlanetMesh::KERANTIA)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/UniverseOne/Planets/Lunari.obj", *m_meshes[int(PlanetMesh::LUNARI)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/UniverseOne/Planets/Gueristis.obj", *m_meshes[int(PlanetMesh::GUERISTIS)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/UniverseOne/Planets/Keminth.obj", *m_meshes[int(PlanetMesh::KEMINTH)]);
		
		//Universe-27
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Universe27/Sun.obj", *m_meshes[int(PlanetMesh::SUN2)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Universe27/Null_Planet1.obj", *m_meshes[int(PlanetMesh::NP1)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Universe27/Null_Planet2.obj", *m_meshes[int(PlanetMesh::NP2)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Universe27/Null_Planet3.obj", *m_meshes[int(PlanetMesh::NP3)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Universe27/Gas Planet 1.obj", *m_meshes[int(PlanetMesh::GP1)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Universe27/Gas Planet 2.obj", *m_meshes[int(PlanetMesh::GP2)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Universe27/Mushroom Planet.obj", *m_meshes[int(PlanetMesh::MP)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Universe27/Citadel.obj", *m_meshes[int(PlanetMesh::CITADEL)]);
	
	}
}

string Scene::GetName()
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

Menu::Menu(string name, unsigned int* num, bool* change)
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
		camera = &cameraEntity->AttachComponent<Camera>(cameraEntity->GetID());
		cameraEntity->GetComponent<Transform>().SetLocalPos(glm::vec3(0.0f, 0.0f, 100.0f));

		if (m_name == "Start_Screen") {
			auto titleScreen = GameObject::Allocate();
			titleScreen->AttachComponent<Sprite2D>(m_textures[int(TextureType::START)], titleScreen->GetID(), 100, 100);
			titleScreen->AttachComponent<Transform>();
			//titleScreen->GetComponent<Transform>().SetLocalScale(temp);
		}
		else if (m_name == "Main_Menu") {

		}
		else if (m_name == "Pause_Menu") {

		}
	}
}

void Menu::Update(float deltaTime)
{
	// Transform Update
	m_sceneReg->view<Transform>().each([=](Transform& transform) {	transform.UpdateGlobal();});

	// Key Input
	KeyInput();

	// Camera Update
	camera->Update();
}

void Menu::KeyInput()
{
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(m_window, GLFW_TRUE);

	if (glfwGetKey(m_window, GLFW_KEY_ENTER) == GLFW_PRESS) {
		*switchIt = true;
		*SceneNo = int(ScenesNum::UNIVERSE_19);
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
		if (gamepad.trigger.LT > 0 && gamepad.trigger.LT < 1) {

		}

		else if (gamepad.trigger.LT > -1 && gamepad.trigger.LT < 0) {

		}

		if (gamepad.trigger.RT > 0 && gamepad.trigger.RT < 1) {

		}

		else if (gamepad.trigger.RT > -1 && gamepad.trigger.RT < 0) {

		}

		//axes input
		if (gamepad.axes[Direction::LEFT].X > 0 && gamepad.axes[Direction::LEFT].X < 1) {

		}
		else if (gamepad.axes[Direction::LEFT].X > -1 && gamepad.axes[Direction::LEFT].X < 0) {

		}

		if (gamepad.axes[Direction::LEFT].Y > 0 && gamepad.axes[Direction::LEFT].Y < 1) {

		}
		else if (gamepad.axes[Direction::LEFT].Y > -1 && gamepad.axes[Direction::LEFT].Y < 0) {

		}

		if (gamepad.axes[Direction::RIGHT].X > 0 && gamepad.axes[Direction::RIGHT].X < 1) {

		}
		else if (gamepad.axes[Direction::RIGHT].X > -1 && gamepad.axes[Direction::RIGHT].X < 0) {

		}

		if (gamepad.axes[Direction::RIGHT].Y > 0 && gamepad.axes[Direction::RIGHT].Y < 1) {

		}
		else if (gamepad.axes[Direction::RIGHT].Y > -1 && gamepad.axes[Direction::RIGHT].Y < 0) {

		}
	}

	else
		std::cout << "No controller connected" << std::endl;

	

}

void Menu::Render(float deltaTime)
{
	m_sceneReg->view<Sprite2D>().each([=](Sprite2D& render) { render.Draw(camera); });
}

Universe::Universe(string name, unsigned int* num, bool* change)
	:Scene(name)
{
	SceneNo = num;
	switchIt = change;
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
		cameraEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 13, 25));
		camera = &cameraEntity->AttachComponent<Camera>(cameraEntity->GetID());
		camera->PerspectiveProj(0.1f, 1000.0f, Application::GetWindowWidth() / Application::GetWindowHeight(), 1.0f);

		// Player
		auto playerEntity = GameObject::Allocate();
		entt::entity MainPlayerID;
		MainPlayerID = playerEntity->GetID();
		m_entities.push_back(MainPlayerID);
		playerEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(1000, 0, 3750));
		playerEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), MainPlayerID, *m_meshes[int(PlayerMesh::PLAYERSHIPPENCIL)], nullptr);
		//playerEntity->GetComponent<Transform>().SetLocalScale(glm::vec3(0.75));
		playerEntity->GetComponent<Transform>().SetLocalRot(0, 180, 0);
		
		playerEntity->GetComponent<Transform>().SetWHD(glm::vec3(m_meshes[int(PlayerMesh::PLAYERSHIPPENCIL)]->GetWidth(), m_meshes[int(PlayerMesh::PLAYERSHIPPENCIL)]->GetHeight(), m_meshes[int(PlayerMesh::PLAYERSHIPPENCIL)]->GetDepth()));

		//HUD
		auto health = GameObject::Allocate();

		auto* tempAnim = &health->AttachComponent<AnimationHandler>();
		auto& anim = health->GetComponent<AnimationHandler>();
		anim.InitUVS(m_textures[4]);
		Animation2D Oneclip;
		Oneclip.AddFrame(UVS(glm::vec2(1, 233), glm::vec2(235, 1)));
		Oneclip.AddFrame(UVS(glm::vec2(236, 233), glm::vec2(470, 1)));
		Oneclip.AddFrame(UVS(glm::vec2(471, 233), glm::vec2(705, 1)));
		Oneclip.AddFrame(UVS(glm::vec2(706, 233), glm::vec2(939, 1)));
		Oneclip.SetIsRepeating(true);
		Oneclip.SetSecPerFrame(0.1);

		anim.AddAnimation(Oneclip);
		anim.SetActiveAnim(0);

		health->AttachComponent<Sprite2D>(m_textures[4], health->GetID(), 15, 15, true, tempAnim);
		health->AttachComponent<Transform>().SetLocalPos(glm::vec3(-80, -80, -10));

		auto abilities = GameObject::Allocate();
		abilities->AttachComponent<Sprite2D>(m_textures[1], abilities->GetID(), 15, 15);
		abilities->AttachComponent<Transform>().SetLocalPos(glm::vec3(80, -80, -10));
		auto powerUp = GameObject::Allocate();
		powerUp->AttachComponent<Sprite2D>(m_textures[2], powerUp->GetID(), 6, 45);
		powerUp->AttachComponent<Transform>().SetLocalPos(glm::vec3(-90, 10, -10));

		//effects
		/*effect = GameObject::Allocate();
		effect->AttachComponent<PostEffect>().Init(Application::GetWindowWidth(), Application::GetWindowHeight());*/
		/*effect->AttachComponent<GreyscaleEffect>().Init(Application::GetWindowWidth(), Application::GetWindowHeight());
		effect->AttachComponent<SepiaEffect>().Init(Application::GetWindowWidth(), Application::GetWindowHeight());
		effect->AttachComponent<ColorCorrectionEffect>().Init(Application::GetWindowWidth(), Application::GetWindowHeight());
		effect->GetComponent<ColorCorrectionEffect>().AddLUT("Resource Files/LUTs/NeutralLUT.cube");*/

		if (m_name == "Universe_19") {

			// Player
			/*auto playerEntity = GameObject::Allocate();
			MainPlayerID = playerEntity->GetID();
			playerEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 0));
			playerEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), MainPlayerID, *m_meshes[int(PlayerMesh::PLAYERSHIPPENCIL)], nullptr);
			playerEntity->GetComponent<Transform>().SetLocalScale(glm::vec3(0.75));
			
			playerEntity->GetComponent<Transform>().SetLocalRot(0, 180, 0);*/

			// Solari
			auto sunEntity = GameObject::Allocate();
			sunEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 0));
			sunEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), sunEntity->GetID(), *m_meshes[int(PlanetMesh::SOLARI)], nullptr, true);
			sunEntity->GetComponent<Transform>().SetRadius(3 * (m_meshes[int(PlanetMesh::SOLARI)]->GetWidth() / 2));
			sunEntity->GetComponent<Transform>().SetLocalScale(glm::vec3(3.0));

			//testing for ai
			auto enemy = GameObject::Allocate();
			entt::entity enemyID;
			enemyID = enemy->GetID();
			m_entities.push_back(enemyID);
			Transform tempTrans;
			enemy->AttachComponent<Transform>();
			BasicAI* testEnemy = new BasicAI(&tempTrans, &sunEntity->GetComponent<Transform>());
			//enemy->GetComponent<Transform>().SetLocalPos(tempTrans.GetLocalPos());
			enemy->GetComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 0));
			enemy->AttachComponent<StaticRenderer>(cameraEntity->GetID(), enemy->GetID(), *m_meshes[int(PlayerMesh::PLAYERSHIPPENCIL)], nullptr);

			enemy->GetComponent<Transform>().SetWHD(glm::vec3(m_meshes[int(PlayerMesh::PLAYERSHIPPENCIL)]->GetWidth(), m_meshes[int(PlayerMesh::PLAYERSHIPPENCIL)]->GetHeight(), m_meshes[int(PlayerMesh::PLAYERSHIPPENCIL)]->GetDepth()));

			// Verasten
			auto lavaPlanetEntity = GameObject::Allocate();
			lavaPlanetEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 315500));
			lavaPlanetEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), lavaPlanetEntity->GetID(), *m_meshes[int(PlanetMesh::VERASTEN)], nullptr);
			lavaPlanetEntity->GetComponent<Transform>().SetLocalScale(glm::vec3(250.0));

			// Yechin
			auto desertPlanetEntity = GameObject::Allocate();
			desertPlanetEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 515500));
			desertPlanetEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), desertPlanetEntity->GetID(), *m_meshes[int(PlanetMesh::YECHIN)], nullptr);
			desertPlanetEntity->GetComponent<Transform>().SetLocalScale(glm::vec3(550.0));

			// Kerantia
			auto homePlanetEntity = GameObject::Allocate();
			homePlanetEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 915500));
			homePlanetEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), homePlanetEntity->GetID(), *m_meshes[int(PlanetMesh::KERANTIA)], nullptr);
			homePlanetEntity->GetComponent<Transform>().SetLocalScale(glm::vec3(400.0));

			// Lunari
			auto moonEntity = GameObject::Allocate();
			moonEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 945500));
			moonEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), moonEntity->GetID(), *m_meshes[int(PlanetMesh::LUNARI)], nullptr);
			moonEntity->GetComponent<Transform>().SetLocalScale(glm::vec3(250.0));

			// Gueristis
			auto rockPlanetEntity = GameObject::Allocate();
			rockPlanetEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 1315500));
			rockPlanetEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), rockPlanetEntity->GetID(), *m_meshes[int(PlanetMesh::GUERISTIS)], nullptr);
			rockPlanetEntity->GetComponent<Transform>().SetLocalScale(glm::vec3(400.0));

			// Keminth
			auto icePlanetEntity = GameObject::Allocate();
			icePlanetEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 1515500));
			icePlanetEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), icePlanetEntity->GetID(), *m_meshes[int(PlanetMesh::KEMINTH)], nullptr);
			 
			
			//HUD
		/*	auto health = GameObject::Allocate();

			auto* tempAnim = &health->AttachComponent<AnimationHandler>();
			auto& anim = health->GetComponent<AnimationHandler>();
			anim.InitUVS(m_textures[4]);
			Animation2D Oneclip;
			Oneclip.AddFrame(UVS(glm::vec2(1, 233), glm::vec2(235, 1)));
			Oneclip.AddFrame(UVS(glm::vec2(236, 233), glm::vec2(470, 1)));
			Oneclip.AddFrame(UVS(glm::vec2(471, 233), glm::vec2(705, 1)));
			Oneclip.AddFrame(UVS(glm::vec2(706, 233), glm::vec2(939, 1)));
			Oneclip.SetIsRepeating(true);
			Oneclip.SetSecPerFrame(0.1);

			anim.AddAnimation(Oneclip);
			anim.SetActiveAnim(0);

			health->AttachComponent<Sprite2D>(m_textures[4], health->GetID(), 15, 15, true, tempAnim);
			health->AttachComponent<Transform>().SetLocalPos(glm::vec3(-80, -80, -10));
			*/


			auto abilities = GameObject::Allocate();
			abilities->AttachComponent<Sprite2D>(m_textures[1], abilities->GetID(), 15, 15);
			abilities->AttachComponent<Transform>().SetLocalPos(glm::vec3(80, -80, -10));
			auto powerUp = GameObject::Allocate();
			powerUp->AttachComponent<Sprite2D>(m_textures[2], powerUp->GetID(), 6, 45);
			powerUp->AttachComponent<Transform>().SetLocalPos(glm::vec3(-90, 10, -10));

			//effects
			/*effect = GameObject::Allocate();
			effect->AttachComponent<PostEffect>().Init(Application::GetWindowWidth(), Application::GetWindowHeight());*/
			/*effect->AttachComponent<GreyscaleEffect>().Init(Application::GetWindowWidth(), Application::GetWindowHeight());
			effect->AttachComponent<SepiaEffect>().Init(Application::GetWindowWidth(), Application::GetWindowHeight());
			effect->AttachComponent<ColorCorrectionEffect>().Init(Application::GetWindowWidth(), Application::GetWindowHeight());
			effect->GetComponent<ColorCorrectionEffect>().AddLUT("Resource Files/LUTs/NeutralLUT.cube");*/

			//Setting Parent/childe
			cameraEntity->GetComponent<Transform>().SetParent(&playerEntity->GetComponent<Transform>());
		/*	health->GetComponent<Transform>().SetParent(&cameraEntity->GetComponent<Transform>());*/
			abilities->GetComponent<Transform>().SetParent(&cameraEntity->GetComponent<Transform>());
			powerUp->GetComponent<Transform>().SetParent(&cameraEntity->GetComponent<Transform>());



		}
		else if (m_name == "Universe_27") {
			
			//// Sun
			//auto sunEntity2 = GameObject::Allocate();
			//sunEntity2->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 0));
			//sunEntity2->AttachComponent<StaticRenderer>(cameraEntity->GetID(), sunEntity2->GetID(), *m_meshes[int(PlanetMesh::SUN2)], nullptr, true);
			//sunEntity2->GetComponent<Transform>().SetLocalScale(glm::vec3(3.0));

			//// NP1
			//auto npEntity1 = GameObject::Allocate();
			//npEntity1->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 750));
			//npEntity1->AttachComponent<StaticRenderer>(cameraEntity->GetID(), npEntity1->GetID(), *m_meshes[int(PlanetMesh::NP1)], nullptr, true);
			//npEntity1->GetComponent<Transform>().SetLocalScale(glm::vec3(3.0));

			//// NP2
			//auto npEntity2 = GameObject::Allocate();
			//npEntity2->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 1500));
			//npEntity2->AttachComponent<StaticRenderer>(cameraEntity->GetID(), npEntity2->GetID(), *m_meshes[int(PlanetMesh::NP2)], nullptr, true);
			//npEntity2->GetComponent<Transform>().SetLocalScale(glm::vec3(3.0));

			//// NP3
			//auto npEntity3 = GameObject::Allocate();
			//npEntity3->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 2100));
			//npEntity3->AttachComponent<StaticRenderer>(cameraEntity->GetID(), npEntity3->GetID(), *m_meshes[int(PlanetMesh::NP3)], nullptr, true);
			//npEntity3->GetComponent<Transform>().SetLocalScale(glm::vec3(3.0));

			//// GP1
			//auto gasEntity1 = GameObject::Allocate();
			//gasEntity1->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 2250));
			//gasEntity1->AttachComponent<StaticRenderer>(cameraEntity->GetID(), gasEntity1->GetID(), *m_meshes[int(PlanetMesh::GP1)], nullptr, true);
			//gasEntity1->GetComponent<Transform>().SetLocalScale(glm::vec3(3.0));

			//// GP2
			//auto gasEntity2 = GameObject::Allocate();
			//gasEntity2->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 2875));
			//gasEntity2->AttachComponent<StaticRenderer>(cameraEntity->GetID(), gasEntity2->GetID(), *m_meshes[int(PlanetMesh::GP2)], nullptr, true);
			//gasEntity2->GetComponent<Transform>().SetLocalScale(glm::vec3(3.0));

			//// Citedal
			//auto citedalEntity = GameObject::Allocate();
			//citedalEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 3500));
			//citedalEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), citedalEntity->GetID(), *m_meshes[int(PlanetMesh::SUN2)], nullptr, true);
			//citedalEntity->GetComponent<Transform>().SetLocalScale(glm::vec3(3.0));

			//// Mushroom Planet
			//auto mpEntity = GameObject::Allocate();
			//mpEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, 4000));
			//mpEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), mpEntity->GetID(), *m_meshes[int(PlanetMesh::SUN2)], nullptr, true);
			//mpEntity->GetComponent<Transform>().SetLocalScale(glm::vec3(3.0));

		}
		else if (m_name == "Universe_5") {

		}

		Skybox::Init();
	}
}

void Universe::Update(float deltaTime)
{
	// Key Input
	KeyInput();

	// Camera Update 
	camera->Update();

	m_sceneReg->view<AnimationHandler>().each([=](AnimationHandler& anim) {	anim.Update(deltaTime); });
	// Transform Update
	m_sceneReg->view<Transform>().each([=](Transform& transform) {	transform.UpdateGlobal(); });

	if (isCollide(GameObject::GetComponent<Transform>(m_entities[0]), GameObject::GetComponent<Transform>(m_entities[1])))
		std::cout << "ISCOLLIDE" << std::endl;
}

void Universe::Render(float deltaTime)
{
//	effect->GetComponent<PostEffect>().Clear();
//	/*effect->GetComponent<ColorCorrectionEffect>().Clear();
//	*/
//	effect->GetComponent<PostEffect>().BindBuffer(0);

	m_sceneReg->view<StaticRenderer>().each([=](StaticRenderer& renderer) { renderer.Draw(); });
	Skybox::Draw(camera->GetView(), camera->GetProj());
	m_sceneReg->view<Sprite2D>().each([=](Sprite2D& renderer) {renderer.Draw(camera); });
	
	//effect->GetComponent<PostEffect>().UnbindBuffer();
	///*
	//effect->GetComponent<ColorCorrectionEffect>().ApplyEffect(&effect->GetComponent<PostEffect>());
	//effect->GetComponent<ColorCorrectionEffect>().DrawToScreen();*/
	//effect->GetComponent<PostEffect>().DrawToScreen();

}

void Universe::KeyInput()
{
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(m_window, GLFW_TRUE);

	// Player Movement //
	auto& playerEnt = GameObject::GetComponent<Transform>(m_entities[0]);

	if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		glm::vec3 temp = glm::vec3(0, 0, -2);
		playerEnt.MoveLocalPos(temp);
	}
	if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		glm::vec3 temp = glm::vec3(0, 0, 2);
		playerEnt.MoveLocalPos(temp);
	}
	if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		glm::vec3 temp = glm::vec3(0.0f, 1.0f, 0.0f);
		playerEnt.RotateLocal(temp);
	}
	if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		glm::vec3 temp = glm::vec3(0.0f, -1.0f, 0.0f);
		playerEnt.RotateLocal(temp);
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
		if (gamepad.trigger.LT > 0 && gamepad.trigger.LT < 1) {

		}

		else if (gamepad.trigger.LT > -1 && gamepad.trigger.LT < 0) {

		}

		if (gamepad.trigger.RT > 0 && gamepad.trigger.RT < 1) {

		}

		else if (gamepad.trigger.RT > -1 && gamepad.trigger.RT < 0) {

		}

		//axes input
		if (gamepad.axes[Direction::LEFT].X > 0 && gamepad.axes[Direction::LEFT].X < 1) {

		}
		else if (gamepad.axes[Direction::LEFT].X > -1 && gamepad.axes[Direction::LEFT].X < 0) {

		}

		if (gamepad.axes[Direction::LEFT].Y > 0 && gamepad.axes[Direction::LEFT].Y < 1) {

		}
		else if (gamepad.axes[Direction::LEFT].Y > -1 && gamepad.axes[Direction::LEFT].Y < 0) {

		}

		if (gamepad.axes[Direction::RIGHT].X > 0 && gamepad.axes[Direction::RIGHT].X < 1) {

		}
		else if (gamepad.axes[Direction::RIGHT].X > -1 && gamepad.axes[Direction::RIGHT].X < 0) {

		}

		if (gamepad.axes[Direction::RIGHT].Y > 0 && gamepad.axes[Direction::RIGHT].Y < 1) {

		}
		else if (gamepad.axes[Direction::RIGHT].Y > -1 && gamepad.axes[Direction::RIGHT].Y < 0) {

		}
	}

	else
		std::cout << "No controller connected" << std::endl;

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
