#pragma once
#include "MediaFile.h"

class AudioClip : public MediaFile
{
	public:
		AudioClip(const char* mediaFileName) : MediaFile(mediaFileName)
		{
			if (mediaType != MediaType::AUDIO && mediaType != MediaType::VIDEO_AND_AUDIO)
			{
				std::cerr << "[AudioClip] : The file '" << mediaFileName << "' is not an audio file!" << std::endl;
				exit(15);
			}
		}
		~AudioClip();

		bool isStereo();
		int getSampleRate();
		double getDuration();

		void decodeAudio();

		unsigned int audioDataSize();
		short* getAudioData();
	private:
		bool stereo;
		int channels;
		int sampleRate;
		double duration;

		AVCodecContext* audioCodecContext;
		AVFrame* avFrame;
		AVPacket* avPacket;

		unsigned int audioBufferSize;
		unsigned int audioSampleIndex;
		short* audioBuffer;

		void setupForDecoding();
};

