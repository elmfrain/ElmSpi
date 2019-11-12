#include "ALUtil.h"

static ALCdevice* alDevice;

void ALUtil::initOpenAL()
{
	if (alDevice == nullptr) alDevice = alcOpenDevice(nullptr);
}
void ALUtil::destoryContext(ALCcontext* context)
{
	alcDestroyContext(context);
}
ALCcontext* ALUtil::createContext()
{
	return alcCreateContext(alDevice, nullptr);
}
void ALUtil::terminateOpenAL()
{
	if (alDevice == nullptr) alcCloseDevice(alDevice);
	alDevice = nullptr;
}