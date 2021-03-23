#pragma once
#include "glfw3.h"
#include<iostream>

enum Joystick { LEFT = 0, RIGHT };

struct Triggers {

	Triggers()
		:RT(0)
		, LT(0)
	{}

	float RT;
	float LT;
};

struct Buttons
{
	Buttons()
		:A(0)
		, B(0)
		, X(0)
		, Y(0)
	{}

	bool A;
	bool B;
	bool X;
	bool Y;
};

struct Bumper {
	Bumper()
		:RB(0)
		, LB(0)
	{}

	bool RB;
	bool LB;
};

struct Axis {

	Axis()
		:X(0)
		, Y(0)
	{}

	float X;
	float Y;
};

class GamePad {

public:

	bool getGamepadInput();



	Triggers trigger;
	Axis axes[2];// 0 is left 1 is right
	Bumper bumper;
	Buttons buttons;

};