#include "Application.h"

int main() {

	
	Application::Init("Blazark", false); //DONT SET FULLSCREEN
	Application::SetClearColor(glm::vec4(0.25f, 0.25f, 0.95f, 1.0f));

	while (!Application::IsExitProgram())
	{
		Application::FrameStart();

		

		Application::SwapBuffers();

	
	}
		


	return 0;
}