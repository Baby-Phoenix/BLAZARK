#include "Application.h"
#include "Input.h"

GLFWwindow* Application::m_window = nullptr;
float Application::m_prevTime = 0.0f;
float Application::m_deltaTime = 0.0f;
bool Application::m_imguiInit = false;

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

	glfwSetKeyCallback(m_window, Input::GLFWInputCallback);

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

void Application::Tick()
{
	float time = static_cast<float>(glfwGetTime());
	m_deltaTime = time - m_prevTime;
	m_prevTime = time;
}

void Application::FrameStart()
{
	//Calculate our delta time for this frame.
	Tick();

	//Input polling.
	Input::FrameStart();
	glfwPollEvents();

	//Clear our window.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Application::SwapBuffers()
{

}

bool Application::IsExitProgram()
{
	return glfwWindowShouldClose(m_window);
}

void Application::SetClearColor(const glm::vec4& clearColor)
{
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
}
