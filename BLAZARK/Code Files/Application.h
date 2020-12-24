#pragma once

#include <glad/glad.h>
#include <glfw3.h>
#include<glm.hpp>

#include <iostream>
#include <string>

class Application
{
public:
	Application() = default;
	~Application() = default;

	static void Init(const std::string& name, bool isFullscreen);

private:
	static GLFWwindow* m_window;
};