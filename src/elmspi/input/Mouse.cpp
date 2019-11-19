#include "Mouse.h"
#include "../window/Window.h"
#include <cstring>

static bool onMouseCallBack = false;
#define callback(x) onMouseCallBack = true; x; onMouseCallBack = false;

/*Private Symbols*/
static double* pCursorPosX = nullptr;
static double* pCursorPosY = nullptr;
static bool* pCursorIsOnScreen = nullptr;
static bool* pHadBeenScrolled = nullptr;
static bool* pPressedStates = nullptr;
static bool* pInitialPressedStates = nullptr;
static bool* pReleasedStates = nullptr;
static double* pScrollAmountX = nullptr;
static double* pScrollAmountY = nullptr;
static double* pSumScrollX = nullptr;
static double* pSumScrollY = nullptr;

namespace CallbackHandlers
{
	static void onCursorPos(GLFWwindow* windowID, double posX, double posY)
	{
		Window* window = WindowUtil::getWindowByID(windowID);
		callback(window->getMouse().tick());

		*pCursorPosX = posX;
		*pCursorPosY = posY;
	}
	static void onEnteringCursor(GLFWwindow* windowID, int isEntering)
	{
		Window* window = WindowUtil::getWindowByID(windowID);
		callback(window->getMouse().tick());

		*pCursorIsOnScreen = isEntering ? true : false;
	}
	static void onMouseButton(GLFWwindow* windowID, int button, int action, int mods)
	{
		Window* window = WindowUtil::getWindowByID(windowID);
		callback(window->getMouse().tick());

		if (action == GLFW_PRESS)
		{
			pPressedStates[button] = true;
			pInitialPressedStates[button] = true;
		}
		if (action == GLFW_RELEASE)
		{
			pReleasedStates[button] = true;
			pPressedStates[button] = false;
		}
	}
	static void onScroll(GLFWwindow* windowID, double scrollX, double scrollY)
	{
		*pHadBeenScrolled = true;
		*pScrollAmountX = scrollX;
		*pScrollAmountY = scrollY;
		*pSumScrollX += scrollX;
		*pSumScrollY += scrollY;
	}
}

Mouse::Mouse(GLFWwindow* window)
{
	windowID = window;

	cursorPosX = cursorPosY = prevCursorPosX = prevCursorPosY = 0.0;

	cursorIsOnScreen = hadBeenScrolled = cursorIsLocked = false;

	scrollAmountX = scrollAmountY = sumScrollX = sumScrollY = 0.0;

	memset(pressedStates, 0, statesArraySize);
	memset(initialPressStates, 0, statesArraySize);
	memset(releasedStates, 0, statesArraySize);

	glfwSetCursorPosCallback(window, CallbackHandlers::onCursorPos);
	glfwSetCursorEnterCallback(window, CallbackHandlers::onEnteringCursor);
	glfwSetMouseButtonCallback(window, CallbackHandlers::onMouseButton);
	glfwSetScrollCallback(window, CallbackHandlers::onScroll);
}

Mouse::~Mouse()
{
}

void Mouse::setLockMouse(bool locked)
{
	if (cursorIsLocked == locked) return;
	cursorIsLocked = locked;
	glfwSetInputMode(windowID, GLFW_CURSOR, locked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

double Mouse::posX() const
{
	return cursorPosX;
}

double Mouse::posY() const
{
	return cursorPosY;
}

double Mouse::deltaPosX() const
{
	return cursorPosX - prevCursorPosX;
}

double Mouse::deltaPosY() const
{
	return cursorPosY - prevCursorPosY;
}

double Mouse::scrollX() const
{
	return scrollAmountX;
}

double Mouse::scrollY() const
{
	return scrollAmountY;
}

double Mouse::totalScrollX() const
{
	return sumScrollX;
}

double Mouse::totalScrollY() const
{
	return sumScrollY;
}

bool Mouse::leftJustReleased() const
{
	return releasedStates[Buttons::MOUSE_BUTTON_LEFT];
}

bool Mouse::leftIsPressed() const
{
	return pressedStates[Buttons::MOUSE_BUTTON_LEFT];
}

bool Mouse::leftJustPressed() const
{
	return initialPressStates[Buttons::MOUSE_BUTTON_LEFT];
}

bool Mouse::rightJustReleased() const
{
	return releasedStates[Buttons::MOUSE_BUTTON_RIGHT];
}

bool Mouse::rightIsPressed() const
{
	return pressedStates[Buttons::MOUSE_BUTTON_RIGHT];
}

bool Mouse::rightJustPressed() const
{
	return initialPressStates[Buttons::MOUSE_BUTTON_RIGHT];
}

bool Mouse::middleJustReleased() const
{
	return releasedStates[Buttons::MOUSE_BUTTON_MIDDLE];
}

bool Mouse::middleIsPressed() const
{
	return pressedStates[Buttons::MOUSE_BUTTON_MIDDLE];
}

bool Mouse::middleJustPressed() const
{
	return initialPressStates[Buttons::MOUSE_BUTTON_MIDDLE];
}

bool Mouse::buttonIsPressed(Buttons button) const
{
	return pressedStates[button];
}

bool Mouse::buttonJustPressed(Buttons button) const
{
	return initialPressStates[button];
}

bool Mouse::buttonJustReleased(Buttons button) const
{
	return releasedStates[button];
}

bool Mouse::cursorIsFocused() const
{
	return cursorIsOnScreen;
}

bool Mouse::hasScrolled() const
{
	return hadBeenScrolled;
}

bool Mouse::isLocked() const
{
	return cursorIsLocked;
}

void Mouse::resetScroll()
{
	scrollAmountX = scrollAmountY = sumScrollX = sumScrollY = 0.0;
}

void Mouse::tick()
{
	if (onMouseCallBack)
	{
		publicize();
		return;
	}

	prevCursorPosX = cursorPosX;
	prevCursorPosY = cursorPosY;

	hadBeenScrolled = false;

	memset(initialPressStates, 0, statesArraySize);
	memset(releasedStates, 0, statesArraySize);
}

void Mouse::publicize()
{
	pCursorPosX = &cursorPosX;
	pCursorPosY = &cursorPosY;
	pCursorIsOnScreen = &cursorIsOnScreen;
	pHadBeenScrolled = &hadBeenScrolled;
	pPressedStates = pressedStates;
	pInitialPressedStates = initialPressStates;
	pReleasedStates = releasedStates;
	pScrollAmountX = &scrollAmountX;
	pScrollAmountY = &scrollAmountY;
	pSumScrollX = &sumScrollX;
	pSumScrollY = &sumScrollY;
}