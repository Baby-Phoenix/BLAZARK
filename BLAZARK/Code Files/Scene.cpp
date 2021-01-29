#include "Scene.h"
#include "btBulletDynamicsCommon.h"

std::unique_ptr<GameObject> cament;
std::unique_ptr<GameObject> spriteent;
std::unique_ptr<GameObject> planetent;


Scene::Scene(string name)
	:m_name(name)
{
}

string Scene::GetName()
{
	return m_name;
}

void Scene::Render(float deltaTime)
{
}

void Scene::Update(float deltaTime)
{
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
	m_shaders.push_back(new Shader("Resource Files/Shaders/static_shader_vert.glsl", "Resource Files/Shaders/static_shader_frag.glsl"));
	m_shaders.push_back(new Shader("Resource Files/Shaders/Sprite2D_vert.glsl", "Resource Files/Shaders/Sprite2D_frag.glsl"));

	m_textures.push_back(new Texture("Resource Files/Textures/tempPlanetTex.png", GL_TEXTURE_2D));
	
	//creating a new registry for the scene when initialised
	if (m_sceneReg == nullptr) 
		m_sceneReg = new entt::registry();
	
	//Giving the ECS the same registry as the current scene
	GameObject::SetRegistry(m_sceneReg);

	if (GameObject::IsEmpty()) {
		//add entites in here if not initialised
	}


	auto testMesh = std::make_unique<Mesh>();
	loadOBJ("Resource Files/OBJFiles/cube.obj", *testMesh);

	cament = GameObject::Allocate();
	cament->AttachComponent<Transform>();
	cam = &cament->AttachComponent<Camera>(cament.get());
	cament->GetComponent<Camera>().PerspectiveProj(1.0f, 60.0f, 1.0f, 100.0f);
	cament->GetComponent<Transform>().SetLocalPos(glm::vec3(0.0f, 0.0f, 100.0f));
	

	planetent = GameObject::Allocate();
	planetent->AttachComponent<StaticRenderer>(cament.get(), planetent.get(), *testMesh, m_textures[0]);
	planetent->AttachComponent<Transform>().SetLocalScale(glm::vec3(50, 50, 50));
	
	spriteent = GameObject::Allocate();
	spriteent->AttachComponent<Sprite2D>(m_textures[0], spriteent.get(), 50, 50);

	auto temp = glm::vec3(90, 0, 0);

	spriteent->AttachComponent<Transform>().RotateLocalFixed(temp);

}

void Menu::Update(float deltaTime)
{
	//Camera Update
	

	KeyInput();
	cam->Update();
	spriteent->GetComponent<Transform>().UpdateGlobal();
	planetent->GetComponent<Transform>().UpdateGlobal();
}

void Menu::KeyInput()
{
	if (glfwGetKey(this->m_window, GLFW_KEY_W) == GLFW_PRESS)
	{
		glm::vec3 temp = glm::vec3(45.f, 0, 0);
		planetent->GetComponent<Transform>().RotateLocalFixed(temp);
		spriteent->GetComponent<Transform>().RotateLocalFixed(temp);

	}
	if (glfwGetKey(this->m_window, GLFW_KEY_A) == GLFW_PRESS)
	{
		glm::vec3 temp = glm::vec3(0.f, -45, 0);
		spriteent->GetComponent<Transform>().RotateLocalFixed(temp);
	}
	if (glfwGetKey(this->m_window, GLFW_KEY_S) == GLFW_PRESS)
	{
		glm::vec3 temp = glm::vec3(-45.f, 0, 0);
		spriteent->GetComponent<Transform>().RotateLocalFixed(temp);
	}
	if (glfwGetKey(this->m_window, GLFW_KEY_D) == GLFW_PRESS)
	{
		glm::vec3 temp = glm::vec3(0.f, 45, 0);
		spriteent->GetComponent<Transform>().RotateLocalFixed(temp);
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
	

		planetent->GetComponent<StaticRenderer>().Draw(m_shaders[0]);

		//spriteent->GetComponent<Sprite2D>().Draw(m_shaders[1], cam);

		
	
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
