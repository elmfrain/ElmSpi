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