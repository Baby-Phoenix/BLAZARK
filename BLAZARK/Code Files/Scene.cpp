#include "Scene.h"

enum class EntityType { PLAYER = 0 };

enum class TextureType { TEMPPLANET, TEMPSHIP, START };

enum class MeshType { PLAYERSHIP, HOMEPLANET };

vector<Mesh*> Scene::m_meshes;
vector<Texture*> Scene::m_textures;

Scene::Scene(string name)
	:m_name(name)
{

	if (m_textures.size() < 1) {
		m_textures.push_back(new Texture("Resource Files/Textures/tempPlanetTex.png"));
		m_textures.push_back(new Texture("Resource Files/Textures/tempSpaceshipTex.png"));
		m_textures.push_back(new Texture("Resource Files/Textures/Menu/TitleScreen.png"));
	}
	if (m_meshes.size() < 1) {
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Ships/PlayerShips/PlayerShipBat.obj", *m_meshes[int(MeshType::PLAYERSHIP)]);
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/UniverseOne/Planets/Home_Planet.obj", *m_meshes[int(MeshType::HOMEPLANET)]);
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
	if (glfwGetKey(this->m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(this->m_window, GLFW_TRUE);

	if (glfwGetKey(this->m_window, GLFW_KEY_ENTER) == GLFW_PRESS) {
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

		if (m_name == "Universe_19") {
			auto cameraEntity = GameObject::Allocate();
			cameraEntity->AttachComponent<Transform>();
			camera = &cameraEntity->AttachComponent<Camera>(cameraEntity->GetID());
			camera->PerspectiveProj(1.0f, 100000.0f, Application::GetWindowWidth() / Application::GetWindowHeight(), 100.0f);
			cameraEntity->GetComponent<Transform>().SetLocalPos(glm::vec3(0.0f, 15.0f, 12.5f));

			auto playerEntity = GameObject::Allocate();
			MainPlayerID = playerEntity->GetID();
			playerEntity->AttachComponent<Transform>();
			playerEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), MainPlayerID, *m_meshes[int(MeshType::PLAYERSHIP)], m_textures[int(TextureType::TEMPSHIP)]);
			camera->Update(MainPlayerID);

			auto homePlanetEntity = GameObject::Allocate();
			homePlanetEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0, 0, -200));
			homePlanetEntity->AttachComponent<StaticRenderer>(cameraEntity->GetID(), homePlanetEntity->GetID(), *m_meshes[int(MeshType::HOMEPLANET)], m_textures[int(TextureType::TEMPPLANET)]);
		}
		else if (m_name == "Universe_27") {

		}
		else if (m_name == "Universe_5") {

		}

		Skybox::Init();
	}
}

void Universe::Update(float deltaTime)
{
	// Transform Update
	m_sceneReg->view<Transform>().each([=](Transform& transform) {	transform.UpdateGlobal(); });

	// Key Input
	KeyInput();

	// Camera Update
	camera->Update(MainPlayerID);
}

void Universe::Render(float deltaTime)
{
	m_sceneReg->view<StaticRenderer>().each([=](StaticRenderer& render) { render.Draw(); });
	Skybox::Draw(camera->GetView(), camera->GetProj());

	/*m_sceneReg->view<GreyscaleEffect>().each([=](GreyscaleEffect& effect)
	{
		effect.Clear();

		effect.BindBuffer(0);

		m_sceneReg->view<StaticRenderer>().each([=](StaticRenderer& render) {render.Draw(); });
		m_sceneReg->view<Sprite2D>().each([=](Sprite2D& render) {render.Draw(camera); });
		Skybox::Draw(camera->GetView(), camera->GetProj());

		effect.DrawToScreen();
		effect.UnbindBuffer();
	});*/
}

void Universe::KeyInput()
{
	if (glfwGetKey(this->m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(this->m_window, GLFW_TRUE);

	// Player Movement
	{
		auto view = m_sceneReg->view<Transform, StaticRenderer>();

		for (auto entity : view) {
			if (glfwGetKey(this->m_window, GLFW_KEY_W) == GLFW_PRESS)
			{
				glm::vec3 temp = glm::vec3(5.f, 0, 0);
				m_sceneReg->get<Transform>(entity).RotateLocalFixed(temp);
			}
		}
	}

	// Camera Movement
	{
		auto view = m_sceneReg->view<Camera>();

		for (auto entity : view) {

			auto& cament = GameObject::GetComponent<Camera>(entity);

			if (glfwGetKey(this->m_window, GLFW_KEY_UP) == GLFW_PRESS)
			{
				glm::vec3 temp = glm::vec3(0, 0, -5);
				cament.GetTransform().MoveLocalPos(temp);

			}
			if (glfwGetKey(this->m_window, GLFW_KEY_DOWN) == GLFW_PRESS)
			{

				glm::vec3 temp = glm::vec3(0.f, 0, 5);
				cament.GetTransform().MoveLocalPos(temp);

			}
			if (glfwGetKey(this->m_window, GLFW_KEY_LEFT) == GLFW_PRESS)
			{
				glm::vec3 temp = glm::vec3(-0.5f, 0, 0);
				cament.GetTransform().RotateLocal(temp);
			}
			if (glfwGetKey(this->m_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			{
				glm::vec3 temp = glm::vec3(0.5f, 0, 0);
				cament.GetTransform().RotateLocal(temp);
			}

		}
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
