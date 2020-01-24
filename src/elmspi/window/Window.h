#pragma once
#include "../input/Keyboard.h"
#include "../input/Mouse.h"
#include "../audio/ALUtil.h"

static const int TITLE_MAX_CHAR = 1024;

class Window
{
public:
	typedef void (*RenderCallbackFun)(Window*);
private:
	RenderCallbackFun renderCallback;
	GLFWwindow* windowID;
	Keyboard* keyboard;
	Mouse* mouse;
	ALCcontext* alContext;
	char title[TITLE_MAX_CHAR];
	int width, height, windowWidth, windowHeight, posX, posY;
	bool fullscreen;
	bool needsToBeClosed;
	bool vsyncActive;

	void centerWindow();
public:
	Window(int sizeX, int sizeY, const char* title);
	~Window();
	void tick();
	GLFWwindow* getID() const;
	Keyboard& getKeyboard() const;
	Mouse& getMouse() const;
	void makeThisContextCurrent() const;
	bool shouldClose() const;
	bool isInFullScreen() const;
	bool usingVsync() const;
	bool isFocused() const;
	int getWidth() const;
	int getHeight() const;
	int getPosX() const;
	int getPosY() const;
	void setVsync(bool vsync);
	void setTitle(const char* newTitle);
	void setShouldClose(bool shouldClose);
	void setPos(int posX, int posY);
	void setSize(int width, int height);
	void setFullscreen(bool fullscreen);
	void setRenderCallBack(RenderCallbackFun);
};

namespace WindowUtil
{
	Window* getWindowByID(GLFWwindow* window);
	int getWindowsOpened();
	void setResizableHint(bool resizable);
	void shutdown();
};
