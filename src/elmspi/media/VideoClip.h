#pragma once

#include "MediaFile.h"

class VideoClip : public MediaFile
{
	public:
		VideoClip(const char* mediaFileName) : MediaFile(mediaFileName)
		{
			if (mediaType != MediaType::VIDEO && mediaType != MediaType::VIDEO_AND_AUDIO)
			{
				std::cerr << "[VideoClip] : The file '" << mediaFileName << "' is not a video file!" << std::endl;
				exit(15);
			}

			setupForDecoding();
			nextFrame();
		}
		~VideoClip();
		void nextFrame();
		void seekFrame(double timeStamp);
		void flush();

		int getVideoWidth();
		int getVideoHeight();
		int getFrameNumber();
		double getFrameRate();
		double getDuration();
		double getTimePos();

		unsigned char* getVideoFrame();
	private:
		int imageWidth = -1;
		int imageHeight = -1;
		double frameRate = -1.0;
		double duration = -1.0;
		double timePos = 0.0;

		AVCodecContext* videoCodecContext;
		AVFrame* avFrame;
		AVPacket* avPacket;

		SwsContext* reformatterContext;
		uint8_t* videoFrameData[4];
		int videoFrameLineSizes[4];

		void setupForDecoding();
};