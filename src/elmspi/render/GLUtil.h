#pragma once
#define GLEW_STATIC
#include "GL/glew.h"
#include "../window/Window.h"

namespace GLUtil
{
	void initOpenGL(Window* window);
}