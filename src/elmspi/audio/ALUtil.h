#pragma once

#include <alc.h>

namespace ALUtil
{
	void initOpenAL();
	ALCcontext* createContext();
	void destoryContext(ALCcontext* context);
	void terminateOpenAL();
}
