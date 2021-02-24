#include "Application.h"
#include "Input.h"
#include "FrameBuffer.h"

GLFWwindow* Application::m_window = nullptr;
float Application::m_prevTime = 0.0f;
float Application::m_deltaTime = 0.0f;

int Application::m_width;
int Application::m_height;

void Application::Init(const std::string& name, bool isFullscreen)
{
	//Initialize GLFW
	if (glfwInit() == GLFW_FALSE)
	{
		std::cout << "ERROR::GLFW_WINDOW_INIT_FAILED" << "\n";
		glfwTerminate();
	}

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//GLFW version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);

	//Window resizing
	glfwWindowHint(GLFW_RESIZABLE, false);

	//Create window
	if(isFullscreen) {
		m_window = glfwCreateWindow(800, 800, name.c_str(), glfwGetPrimaryMonitor(), NULL);
		glfwSetWindowSize(m_window, 1920, 1080);
	}
	else
	{
		m_width  = 1440;
		m_height = 900;
		m_window = glfwCreateWindow(m_width, m_height, name.c_str(), NULL, NULL);
	}
	glfwMakeContextCurrent(m_window);

	//glfwSetKeyCallback(m_window, Input::GLFWInputCallback);

	//Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR::GLAD_INIT_FAILED" << std::endl;
		glfwTerminate();
	}

	printf("OpenGL Renderer: %s\n", glGetString(GL_RENDERER));
	printf("OpenGL Version: %s\n", glGetString(GL_VERSION));

	glViewport(0, 0, m_width, m_height);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);

	//glDisable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	FrameBuffer::InitFullscreenQuad();
	//Disable cursor
	//glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Application::DeltaTime()
{
	float time = (float)glfwGetTime();
	m_deltaTime = time - m_prevTime;
	m_prevTime = time;
}

void Application::FrameStart()
{

	//Input
	glfwPollEvents();//Clearwindow.

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Calculate delta time
	DeltaTime();
	
}

void Application::SwapBuffers()
{
	glfwSwapBuffers(m_window);
	glFlush();
}

float Application::GetDeltaTime()
{
	return m_deltaTime;
}

GLFWwindow* Application::GetWindow()
{
	return m_window;
}

bool Application::IsExitProgram()
{
	return glfwWindowShouldClose(m_window);
}

void Application::SetClearColor(const glm::vec4& clearColor)
{
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
}

float Application::GetWindowWidth()
{
	return (float)m_width;
}

float Application::GetWindowHeight()
{
	return (float)m_height;
}
