#include "AudioClip.h"

void AudioClip::setupForDecoding()
{
	AVCodec* audioCodec = avcodec_find_decoder(audioStream->codecpar->codec_id);

	audioCodecContext = avcodec_alloc_context3(audioCodec);
	avcodec_parameters_to_context(audioCodecContext, audioStream->codecpar);
	avcodec_open2(audioCodecContext, audioCodec, NULL);

	sampleRate = audioCodecContext->sample_rate;
	duration = formatContext->duration / (double)AV_TIME_BASE;
	channels = audioStream->codecpar->channels;
	stereo = channels == 2 ? true : false;

	avPacket = av_packet_alloc();
	avFrame = av_frame_alloc();

	audioBufferSize = duration * sampleRate * channels + 256;
	audioBuffer = new short[audioBufferSize];
	memset(audioBuffer, 0, audioBufferSize * sizeof(short));
}

void AudioClip::decodeAudio()
{
	setupForDecoding();

	while (!readFrame(audioCodecContext, avPacket, avFrame, audioStreamIndex))
	{
		if (audioCodecContext->sample_fmt == AV_SAMPLE_FMT_S16P)
		{
			for (int i = 0; i < avFrame->nb_samples; i++)
			{
				for (int channel = 0; channel < channels; channel++)
				{
					short sample = *((short*)(avFrame->data[channel] + i * sizeof(short)));
					audioBuffer[audioSampleIndex++] = sample;
				}
			}
		}
		if (audioCodecContext->sample_fmt == AV_SAMPLE_FMT_S16)
		{
			for (int i = 0; i < avFrame->nb_samples * channels; i++)
			{
				short sample = *((short*)(avFrame->data[0] + i * sizeof(short)));
				audioBuffer[audioSampleIndex++] = sample;
			}
		}
		if (audioCodecContext->sample_fmt == AV_SAMPLE_FMT_FLTP)
		{
			for (int i = 0; i < avFrame->nb_samples; i++)
			{
				for (int channel = 0; channel < channels; channel++)
				{
					float inputSample = *((float*)(avFrame->data[channel] + i * sizeof(float)));
					short sample = inputSample * 24575;
					audioBuffer[audioSampleIndex++] = sample;
				}
			}
		}
	}
}

AudioClip::~AudioClip()
{
	avcodec_free_context(&audioCodecContext);
	av_packet_free(&avPacket);
	av_frame_free(&avFrame);
	delete[] audioBuffer;
}

bool AudioClip::isStereo()
{
	return stereo;
}
int AudioClip::getSampleRate()
{
	return sampleRate;
}
double AudioClip::getDuration()
{
	return duration;
}
unsigned int AudioClip::audioDataSize()
{
	return audioBufferSize * sizeof(short);
}
short* AudioClip::getAudioData()
{
	return audioBuffer;
}
