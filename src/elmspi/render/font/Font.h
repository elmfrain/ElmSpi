#pragma once
#include <string>

class Font
{
	public:
		const char fontName[256] = {};
		const int fontHeight = -1;

		Font(const char* fontInfoFile);
		Font(const Font&);
		~Font();
		int getStringWidth(const char* text);
		int getStringHeight();
		void renderString(const char* text);
		void operator=(const Font&);
	private:
		static struct FontChar
		{
			int x = -1;
			int y = -1;
			int width = -1;
			int height = -1;
			int xOffset = -1;
			int yOffset = -1;
			int xAdvance = -1;
		};
		int atlasWidth;
		int atlasHeight;
		mutable bool isBeingCopied = false;
		FontChar* chars = new FontChar[256];
		unsigned int stringVertexBuffer = 0;
		unsigned int fontAtlasTexture = 0;

		FontChar getFontChar(char c);
		std::string loadFontInfo(const char* fontInfoFile);
		void preRender();
		void postRender();
};

