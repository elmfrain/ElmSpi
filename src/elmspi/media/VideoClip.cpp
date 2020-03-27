#include "VideoClip.h"

void VideoClip::setupForDecoding()
{
	AVCodec* videoCodec = avcodec_find_decoder(videoStream->codecpar->codec_id);

	videoCodecContext = avcodec_alloc_context3(videoCodec);
	avcodec_parameters_to_context(videoCodecContext, videoStream->codecpar);
	avcodec_open2(videoCodecContext, videoCodec, NULL);

	imageWidth = videoStream->codecpar->width;
	imageHeight = videoStream->codecpar->height;
	frameRate = av_q2d(videoStream->r_frame_rate);
	duration = formatContext->duration / (double) AV_TIME_BASE;

	avPacket = av_packet_alloc();
	avFrame = av_frame_alloc();

	reformatterContext = sws_alloc_context();

	AVPixelFormat format = (AVPixelFormat)videoStream->codecpar->format;
	AVPixelFormat pixFormat;
	switch (format) {
	case AV_PIX_FMT_YUVJ420P:
		pixFormat = AV_PIX_FMT_YUV420P;
		break;
	case AV_PIX_FMT_YUVJ422P:
		pixFormat = AV_PIX_FMT_YUV422P;
		break;
	case AV_PIX_FMT_YUVJ444P:
		pixFormat = AV_PIX_FMT_YUV444P;
		break;
	case AV_PIX_FMT_YUVJ440P:
		pixFormat = AV_PIX_FMT_YUV440P;
		break;
	default:
		pixFormat = format;
		break;
	}
	int dummy[4];
	int srcRange, dstRange;
	int brightness, contrast, saturation;
	sws_getColorspaceDetails(reformatterContext, (int**)&dummy, &srcRange, (int**)&dummy, &dstRange, &brightness, &contrast, &saturation);
	const int* coefs = sws_getCoefficients(SWS_CS_DEFAULT);
	srcRange = 1; // this marks that values are according to yuvj
	sws_setColorspaceDetails(reformatterContext, coefs, srcRange, coefs, dstRange, brightness, contrast, saturation);

	reformatterContext = sws_getContext(imageWidth, imageHeight, pixFormat, imageWidth, imageHeight, AV_PIX_FMT_RGB0, SWS_BITEXACT, NULL, NULL, NULL);

	av_image_alloc(videoFrameData, videoFrameLineSizes, imageWidth, imageHeight, AV_PIX_FMT_RGB0, 1);
}

VideoClip::~VideoClip()
{
	avcodec_free_context(&videoCodecContext);
	av_packet_free(&avPacket);
	av_frame_free(&avFrame);
	sws_freeContext(reformatterContext);
}

void VideoClip::nextFrame()
{
	if (!readFrame(videoCodecContext, avPacket, avFrame, videoStreamIndex))
	{
		timePos += 1.0 / frameRate;
		sws_scale(reformatterContext, (const uint8_t* const*)avFrame->data, avFrame->linesize, 0, avFrame->height, videoFrameData, videoFrameLineSizes);
	}
}

void VideoClip::seekFrame(double timeStamp)
{
	long stamp = timeStamp / av_q2d(videoStream->time_base);
	//long stamp = timeStamp * frameRate;
	av_packet_unref(avPacket);
	av_seek_frame(formatContext, videoStreamIndex, stamp, AVSEEK_FLAG_BACKWARD);
	readFrame(videoCodecContext, avPacket, avFrame, videoStreamIndex);
	readFrame(videoCodecContext, avPacket, avFrame, videoStreamIndex);

	timePos = avPacket->dts * av_q2d(videoStream->time_base);
	videoCodecContext->frame_number = timePos * frameRate;

	bool eof = false;
	while (videoCodecContext->frame_number < timeStamp * frameRate && !eof) eof = readFrame(videoCodecContext, avPacket, avFrame, videoStreamIndex);

	timePos = videoCodecContext->frame_number / frameRate;
	sws_scale(reformatterContext, (const uint8_t* const*)avFrame->data, avFrame->linesize, 0, avFrame->height, videoFrameData, videoFrameLineSizes);
}

void VideoClip::flush()
{
}

int VideoClip::getVideoWidth()
{
	return imageWidth;
}

int VideoClip::getVideoHeight()
{
	return imageHeight;
}

int VideoClip::getFrameNumber()
{
	//double frameTimeStamp = avPacket->dts * av_q2d(videoStream->time_base);
	//return frameTimeStamp * frameRate;
	return videoCodecContext->frame_number;
}

double VideoClip::getFrameRate()
{
	return frameRate;
}

double VideoClip::getDuration()
{
	return duration;
}

double VideoClip::getTimePos()
{
	return timePos;
}

unsigned char* VideoClip::getVideoFrame()
{
	return this->videoFrameData[0];
}