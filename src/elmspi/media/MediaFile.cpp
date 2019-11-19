#include "MediaFile.h"

MediaFile::MediaFile(const char* mediaFileName)
{
	*((MediaType*)&mediaType) = setupMedia(mediaFileName);
}

MediaFile::~MediaFile()
{
	avformat_free_context(formatContext);
}

/**
	Returns true if reached end of file.
*/
bool MediaFile::readPacket(AVCodecContext* decoder, AVPacket* packet, int streamIndex)
{
	int outcome = 1;
	bool eof = false;
	while (outcome > 0)
	{
		eof = av_read_frame(formatContext, packet) < 0 ? true : false;
		if (eof) break;
		if (packet->stream_index == streamIndex) outcome = 0;
	}
	return eof;
}

bool MediaFile::readFrame(AVCodecContext* decoder, AVPacket* packet, AVFrame* frame, int streamIndex)
{
	int outcome = 0;
	do
	{
		if (readPacket(decoder, packet, streamIndex)) return true;

		avcodec_send_packet(decoder, packet);
		outcome = avcodec_receive_frame(decoder, frame);

		if (outcome == AVERROR(EAGAIN)) outcome = 1;
	} while (outcome > 0);
	return false;
}

MediaType MediaFile::setupMedia(const char* mediaFileName)
{
	MediaType mediaType = MediaType::UNKNOWN;

	formatContext = avformat_alloc_context();

	if (avformat_open_input(&formatContext, mediaFileName, NULL, NULL) != 0)
	{

		throw std::invalid_argument("[MediaClip] : Unable to open  '" + (std::string)mediaFileName + "'!");
		return mediaType;
	}

	avformat_find_stream_info(formatContext, NULL);

	bool hasVideo, hasAudio, hasImageCodec;
	hasVideo = hasAudio = hasImageCodec = false;
	for (int i = 0; i < formatContext->nb_streams; i++)
	{
		AVStream* stream = formatContext->streams[i];
		AVCodecParameters* streamCodecParameter = stream->codecpar;

		if (streamCodecParameter->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			videoStream = stream;
			videoStreamIndex = i;
			hasVideo = true;
			mediaType = MediaType::VIDEO;

			for (int j = 0; j < MediaUtil::numOfImageCodecs; j++)
			{
				if (streamCodecParameter->codec_id == MediaUtil::imageCodecs[j])
				{
					hasImageCodec = true;
					break;
				}
				
			}
		}
		if (streamCodecParameter->codec_type == AVMEDIA_TYPE_AUDIO)
		{
			audioStream = stream;
			audioStreamIndex = i;
			hasAudio = true;
			mediaType = MediaType::AUDIO;
		}
	}

	if (hasVideo && hasAudio) mediaType = MediaType::VIDEO_AND_AUDIO;
	if (hasVideo && !hasAudio && hasImageCodec) mediaType = MediaType::IMAGE;

	return mediaType;
}