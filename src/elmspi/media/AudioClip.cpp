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
}

void AudioClip::decodeAudio()
{
	setupForDecoding();
	size_t samples = duration * sampleRate + 128;
	size_t bytesPerSample = av_get_bytes_per_sample(audioCodecContext->sample_fmt);
	audioBuffer = new ALUtil::AudioBuffer(samples, sampleRate, audioCodecContext->sample_fmt, channels);

	if (audioCodecContext->channel_layout != AV_CH_LAYOUT_MONO && audioCodecContext->channel_layout != AV_CH_LAYOUT_STEREO)
	{
		std::cout << "[AudioClip] : Wierd Channel Layout :/\n"; 
		return;
	}

	unsigned int samplePos = 0;
	while (!readFrame(audioCodecContext, avPacket, avFrame, audioStreamIndex))
	{
		for (int sample = 0; sample < avFrame->nb_samples; sample++)
		{
			for (int channel = 0; channel < channels; channel++)
			{
				for (int byte = 0; byte < bytesPerSample; byte++)
				{
					audioBuffer->data[channel][samplePos * bytesPerSample + byte] = avFrame->data[channel][sample * bytesPerSample + byte];
				}
			}
			samplePos++;
		}
	}
}

AudioClip::~AudioClip()
{
	avcodec_free_context(&audioCodecContext);
	av_packet_free(&avPacket);
	av_frame_free(&avFrame);
	delete audioBuffer;
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
ALUtil::AudioBuffer& AudioClip::getAudioBuffer()
{
	return *audioBuffer;
}
