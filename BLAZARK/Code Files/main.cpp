#include "Application.h"
#include "Game.h"


int main() {
	Application::Init("Blazark", false); //DONT SET FULLSCREEN
	Application::SetClearColor(glm::vec4(0.25f, 0.25f, 0.95f, 1.0f));

	Game game(Application::GetWindow());

	while (!Application::IsExitProgram())
	{
		Application::FrameStart();
		Application::DeltaTime();

		game.update(Application::GetDeltaTime());	

		Application::SwapBuffers();
	}
		
	return 0;
}