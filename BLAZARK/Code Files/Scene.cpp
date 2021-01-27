#include "Scene.h"
#include "btBulletDynamicsCommon.h"

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
	m_shaders.push_back(new Shader("Resource Files/Shaders/static_shader.vert", "Resource Files/Shaders/static_shader.frag"));
	m_shaders.push_back(new Shader("Resource Files/Shaders/Sprite2D.vert", "Resource Files/Shaders/Sprite2D.frag"));

	m_textures.push_back(new Texture("Resource Files/Textures/blazark.png", GL_TEXTURE_2D));
//creating a new registry for the scene when initialised
	if (m_sceneReg == nullptr)
		m_sceneReg = new entt::registry();

	//Giving the ECS the same registry as the current scene
	GameObject::SetRegistry(m_sceneReg);

	if (GameObject::IsEmpty()) {
		//add entites in here if not initialised
	}

	auto testMesh = std::make_unique<Mesh>();
	loadOBJ("Resource Files/OBJFiles/Home_Planet.obj", *testMesh);

	auto CamEntity = GameObject::Allocate();
	CamEntity->AttachComponent<Transform>();
	CamEntity->AttachComponent<Camera>(CamEntity.get());
	CamEntity->GetComponent<Camera>().PerspectiveProj(60.0f, 1.0f, 0.1f, 100.0f);
	CamEntity->GetComponent<Transform>().SetLocalPos(glm::vec3(0.0f, 0.0f, 1000.0f));
	

	auto TestEntity = GameObject::Allocate();
	TestEntity->AttachComponent<Transform>();
	TestEntity->AttachComponent<StaticRenderer>(CamEntity.get(), TestEntity.get(), *testMesh);

	auto SpriteEntity = GameObject::Allocate();
	SpriteEntity->AttachComponent<Sprite2D>(m_textures[0],SpriteEntity.get(), 10, 10);
	SpriteEntity->AttachComponent<Transform>().SetLocalPos(glm::vec3(0.0f, 0.0f, 1000.0f));

}

void Menu::Update(float deltaTime)
{
	//Camera Update
	

	{
		auto CamView = m_sceneReg->view<Camera>();

		for (auto entity : CamView) {
			CamView.get(entity).Update();
			cam = &CamView.get<Camera>(entity);
		}
	}
	
	{
		auto view = m_sceneReg->view<Sprite2D, Transform>();

		for (auto entity : view) {
			auto& spr = view.get<Sprite2D>(entity);
			
			spr.Draw(m_shaders[1], cam);
			
		}
	}

}

void Menu::KeyInput()
{
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
	auto StaticView = m_sceneReg->view<StaticRenderer>();

	for (auto entity : StaticView) {
		StaticView.get(entity).Draw(m_shaders[0]);
	}
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
