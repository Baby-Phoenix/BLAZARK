#include "Scene.h"

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
	m_sceneReg = new entt::registry();

	//Giving the ECS the same registry as the current scene
	GameObject::SetRegistry(m_sceneReg);
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
	m_sceneReg = new entt::registry();

	//Giving the ECS the same registry as the current scene
	GameObject::SetRegistry(m_sceneReg);
}

void Menu::Update(float deltaTime)
{
}

void Menu::KeyInput()
{
}

void Menu::MouseInput()
{
}

void Menu::GamepadInput()
{

	int present = glfwJoystickPresent(GLFW_JOYSTICK_1);

	//DEBUGGING
		/*std::cout << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
		std::cout << "Left Stick X Axis: " << axes[0] << std::endl;
		std::cout << "Left Stick Y Axis: " << axes[1] << std::endl;
		std::cout << "Right Stick X Axis: " << axes[2] << std::endl;
		std::cout << "Right Stick Y Axis: " << axes[3] << std::endl;
		std::cout << "Left Trigger/L2: " << axes[4] << std::endl;
		std::cout << "Right Trigger/R2: " << axes[5] << std::endl;*/


	if (present == 1)
	{
		int axesCount;
		const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);

		float temp;

		//Turn left
		if (axes[0] < -0.2f) //Joystick left
		{
		}
		//Turn right
		if (axes[1] > 0.2f) //Joystick right
		{

		}

		if (axes[2]) //Right Stick X 
		{

		}

		if (axes[3]) //Right Stick Y 
		{

		}

		if (axes[5] > -0.7) //Right Trigger 
		{

		}

	}






	int buttonCount;
	const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);

	if (GLFW_PRESS == buttons[0])
	{
		std::cout << "A button pressed: " << std::endl;
	}

	if (GLFW_PRESS == buttons[1])
	{
		std::cout << "B button pressed: " << std::endl;
	}

	if (GLFW_PRESS == buttons[2])
	{
		std::cout << "X button pressed: " << std::endl;
	}

	if (GLFW_PRESS == buttons[3])
	{
		std::cout << "Y button pressed: " << std::endl;
	}

}


void Menu::Render(float deltaTime)
{
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

	int present = glfwJoystickPresent(GLFW_JOYSTICK_1);

	//DEBUGGING
		/*std::cout << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
		std::cout << "Left Stick X Axis: " << axes[0] << std::endl;
		std::cout << "Left Stick Y Axis: " << axes[1] << std::endl;
		std::cout << "Right Stick X Axis: " << axes[2] << std::endl;
		std::cout << "Right Stick Y Axis: " << axes[3] << std::endl;
		std::cout << "Left Trigger/L2: " << axes[4] << std::endl;
		std::cout << "Right Trigger/R2: " << axes[5] << std::endl;*/


	if (present == 1)
	{
		int axesCount;
		const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);

		float temp;

		//Turn left
		if (axes[0] < -0.2f) //Joystick left
		{
		}
		//Turn right
		if (axes[1] > 0.2f) //Joystick right
		{

		}

		if (axes[2]) //Right Stick X 
		{

		}

		if (axes[3]) //Right Stick Y 
		{

		}

		if (axes[5] > -0.7) //Right Trigger 
		{

		}

	}



	int buttonCount;
	const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);

	if (GLFW_PRESS == buttons[0])
	{
		std::cout << "A button pressed: " << std::endl;
	}

	if (GLFW_PRESS == buttons[1])
	{
		std::cout << "B button pressed: " << std::endl;
	}

	if (GLFW_PRESS == buttons[2])
	{
		std::cout << "X button pressed: " << std::endl;
	}

	if (GLFW_PRESS == buttons[3])
	{
		std::cout << "Y button pressed: " << std::endl;
	}

}
