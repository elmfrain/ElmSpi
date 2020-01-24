#pragma once
#define GLEW_STATIC
#include "GL/glew.h"
#include "../maths/Maths.h"
#include "../window/Window.h"

namespace GLUtil
{
	void initOpenGL(Window* window);
	/**
		Important to call this function to display text properly.
	*/
	void setGUIViewPort(int x, int y, int width, int height);
	espi::matrixf getModelViewMatrix();
	espi::matrixf getProjectionMatrix();
	espi::matrixf getGUIViewportMatrix(int x, int y, int width, int height);
}