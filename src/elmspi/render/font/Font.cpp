#include "Font.h"
#include <string>
#include <GL/glew.h>
#include "../../media/ImageFile.h"
#include <fstream>

static bool doesFileExist(const char* fileName)
{
	struct stat buffer;
	return (stat(fileName, &buffer) == 0);
}

static bool isCharNumerical(const char& theChar)
{
	return theChar == '-' || theChar == '.' || (47 < theChar && theChar < 58);
}

static int fetchNumberString(const char* string, char* buffer)
{
	memset(buffer, 0, 32);
	uint8_t pos = 0;
	char c = string[pos];
	uint8_t charsCopied = 0;
	while (!isCharNumerical(c)) c = string[pos++];
	do
	{
		buffer[charsCopied++] = c;
		pos++;
		c = string[pos];
	} while (charsCopied < 32 && isCharNumerical(c));
	return charsCopied;
}

Font::Font(const char* fontAtlasFile, const char* fontInfoFile)
{
	if (!doesFileExist(fontInfoFile)) throw std::invalid_argument("[Font] : The file for font data loading does not exist!");

	loadFontInfo(fontInfoFile);

	ImageFile image(fontAtlasFile);
	image.decodeImage();
	glGenTextures(1, &fontAtlasTexture);
	glBindTexture(GL_TEXTURE_2D, fontAtlasTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	atlasWidth = image.getImageWidth();
	atlasHeight = image.getImageHeight();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getImageWidth(), image.getImageHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getImage());
}

Font::~Font()
{
	delete[] chars;
	if (stringVertexBuffer) glDeleteBuffers(1, &stringVertexBuffer);
	glDeleteTextures(1, &fontAtlasTexture);
}

Font::FontChar Font::getFontChar(char c)
{
	if (chars[c].x == -1) return chars[0];
	return chars[c];
}

void Font::loadFontInfo(const char* fontInfoFile)
{
	std::ifstream fontInfo(fontInfoFile, std::ios::in | std::ios::ate);
	size_t fileSize = fontInfo.tellg();
	fontInfo.seekg(0, std::ios::beg);
	char* fontInfoData = new char[fileSize];
	fontInfo.read(fontInfoData, fileSize);
	fontInfo.close();

	char* line = fontInfoData;
	char numberAsString[32] = {};
	int lineSize = 0;
	int maxChars = 0;
	for (unsigned int i = 0; i < fileSize; i++)
	{
		char c = fontInfoData[i];
		if (c == '\n')
		{
			if (std::string("info") == std::string(line, 4))
			{
				char* charLine = line + 4;
				while (charLine - line < lineSize)
				{
					if (std::string("face=") == std::string(charLine, 5))
					{
						int stringLength = 0;
						charLine += 5;
						if (*charLine == '"') 
						{
							charLine++; 
							while (*charLine != '"') { stringLength++; charLine++; }
							memcpy((void*) fontName, charLine - stringLength, stringLength > 255 ? 255 : stringLength);
						}
					}
					charLine++;
				}
			}
			if (std::string("common") == std::string(line, 6))
			{
				char* charLine = line + 6;
				while (charLine - line < lineSize)
				{
					if (std::string("lineHeight=") == std::string(charLine, 11))
					{
						charLine += 11;
						fetchNumberString(charLine, numberAsString);
						*((int*)&fontHeight) = atoi(numberAsString);
					}
					charLine++;
				}
			}
			if (std::string("char id=") == std::string(line, 8))
			{
				char* charLine = line + 8;
				charLine += fetchNumberString(charLine, numberAsString);
				int charID = atoi(numberAsString);
				
				while (charLine - line < lineSize)
				{
					if (std::string("x=") == std::string(charLine, 2))
					{
						charLine += 2;
						charLine += fetchNumberString(charLine, numberAsString);
						chars[charID].x = atoi(numberAsString);
					}
					if (std::string("y=") == std::string(charLine, 2))
					{
						charLine += 2;
						charLine += fetchNumberString(charLine, numberAsString);
						chars[charID].y = atoi(numberAsString);
					}
					if (std::string("width=") == std::string(charLine, 6))
					{
						charLine += 6;
						charLine += fetchNumberString(charLine, numberAsString);
						chars[charID].width = atoi(numberAsString);
					}
					if (std::string("height=") == std::string(charLine, 7))
					{
						charLine += 7;
						charLine += fetchNumberString(charLine, numberAsString);
						chars[charID].height = atoi(numberAsString);
					}
					if (std::string("xoffset=") == std::string(charLine, 8))
					{
						charLine += 8;
						charLine += fetchNumberString(charLine, numberAsString);
						chars[charID].xOffset = atoi(numberAsString);
					}
					if (std::string("yoffset=") == std::string(charLine, 8))
					{
						charLine += 8;
						charLine += fetchNumberString(charLine, numberAsString);
						chars[charID].yOffset = atoi(numberAsString);
					}
					if (std::string("xadvance=") == std::string(charLine, 9))
					{
						charLine += 9;
						charLine += fetchNumberString(charLine, numberAsString);
						chars[charID].xAdvance = atoi(numberAsString);
					}
					charLine++;
				}
			}

			line = &fontInfoData[i] + 1;
			lineSize = 0;
		}
		else lineSize++;
	}
	delete[] fontInfoData;
}

void Font::renderString(const char* text)
{
	int textLength = strlen(text);
	int validChars = textLength;
	float* vertData = new float[textLength * 28];
	int cursorX = 0;
	int cursorY = 0;
	int pos = 0;

	for (int chr = 0; chr < textLength; chr++)
	{
		if (text[chr] == '\n')
		{
			cursorX = 0;
			cursorY += fontHeight;
			validChars--;
			continue;
		}
		FontChar fChar = getFontChar(text[chr]);
		if (fChar.width == 0 && fChar.height == 0)
		{
			validChars--;
			continue;
		}

		int p1x = cursorX + fChar.xOffset;
		int p1y = cursorY + fChar.yOffset;

		int p2x = cursorX + fChar.xOffset + fChar.width;
		int p2y = cursorY + fChar.yOffset + fChar.height;

		float t1x = fChar.x / (atlasWidth * 1.0f);
		float t1y = (fChar.y) / (atlasHeight * 1.0f);

		float t2x = (fChar.x + fChar.width) / (atlasWidth * 1.0f);
		float t2y = (fChar.y + fChar.height) / (atlasHeight * 1.0f);

		//Vertex 1
		vertData[pos++] = p1x;
		vertData[pos++] = p1y;
		vertData[pos++] = t1x;
		vertData[pos++] = t1y;
		vertData[pos++] = 0.0f; vertData[pos++] = 0.0f; vertData[pos++] = 1.0f;
		//Vertex 2
		vertData[pos++] = p2x;
		vertData[pos++] = p1y;
		vertData[pos++] = t2x;
		vertData[pos++] = t1y;
		vertData[pos++] = 0.0f; vertData[pos++] = 0.0f; vertData[pos++] = 1.0f;
		//Vertex 3
		vertData[pos++] = p2x;
		vertData[pos++] = p2y;
		vertData[pos++] = t2x;
		vertData[pos++] = t2y;
		vertData[pos++] = 0.0f; vertData[pos++] = 0.0f; vertData[pos++] = 1.0f;
		//Vertex 4
		vertData[pos++] = p1x;
		vertData[pos++] = p2y;
		vertData[pos++] = t1x;
		vertData[pos++] = t2y;
		vertData[pos++] = 0.0f; vertData[pos++] = 0.0f; vertData[pos++] = 1.0f;

		cursorX += fChar.xAdvance;
	}
	if (stringVertexBuffer) glDeleteBuffers(1, &stringVertexBuffer);
	glGenBuffers(1, &stringVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, stringVertexBuffer);
	{
		glBufferData(GL_ARRAY_BUFFER, textLength * 28 * sizeof(float), vertData, GL_STATIC_DRAW);
		delete[] vertData;
	}
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, fontAtlasTexture);
	preRender();
	glDrawArrays(GL_QUADS, 0, validChars * 4);
	postRender();
}

void Font::preRender()
{
	glEnable(GL_ALPHA);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, sizeof(float) * 7, (void*)(sizeof(float) * 2));

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, 7 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 7 * sizeof(float), (void*)(sizeof(float) * 2));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, true, 7 * sizeof(float), (void*)(sizeof(float) * 4));
}

void Font::postRender()
{
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

int Font::getStringHeight()
{
	return fontHeight;
}

int Font::getStringWidth(const char* text)
{
	int cursor = 0;

	for (int i = 0; i < strlen(text); i++)
	{
		FontChar fChar = getFontChar(text[i]);

		cursor += fChar.xAdvance;
	}

	return cursor;
}
