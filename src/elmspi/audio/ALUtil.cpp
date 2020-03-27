#include "ALUtil.h"
extern "C"
{
#include "libavutil/avutil.h"
}

static ALCdevice* alDevice;

uint8_t** ALUtil::convertAudio(uint8_t** src, uint8_t srcNbChannels, int srcSampleRate, AVSampleFormat srcFmt, size_t nbOfSamples, uint8_t dstNbChannels, int dstSampleRate, AVSampleFormat dstFmt)
{
	SwrContext* swrContext = swr_alloc();
	uint8_t** dstData = nullptr;
	int dstLineSize;
	int64_t dstNbSamples = av_rescale_rnd(nbOfSamples, dstSampleRate, srcSampleRate, AVRounding::AV_ROUND_UP);

	uint64_t srcChnlLayout = 0xFFFFFFFFFFFFFFFF;
	uint64_t dstChnlLayout = 0xFFFFFFFFFFFFFFFF;
	if (srcNbChannels < 64 && dstNbChannels < 64)
	{
		srcChnlLayout = srcChnlLayout >> (64 - srcNbChannels);
		dstChnlLayout = dstChnlLayout >> (64 - dstNbChannels);
	}
	else
	{
		std::cout << "[AlUtil] : Cannot convert more than 64 channels!";
		return nullptr;
	}

	av_opt_set_int(swrContext, "in_channel_layout", srcChnlLayout, 0);
	av_opt_set_int(swrContext, "in_sample_rate", srcSampleRate, 0);
	av_opt_set_sample_fmt(swrContext, "in_sample_fmt", srcFmt, 0);

	av_opt_set_int(swrContext, "out_channel_layout", dstChnlLayout, 0);
	av_opt_set_int(swrContext, "out_sample_rate", dstSampleRate, 0);
	av_opt_set_sample_fmt(swrContext, "out_sample_fmt", dstFmt, 0);

	swr_init(swrContext);

	av_samples_alloc_array_and_samples(&dstData, &dstLineSize, srcNbChannels, dstNbSamples, dstFmt, 0);

	swr_convert(swrContext, dstData, dstNbSamples, (const uint8_t**) src, nbOfSamples);

	swr_free(&swrContext);

	return dstData;
}
ALUtil::AudioBuffer ALUtil::convertAudio(AudioBuffer& src, uint8_t dstNbChannels, int dstSampleRate, AVSampleFormat dstFmt)
{
	SwrContext* swrContext = swr_alloc();
	uint8_t** dstData = nullptr;
	int dstLineSize;
	int64_t dstNbSamples = av_rescale_rnd(src.samples, dstSampleRate, src.sampleRate, AVRounding::AV_ROUND_UP);

	uint64_t srcChnlLayout = 0xFFFFFFFFFFFFFFFF;
	uint64_t dstChnlLayout = 0xFFFFFFFFFFFFFFFF;
	if (src.channels < 64 && dstNbChannels < 64)
	{
		srcChnlLayout = srcChnlLayout >> (64 - src.channels);
		dstChnlLayout = dstChnlLayout >> (64 - dstNbChannels);
	}
	else
	{
		std::cout << "[AlUtil] : Cannot convert more than 64 channels!";
		return ALUtil::AudioBuffer();
	}

	av_opt_set_int(swrContext, "in_channel_layout", srcChnlLayout, 0);
	av_opt_set_int(swrContext, "in_sample_rate", src.sampleRate, 0);
	av_opt_set_sample_fmt(swrContext, "in_sample_fmt", src.format, 0);

	av_opt_set_int(swrContext, "out_channel_layout", dstChnlLayout, 0);
	av_opt_set_int(swrContext, "out_sample_rate", dstSampleRate, 0);
	av_opt_set_sample_fmt(swrContext, "out_sample_fmt", dstFmt, 0);

	swr_init(swrContext);

	av_samples_alloc_array_and_samples(&dstData, &dstLineSize, src.channels, dstNbSamples, dstFmt, 0);

	swr_convert(swrContext, dstData, dstNbSamples, (const uint8_t**)src.data, src.samples);

	swr_free(&swrContext);

	return ALUtil::AudioBuffer(dstData, dstLineSize, dstNbSamples, dstSampleRate, dstFmt, dstNbChannels);
}
ALUtil::AudioBuffer ALUtil::copyAudioSnip(const ALUtil::AudioBuffer& buffer, unsigned int fromSample, unsigned int toSample)
{
	if (toSample > buffer.samples) toSample = buffer.samples;
	if (fromSample > toSample) fromSample = 0;
	uint8_t** bufferData;
	int lineSize;
	uint32_t nbOfSamples = toSample - fromSample;
	av_samples_alloc_array_and_samples(&bufferData, &lineSize, buffer.channels, nbOfSamples, buffer.format, 0);
	int bytesPerSample = av_get_bytes_per_sample(buffer.format);

	int samplePos = 0;
	for (int sample = fromSample; sample < toSample; sample++)
	{
		for (int channel = 0; channel < buffer.channels; channel++)
		{
			for (int byte = 0; byte < bytesPerSample; byte++)
			{
				bufferData[channel][samplePos * bytesPerSample + byte] = buffer.data[channel][sample * bytesPerSample + byte];
			}
		}
		samplePos++;
	}

	return ALUtil::AudioBuffer(bufferData, lineSize, nbOfSamples, buffer.sampleRate, buffer.format, buffer.channels);
}

void ALUtil::deleteAVBuffer(uint8_t** buffer)
{
	av_freep(&buffer[0]);
	av_freep(&buffer);
}

ALCdevice* ALUtil::initOpenAL()
{
	if (alDevice == nullptr) alDevice = alcOpenDevice(nullptr);
	return alDevice;
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

void ALUtil::createALBuffers(const AudioBuffer& buffer, unsigned int* values, size_t size, bool allowStereo)
{
	uint8_t** audioData = buffer.data;
	if (buffer.format != AV_SAMPLE_FMT_S16P)
	{
		audioData = convertAudio(buffer.data, buffer.channels, buffer.sampleRate, buffer.format, buffer.samples, buffer.channels, buffer.sampleRate, AV_SAMPLE_FMT_S16P);
	}

	if (allowStereo && buffer.channels == 2 && size >= 1)
	{
		alE(alGenBuffers(1, values);)
		short* stereoBuffer = new short[buffer.samples * 2];
		int bufferPos = 0;
		for (int sample = 0; sample < buffer.samples; sample++)
		{
			stereoBuffer[bufferPos++] = ((short*)audioData[0])[sample];
			stereoBuffer[bufferPos++] = ((short*)audioData[1])[sample];
		}
		alE(alBufferData(values[0], AL_FORMAT_STEREO16, stereoBuffer, buffer.samples * sizeof(short) * 2, buffer.sampleRate);)
		delete[] stereoBuffer;
	}
	else
	{
		int buffersToProduce = size >= buffer.channels ? buffer.channels : size;
		alE(alGenBuffers(buffersToProduce, values);)
		for (int buf = 0; buf < buffersToProduce; buf++)
		{
			alE(alBufferData(values[buf], AL_FORMAT_MONO16, audioData[buf], buffer.samples * sizeof(short), buffer.sampleRate);)
		}
	}

	if (audioData != buffer.data) deleteAVBuffer(audioData);
}
