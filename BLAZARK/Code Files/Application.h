#pragma once

#include <glad/glad.h>
#include <glfw3.h>
#include <glm.hpp>

#include <iostream>
#include <string>

class Application
{
public:
	Application() = default;
	~Application() = default;

	static void Init(const std::string& name, bool isFullscreen);
	static void DeltaTime();
	static void FrameStart();
	static void SwapBuffers();

	static float GetDeltaTime();

	static GLFWwindow* GetWindow();

	static bool IsExitProgram();

	static void SetClearColor(const glm::vec4& clearColor);

	static int GetWindowWidth();
	static int GetWindowHeight();

private:
	static GLFWwindow* m_window;
	static int m_width, m_height;
	static float m_prevTime;
	static float m_deltaTime;
};