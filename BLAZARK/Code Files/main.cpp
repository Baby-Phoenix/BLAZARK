#include "Game.h"


int main() {
	Application::Init("Blazark", false); // DONT SET FULLSCREEN

	Game game(Application::GetWindow());

	while (!Application::IsExitProgram())
	{
		Application::FrameStart();

		game.update(Application::GetDeltaTime());

		Application::SwapBuffers();
	}
	
	return 0;
}