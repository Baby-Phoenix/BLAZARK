#include "Application.h"

GLFWwindow* Application::m_window = nullptr;

void Application::Init(const std::string& name, bool isFullscreen)
{
	//Initialize GLFW
	if (glfwInit() == GLFW_FALSE)
	{
		std::cout << "ERROR::GLFW_WINDOW_INIT_FAILED" << "\n";
		glfwTerminate();
	}

	//Window resizing
	glfwWindowHint(GLFW_RESIZABLE, true);

	//Create window
	if(isFullscreen)
		m_window = glfwCreateWindow(800, 600, "Blazark", glfwGetPrimaryMonitor(), NULL); 
	else
		m_window = glfwCreateWindow(800, 600, name.c_str(), nullptr, nullptr); 

	glfwMakeContextCurrent(m_window);

	//Initialize GLAD
	if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0)
	{
		std::cout << "ERROR::GLAD_INIT_FAILED" << std::endl;
		glfwTerminate();
	}

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.5f, 0.5f, 1.0f, 1.0f);

	//Disable cursor
	//glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
