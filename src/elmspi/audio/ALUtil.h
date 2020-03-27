#pragma once

#include <alc.h>
#include <al.h>
#include <iostream>

extern "C"
{
#include <libswresample/swresample.h>
#include <libavutil/opt.h>
}

#define AL_DEBUG

#ifdef AL_DEBUG
#define alE(x)\
	ALUtil::clearALErrors();\
	x;\
	if(ALUtil::getALError()) __debugbreak();
#endif

namespace ALUtil
{
	static void clearALErrors() { while (alGetError() != AL_NO_ERROR); }
	/*Returns true if an error is detected.*/
	static bool getALError()
	{
		while (ALenum error = alGetError())
		{
			std::string errorName;
			switch (error)
			{
			case AL_INVALID_ENUM:
				errorName = "Invalid Enum"; break;
			case AL_INVALID_VALUE:
				errorName = "Invalid Value"; break;
			case AL_INVALID_OPERATION:
				errorName = "Invalid Operation"; break;
			case AL_INVALID_NAME:
				errorName = "Invalid Name"; break;
			case AL_OUT_OF_MEMORY:
				errorName = "Out of Memory"; break;
			default:
				errorName = "Unknown Error"; break;
			}
			std::cout << "[OpenAL] : [Error] : " << errorName << std::endl;
			return true;
		}
		return false;
	}

	uint8_t** convertAudio(uint8_t** src, uint8_t srcNbChannels, int sampleRate, AVSampleFormat srcFmt, size_t nbOfSamples, uint8_t dstNbChannels, int dstSampleRate, AVSampleFormat dstFmt);

	void deleteAVBuffer(uint8_t** buffer);

	ALCdevice* initOpenAL();
	ALCcontext* createContext();
	void destoryContext(ALCcontext* context);
	void terminateOpenAL();

	struct AudioBuffer
	{
	protected:
		mutable bool isBeingCopied = false;
	public:
		size_t samples;
		size_t sampleRate;
		size_t channels;
		size_t bytesPerChannel;
		uint8_t** data = nullptr;
		AVSampleFormat format;

		AudioBuffer() {}
		AudioBuffer(size_t samples, size_t sampleRate, AVSampleFormat format, size_t channels = 1)
		{
			int lineSize;
			av_samples_alloc_array_and_samples(&data, &lineSize, channels, samples, format, 0);
			this->channels = channels;
			this->samples = samples;
			this->sampleRate = sampleRate;
			this->format = format;
			this->bytesPerChannel = lineSize;
		}
		AudioBuffer(uint8_t** audioData, int lineSize, size_t samples, size_t sampleRate, AVSampleFormat format, size_t channels)
		{
			this->data = audioData;
			this->channels = channels;
			this->samples = samples;
			this->sampleRate = sampleRate;
			this->format = format;
			this->bytesPerChannel = lineSize;
		}
		void operator=(const AudioBuffer& bufferCopy)
		{
			bufferCopy.isBeingCopied = true;
			data = bufferCopy.data;
			channels = bufferCopy.channels;
			samples = bufferCopy.samples;
			sampleRate = bufferCopy.sampleRate;
			format = bufferCopy.format;
			bytesPerChannel = bufferCopy.bytesPerChannel;
		}
		~AudioBuffer()
		{
			if (!isBeingCopied && data != nullptr)
			{
				deleteAVBuffer(data);
			}
		}
	};
	/*Creates an ALbuffer for each channel, and creates a stereo ALbuffer if buffer has 2 channels and is allowed.
	*/
	void createALBuffers(const AudioBuffer& buffer, unsigned int* values, size_t size, bool allowStereo = false);

	AudioBuffer convertAudio(AudioBuffer& src, uint8_t dstNbChannels, int dstSampleRate, AVSampleFormat dstFmt);

	AudioBuffer copyAudioSnip(const AudioBuffer& buffer, unsigned int fromSample, unsigned int toSample);
}
