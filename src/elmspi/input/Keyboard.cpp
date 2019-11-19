#include "Keyboard.h"
#include <stdlib.h>
#include "../window/Window.h"
#include <cstring>

static bool onKeyboardCallback = false;
#define callback(x) onKeyboardCallback = true; x; onKeyboardCallback = false;

/*Private Symbols*/
static bool* pressedArray = nullptr;
static bool* initialPressArray = nullptr;
static bool* releasedArray = nullptr;
static bool* repeatArray = nullptr;

namespace CallbackHandlers
{
	static void onKeyCallback(GLFWwindow* windowID, int key, int scancode, int action, int mods)
	{
		Window* window = WindowUtil::getWindowByID(windowID);
		callback(window->getKeyboard().tick());

		if (pressedArray == nullptr || initialPressArray == nullptr || releasedArray == nullptr || repeatArray == nullptr) exit(17);

		if (action == GLFW_PRESS)
		{
			pressedArray[key + 1] = true;
			initialPressArray[key + 1] = true;
		}
		if (action == GLFW_RELEASE)
		{
			pressedArray[key + 1] = false;
			releasedArray[key + 1] = true;
			repeatArray[key + 1] = false;
		}
		if (action == GLFW_REPEAT)
		{
			repeatArray[key + 1] = true;
		}
	}
}

Keyboard::Keyboard(GLFWwindow* window)
{
	memset(pressedStates, 0, arraySize);
	memset(initialPressStates, 0, arraySize);
	memset(releaseStates, 0, arraySize);
	memset(repeatStates, 0, arraySize);

	glfwSetKeyCallback(window, CallbackHandlers::onKeyCallback);
};

Keyboard::~Keyboard()
{
}

bool Keyboard::isKeyPressed(Keys key) const
{
	return pressedStates[key + 1];
}

bool Keyboard::keyJustPressed(Keys key) const
{
	return initialPressStates[key + 1];
}

bool Keyboard::keyJustReleased(Keys key) const
{
	return releaseStates[key + 1];
}

bool Keyboard::isKeyRepeating(Keys key) const
{
	return repeatStates[key + 1];
}

void Keyboard::tick()
{
	if (onKeyboardCallback)
	{
		publicize();
		return;
	}

	memset(initialPressStates, 0, arraySize);
	memset(releaseStates, 0, arraySize);
	memset(repeatStates, 0, arraySize);
}

void Keyboard::publicize()
{
	pressedArray = pressedStates;
	initialPressArray = initialPressStates;
	releasedArray = releaseStates;
	repeatArray = repeatStates;
}
