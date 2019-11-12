#pragma once

#include <iostream>
#include <fstream>

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
}

namespace MediaUtil
{
	const int numOfImageCodecs = 11;
	const AVCodecID imageCodecs[numOfImageCodecs] = {AV_CODEC_ID_JPEG2000, AV_CODEC_ID_TIFF, AV_CODEC_ID_BMP, AV_CODEC_ID_PNG, AV_CODEC_ID_PPM, AV_CODEC_ID_PGM,
													 AV_CODEC_ID_PBM, AV_CODEC_ID_WEBP, AV_CODEC_ID_TARGA, AV_CODEC_ID_JPEGLS, AV_CODEC_ID_MJPEG};
}

enum MediaType : char
{
	UNKNOWN, VIDEO, AUDIO, VIDEO_AND_AUDIO, IMAGE
};

class MediaFile
{
	protected:
		static const int readingBufferSize = 8192 + AV_INPUT_BUFFER_PADDING_SIZE;

		AVFormatContext* formatContext;

		AVStream* videoStream;
		int videoStreamIndex = -1;
		AVStream* audioStream;
		int audioStreamIndex = -1;

		/**
			Returns true if reached end of file.
		*/
		bool readFrame(AVCodecContext* decoder, AVPacket* packet, AVFrame* frame, int streamIndex);
	public:
		const MediaType mediaType = UNKNOWN;
		MediaFile(const char* mediaFileName);
		~MediaFile();
	private:
		MediaType setupMedia(const char* mediaFileName);
		bool readPacket(AVCodecContext* decoder, AVPacket* packet, int streamIndex);
};

