#include "../render/GLUtil.h"
#include "Window.h"
#include <iostream>
#include <map>
#include <string>

static std::map<GLFWwindow*, Window*> windows;
static bool glfwHasInit = false;
static bool onWindowCallback = false;
#define callback(x) onWindowCallback = true; x; onWindowCallback = false;

/*Window Hint*/
static bool windowHintResizable = true;

static void setWindowHints()
{
	if (!glfwHasInit)
	{
		if (!glfwInit()) exit(16);
		glfwHasInit = true;
	}

	glfwWindowHint(GLFW_RESIZABLE, windowHintResizable ? GLFW_TRUE : GLFW_FALSE);
}

int WindowUtil::getWindowsOpened()
{
	return windows.size();
}

Window* WindowUtil::getWindowByID(GLFWwindow* window)
{
	Window* value = nullptr;
	if (windows.find(window) != windows.end())
		value = windows[window];
	if (value == nullptr)
	{
		std::cout << "Couldn't find window!" << std::endl;
		exit(1);
	}
	return value;
}
void WindowUtil::setResizableHint(bool resizable) { windowHintResizable = resizable; }
void WindowUtil::shutdown()
{
	for (std::map<GLFWwindow*, Window*>::iterator i = windows.begin(); i != windows.end(); i++)
	{
		glfwDestroyWindow(i->first);
	}
	glfwTerminate();
}

namespace CallbackHandlers
{
	static void onCloseCallback(GLFWwindow* windowID)
	{
		Window* window = WindowUtil::getWindowByID(windowID);
		callback(window->setShouldClose(true));
	}
	static void onWindowPos(GLFWwindow* windowID, int posX, int posY)
	{
		Window* window = WindowUtil::getWindowByID(windowID);
		if (!window->isInFullScreen())
		{
			callback(window->setPos(posX, posY));
		}

	}
	static void onWindowSize(GLFWwindow* windowID, int width, int height)
	{
		Window* window = WindowUtil::getWindowByID(windowID);
		callback(window->setSize(width, height))
	}
}

Window::Window(int sizeX, int sizeY, const char* title)
{
	if (!glfwHasInit)
	{
		if (!glfwInit()) exit(16);
		glfwHasInit = true;
	}
	ALUtil::initOpenAL();

	this->width = sizeX;
	this->height = sizeY;
	this->windowWidth = sizeX;
	this->windowHeight = sizeY;
	this->fullscreen = false;
	this->needsToBeClosed = false;
	this->renderCallback = nullptr;
	this->vsyncActive = true;
	this->alContext = ALUtil::createContext();

	callback(setTitle(title));

	setWindowHints();
	this->windowID = glfwCreateWindow(width, height, this->title, nullptr, nullptr);
	centerWindow();

	keyboard = new Keyboard(windowID);
	mouse = new Mouse(windowID);

	glfwSetWindowCloseCallback(windowID, CallbackHandlers::onCloseCallback);
	glfwSetWindowPosCallback(windowID, CallbackHandlers::onWindowPos);
	glfwSetWindowSizeCallback(windowID, CallbackHandlers::onWindowSize);

	//Odd perfomance improvement doing this.
	setVsync(false);
	setVsync(true);

	windows[windowID] = this;
}

Window::~Window()
{
	ALUtil::destoryContext(this->alContext);
	if (windows.size() == 1) ALUtil::terminateOpenAL();
	windows.erase(windowID);
}

void Window::tick()
{
	if (renderCallback) renderCallback(this);
	keyboard->tick();
	mouse->tick();
	if (!fullscreen)
	{
		windowWidth = width;
		windowHeight = height;
	}
	glfwPollEvents();
	glfwSwapBuffers(windowID);
}

GLFWwindow* Window::getID() const
{
	return windowID;
}

Keyboard& Window::getKeyboard() const
{
	return *keyboard;
}

Mouse& Window::getMouse() const
{
	return *mouse;
}

void Window::makeThisContextCurrent() const
{
	glfwMakeContextCurrent(windowID);
	GLUtil::initOpenGL((Window*) this);
	alcMakeContextCurrent(alContext);
}

bool Window::usingVsync() const
{
	return vsyncActive;
}

bool Window::isFocused() const
{
	return glfwGetWindowAttrib(windowID, GLFW_FOCUSED);
}

void Window::setVsync(bool vsync)
{
	if (vsyncActive == vsync) return;
	vsyncActive = vsync;
	GLFWwindow* windowID = glfwGetCurrentContext();
	makeThisContextCurrent();
	if (vsyncActive) glfwSwapInterval(1);
	else glfwSwapInterval(0);
	glfwMakeContextCurrent(windowID);
}

bool Window::shouldClose() const
{
	return needsToBeClosed;
}

bool Window::isInFullScreen() const
{
	return fullscreen;
}

int Window::getWidth() const
{
	return width;
}

int Window::getHeight() const
{
	return height;
}

int Window::getPosX() const
{
	return posX;
}
int Window::getPosY() const
{
	return posY;
}

void Window::setTitle(const char* newTitle)
{
	memset(title, 0, TITLE_MAX_CHAR);
	int titleSize = std::strlen(newTitle) > TITLE_MAX_CHAR ? TITLE_MAX_CHAR - 1 : std::strlen(newTitle);
	for (int c = 0; c < titleSize; c++)
	{
		title[c] = newTitle[c];
	}
	if(!onWindowCallback) glfwSetWindowTitle(windowID, title);
}

void Window::setShouldClose(bool shouldClose)
{
	if (shouldClose == needsToBeClosed) return;
	needsToBeClosed = shouldClose;
	if(!onWindowCallback) glfwSetWindowShouldClose(windowID, shouldClose ? GLFW_TRUE : GLFW_FALSE);
}

void Window::setPos(int posX, int posY)
{
	if (this->posX == posX && this->posY == posY) return;
	this->posX = posX;
	this->posY = posY;
	if(!onWindowCallback) glfwSetWindowPos(windowID, posX, posY);
}

void Window::setSize(int width, int height)
{
	if (this->width == width && this->height == height) return;
	this->width = width;
	this->height = height;
	if (!onWindowCallback) glfwSetWindowSize(windowID, width, height);
}

void Window::setFullscreen(bool fullscreen)
{
	if (fullscreen == this->fullscreen) return;
	this->fullscreen = fullscreen;
	const GLFWvidmode* vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	if (fullscreen)
	{
		glfwSetWindowMonitor(windowID, glfwGetPrimaryMonitor(), 0, 0, vidMode->width, vidMode->height, vidMode->refreshRate);
		GLFWwindow* windowID = glfwGetCurrentContext();
		makeThisContextCurrent();
		if (vsyncActive) glfwSwapInterval(1);
		else glfwSwapInterval(0);
		glfwMakeContextCurrent(windowID);
	}
	else
		glfwSetWindowMonitor(windowID, nullptr, posX, posY, windowWidth, windowHeight, 0);
}

void Window::setRenderCallBack(Window::RenderCallbackFun renderFunction)
{
	renderCallback = renderFunction;
}

void Window::centerWindow()
{
	const GLFWvidmode* vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	this->posX = vidMode->width / 2 - width / 2;
	this->posY = vidMode->height / 2 - height / 2;
	glfwSetWindowPos(windowID, posX, posY);
}
