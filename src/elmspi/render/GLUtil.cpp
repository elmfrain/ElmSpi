#include "GLUtil.h"
#include <stdlib.h>
#include <map>

std::map<Window*, bool> windowStates;

void GLUtil::initOpenGL(Window* window)
{
	if (windowStates.find(window) != windowStates.end() && windowStates[window] == true) return;
	if (glewInit() != GLEW_OK) exit(18);
	windowStates[window] = true;
}

void GLUtil::setGUIViewPort(int x, int y, int width, int height)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_LIGHTING);
	glLoadIdentity();
	glScalef(2.0f / width, -2.0f / height, 1.0f);
	glTranslatef(-width / 2.0f + x, -height / 2.0f + y, 0.0f);
}

espi::matrixf GLUtil::getModelViewMatrix()
{
	float matData[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, matData);
	return espi::matrixf(matData, 4, 4);
}
