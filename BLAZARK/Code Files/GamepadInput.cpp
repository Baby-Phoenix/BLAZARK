#include "GamePadInput.h"

bool GamePad::getGamepadInput()
{
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

	//LB
	this->bumper.LB = GLFW_PRESS == buttons[4];

	//RB
	this->bumper.RB = GLFW_PRESS == buttons[5];

	return true;
}
