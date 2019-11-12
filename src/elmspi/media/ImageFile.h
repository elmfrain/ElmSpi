#pragma once
#include "MediaFile.h"

class ImageFile : public MediaFile
{
	public:

		ImageFile(const char* mediaFileName) : MediaFile(mediaFileName)
		{
			if (mediaType != MediaType::IMAGE)
			{
				std::cerr << "[ImageFile] : The file '" << mediaFileName << "' is not an image file!" << std::endl;
				exit(15);
			}
			
		}
		~ImageFile();
		void decodeImage();
		unsigned char* getImage();

		int getImageWidth();
		int getImageHeight();

	private:
		int imageWidth = -1;
		int imageHeight = -1;

		AVCodecContext* imageCodecContext;
		AVFrame* avFrame;
		AVPacket* avPacket;

		SwsContext* reformatterContext;
		uint8_t* imageData[4];
		int imageLineSizes[4];

		void setupForDecoding();
};

