#include "GamePadInput.h"

enum Direction { LEFT = 0, RIGHT };

bool GamePad::getGamepadInput()
{
	//DEBUGGING
		/*std::cout << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
		std::cout << "Left Stick X Axis: " << axes[0] << std::endl;
		std::cout << "Left Stick Y Axis: " << axes[1] << std::endl;
		std::cout << "Right Stick X Axis: " << axes[2] << std::endl;
		std::cout << "Right Stick Y Axis: " << axes[3] << std::endl;
		std::cout << "Left Trigger/L2: " << axes[4] << std::endl;
		std::cout << "Right Trigger/R2: " << axes[5] << std::endl;*/


	int* axesCount = new int();
	const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, axesCount);

	//if no joystick is plugged in
	if (axes == NULL)
		return false;

	//setting all the values
	//left axes
	this->axes[LEFT].X = axes[0];
	this->axes[LEFT].Y = axes[1];

	//right axes
	this->axes[RIGHT].X = axes[2];
	this->axes[RIGHT].Y = axes[3];

	//left trigger
	this->trigger.LT = axes[4];

	//right trigger
	this->trigger.RT = axes[5];

	//Button if statements
	int buttonCount;
	const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);

	//A button
	this->buttons.A = GLFW_PRESS == buttons[0];

	//B button
	this->buttons.B = GLFW_PRESS == buttons[1];

	//X button
	this->buttons.X = GLFW_PRESS == buttons[2];

	//Y button
	this->buttons.Y = GLFW_PRESS == buttons[3];

	return true;

}
