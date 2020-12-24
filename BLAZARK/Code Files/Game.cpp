//#include "Game.h"
//#include "gtx/matrix_decompose.hpp"
//
////Constructors/destructors
////Game::Game(const char* title, const int WINDOW_WIDTH, const int WINDOW_HEIGHT, const int GL_VERSION_MAJOR, const int GL_VERSION_MINOR, bool resizable)
////	: WINDOW_WIDTH(WINDOW_WIDTH), WINDOW_HEIGHT(WINDOW_HEIGHT), GL_VERSION_MAJOR(GL_VERSION_MAJOR), GL_VERSION_MINOR(GL_VERSION_MINOR)
////{
////
////	//Init variables
////	//this->window = nullptr;
////	//this->framebufferWidth = this->WINDOW_WIDTH;
////	//this->framebufferHeight = this->WINDOW_HEIGHT;
////
////	//this->fov = 90.0f;
////	//this->nearPlane = 0.1f;
////	//this->farPlane = 100000.0f;
////
////
////	//this->initGLFW();
////	//this->initWindow(title, resizable);
////	//this->initGLAD();
////	//this->initOpenGLOptions();
////	//this->InitGame();
////	//this->initMatrices(); //false for ortho camera
////	//this->initUniforms();
////
////}
//
//
//Game::~Game()
//{
//	//glfwDestroyWindow(this->window);
//	glfwTerminate();
//
//	/*for (size_t i = 0; i < this->m_scenes.size(); i++)
//		delete this->m_scenes[i];
//
//	for (auto*& i : this->m_curScene->GetEntityRegistry())
//		delete i;*/
//
//
//}
//
//void Game::InitGame()
//{
//
//}
//
//
////void Game::SwitchScene(unsigned int sceneNo)
////{
////
////}
//
//
//
//
//
//
//void Game::update()
//{
//	//UPDATE INPUT ---
//	//this->updateInput();
//
//}
//
//void Game::render()
//{
//
//	//RENDER ---
//	//Update the uniforms
//	//this->updateUniforms();
//
//	//this->m_curScene->Render(this->dt);
//
//
//	/*glBindVertexArray(0);
//	glUseProgram(0);
//	glActiveTexture(0);
//	glBindTexture(GL_TEXTURE_2D, 0);*/
//
//	//END DRAW
//	//glfwSwapBuffers(window);
//	//glFlush();
//}
//
////Private functions
////void Game::initGLFW()
////{
////	if (glfwInit() == GLFW_FALSE)
////	{
////		std::cout << "ERROR::GLFW_INIT_FAILED" << "\n";
////		glfwTerminate();
////	}
////}
////
////
////
////void Game::initGLAD()
////{
////	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
////		std::cout << "Failed to initialize GLAD" << std::endl;
////	}
////}
//
//
//
//
//
////void Game::UpdateScene()
////{
////	if (switchIt)
////		this->SwitchScene(SceneNo);
////
////}
//
//
