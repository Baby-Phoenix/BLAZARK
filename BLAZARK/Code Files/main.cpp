#include "Game.h"

int main() {
	Application::Init("Blazark", false); // DON'T SET TO FULLSCREEN

	Game game(Application::GetWindow());

	while (!Application::IsExitProgram())
	{
		Application::FrameStart();

		game.update(Application::GetDeltaTime());

		Application::SwapBuffers();
	}
	
	return 0;
}