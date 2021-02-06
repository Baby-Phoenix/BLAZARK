#include "Scene.h"


std::unique_ptr<GameObject> cament;
std::unique_ptr<GameObject> spriteent;
std::unique_ptr<GameObject> planetent;

vector<Mesh*> Scene::m_meshes;
vector<Texture*> Scene::m_textures;

Scene::Scene(string name)
	:m_name(name)
{

	if (m_textures.size() < 1) {
		m_textures.push_back(new Texture("Resource Files/Textures/tempPlanetTex.png"));
		m_textures.push_back(new Texture("Resource Files/Textures/venom.png"));

	}
	if (m_meshes.size() < 1) {
		m_meshes.push_back(new Mesh());
		loadOBJ("Resource Files/OBJFiles/Home_Planet.obj", *m_meshes[0]);
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
		//add entites in here if not initialised

		if (m_name == "Start_Menu") {
			cament = GameObject::Allocate();
			cament->AttachComponent<Transform>();
			cam = &cament->AttachComponent<Camera>(cament.get());
			cament->GetComponent<Camera>().PerspectiveProj(1.0f, 100000.0f, Application::GetWindowWidth()/Application::GetWindowHeight(), 100.0f);
			cament->GetComponent<Transform>().SetLocalPos(glm::vec3(0.0f, 0.0f, 100.0f));


			planetent = GameObject::Allocate();
			planetent->AttachComponent<StaticRenderer>(cament.get(), planetent.get(), *m_meshes[0], m_textures[0]);
			planetent->AttachComponent<Transform>().SetLocalScale(glm::vec3(1, 1, 1));
			planetent->GetComponent<Transform>().SetLocalPos(glm::vec3(100, 0, 0));


			spriteent = GameObject::Allocate();
			spriteent->AttachComponent<Sprite2D>(m_textures[1], spriteent.get(), 100, 100);
			spriteent->AttachComponent<Transform>();
			//spriteent->GetComponent<Transform>().SetLocalScale(temp);
		}

		else if (m_name == "Pause_Menu") {
			cament = GameObject::Allocate();
			cament->AttachComponent<Transform>();
			cam = &cament->AttachComponent<Camera>(cament.get());
			cament->GetComponent<Camera>().PerspectiveProj(1.0f, 60.0f, Application::GetWindowWidth() / Application::GetWindowHeight(), 100.0f);
			cament->GetComponent<Transform>().SetLocalPos(glm::vec3(0.0f, 0.0f, 100.0f));
		}
	}

	Skybox::Init();

}

void Menu::Update(float deltaTime)
{
	//Camera Update
	 m_sceneReg->view<Transform>().each([=](Transform& transform) {	transform.UpdateGlobal();});
	KeyInput();
	cam->Update();
	
}

void Menu::KeyInput()
{
	if (glfwGetKey(this->m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(this->m_window, GLFW_TRUE);
	
	//player movement
	if (glfwGetKey(this->m_window, GLFW_KEY_W) == GLFW_PRESS)
	{
		glm::vec3 temp = glm::vec3(5.f, 0, 0);
		planetent->GetComponent<Transform>().RotateLocalFixed(temp);
		spriteent->GetComponent<Transform>().RotateLocalFixed(temp);

	}
	if (glfwGetKey(this->m_window, GLFW_KEY_A) == GLFW_PRESS)
	{
		glm::vec3 temp = glm::vec3(0.f, -5, 0);
		spriteent->GetComponent<Transform>().RotateLocalFixed(temp);
	}
	if (glfwGetKey(this->m_window, GLFW_KEY_S) == GLFW_PRESS)
	{
		glm::vec3 temp = glm::vec3(-5.f, 0, 0);
		spriteent->GetComponent<Transform>().RotateLocalFixed(temp);
	}
	if (glfwGetKey(this->m_window, GLFW_KEY_D) == GLFW_PRESS)
	{
		glm::vec3 temp = glm::vec3(0.f, 5, 0);
		spriteent->GetComponent<Transform>().RotateLocalFixed(temp);
	}


	//camera movement
	if (glfwGetKey(this->m_window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		glm::vec3 temp = glm::vec3(0, 0, -5);
		cament->GetComponent<Transform>().MoveLocalPos(temp);

	}
	if (glfwGetKey(this->m_window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{

		glm::vec3 temp = glm::vec3(0.f, 0, 5);
		cament->GetComponent<Transform>().MoveLocalPos(temp);

	}
	if (glfwGetKey(this->m_window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		glm::vec3 temp = glm::vec3(-0.5f, 0, 0);
		cament->GetComponent<Transform>().RotateLocal(temp);
	}
	if (glfwGetKey(this->m_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		glm::vec3 temp = glm::vec3(0.5f, 0, 0);
		cament->GetComponent<Transform>().RotateLocal(temp);
	}
}

void Menu::MouseInput()
{
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

	m_sceneReg->view<StaticRenderer>().each([=](StaticRenderer& render) {render.Draw(); });
	m_sceneReg->view<Sprite2D>().each([=](Sprite2D& render) {render.Draw(cam); });	
	Skybox::Draw(cam->GetView(), cam->GetProj());
}

Universe::Universe(string name, unsigned int* num, bool* change)
{
}

void Universe::InitScene()
{
}

void Universe::InitSkyBox()
{
	
}

void Universe::Update(float deltaTime)
{
}

void Universe::Render(float deltaTime)
{
}

void Universe::UpdateSkyBox()
{
	
}

void Universe::KeyInput()
{
}

void Universe::MouseInput()
{
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
