#include "ImageFile.h"

void ImageFile::setupForDecoding()
{
	AVCodec* videoCodec = avcodec_find_decoder(videoStream->codecpar->codec_id);

	imageCodecContext = avcodec_alloc_context3(videoCodec);
	avcodec_parameters_to_context(imageCodecContext, videoStream->codecpar);
	avcodec_open2(imageCodecContext, videoCodec, NULL);

	imageWidth = videoStream->codecpar->width;
	imageHeight = videoStream->codecpar->height;

	avPacket = av_packet_alloc();
	avFrame = av_frame_alloc();

	reformatterContext = sws_alloc_context();
	av_image_alloc(imageData, imageLineSizes, imageWidth, imageHeight, AV_PIX_FMT_RGB0, 1);
}

ImageFile::~ImageFile()
{
	avcodec_free_context(&imageCodecContext);
	av_packet_free(&avPacket);
	av_frame_free(&avFrame);
	sws_freeContext(reformatterContext);
}

void ImageFile::decodeImage()
{
	setupForDecoding();
	
	readFrame(imageCodecContext, avPacket, avFrame, videoStreamIndex);

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
	sws_scale(reformatterContext, (const uint8_t* const*)avFrame->data, avFrame->linesize, 0, avFrame->height, imageData, imageLineSizes);
}

unsigned char* ImageFile::getImage()
{
	return this->imageData[0];
}

int ImageFile::getImageWidth()
{
	return imageWidth;
}

int ImageFile::getImageHeight()
{
	return imageHeight;
}
