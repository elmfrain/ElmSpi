#pragma once
#include "MediaFile.h"
#include "../audio/ALUtil.h"

class AudioClip : public MediaFile
{
	public:
		AudioClip(const char* mediaFileName) : MediaFile(mediaFileName)
		{
			if (mediaType != MediaType::AUDIO && mediaType != MediaType::VIDEO_AND_AUDIO)
			{
				throw std::invalid_argument("[AudioClip] : The file '" + (std::string)mediaFileName + "' is not an audio file!");
				exit(15);
			}
		}
		~AudioClip();

		bool isStereo();
		int getSampleRate();
		double getDuration();

		void decodeAudio();

		ALUtil::AudioBuffer& getAudioBuffer();
	private:
		bool stereo;
		int channels;
		int sampleRate;
		double duration;

		AVCodecContext* audioCodecContext;
		AVFrame* avFrame;
		AVPacket* avPacket;

		unsigned int audioSampleIndex;
		ALUtil::AudioBuffer* audioBuffer;

		void setupForDecoding();
};

