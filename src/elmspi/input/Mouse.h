#pragma once
#include <GLFW/glfw3.h>

enum Buttons : unsigned char
{
	MOUSE_BUTTON_1 = 0, MOUSE_BUTTON_2 = 1, MOUSE_BUTTON_3 = 2,
	MOUSE_BUTTON_4 = 3, MOUSE_BUTTON_5 = 4, MOUSE_BUTTON_6 = 5,
	MOUSE_BUTTON_7 = 6, MOUSE_BUTTON_8 = 7, MOUSE_BUTTON_LAST = MOUSE_BUTTON_8,
	MOUSE_BUTTON_LEFT = MOUSE_BUTTON_1, MOUSE_BUTTON_RIGHT = MOUSE_BUTTON_2, MOUSE_BUTTON_MIDDLE = MOUSE_BUTTON_3
};

static const int statesArraySize = 8;

class Mouse
{
private:
	GLFWwindow* windowID;

	double cursorPosX, cursorPosY;
	double prevCursorPosX, prevCursorPosY;

	bool cursorIsOnScreen, hadBeenScrolled, cursorIsLocked;

	double scrollAmountX, scrollAmountY, sumScrollX, sumScrollY;

	bool pressedStates[statesArraySize];
	bool releasedStates[statesArraySize];

	void publicize();
public:
	Mouse(GLFWwindow* window);
	~Mouse();

	void setLockMouse(bool locked);

	double posX() const;
	double posY() const;
	double deltaPosX() const;
	double deltaPosY() const;

	double scrollX() const;
	double scrollY() const;
	double totalScrollX() const;
	double totalScrollY() const;

	bool leftJustReleased() const;
	bool leftIsPressed() const;
	bool rightJustReleased() const;
	bool rightIsPressed() const;
	bool middleJustReleased() const;
	bool middleIsPressed() const;
	bool buttonIsPressed(Buttons button) const;
	bool buttonJustReleased(Buttons button) const;

	bool cursorIsFocused() const;
	bool hasScrolled() const;
	bool isLocked() const;
	void resetScroll();

	void tick();
};