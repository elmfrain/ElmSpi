#pragma once
#include "../window/Window.h"
#include "../render/font/Font.h"
#include "../maths/Maths.h"

class GUIButton
{
	public:
		int x = 0;
		int y = 0;
		int width = 0;
		int height = 0;
		bool disabled = false;
		bool visible = true;

		GUIButton(const char* text, Font& font);
		GUIButton();
		void render();
		void update(const Window& window);
		void setText(const char* text);
		void setFont(Font& font);
		/*Manual Clicking*/
		void click();
		/*Manual Releasing*/
		void release();
		bool isHovered() const;
		bool isPressed() const;
		bool justPressed() const;
		bool isReleased() const;
		bool isReleasedOutside() const;
		bool isReleasedInside() const;
	private:
		bool hovered = false;
		bool pressed = false;
		bool initiallyPressed = false;
		bool released = false;
	protected:
		char text[256];
		Font font = Font("");
		int windowWidth = 0;
		int windowHeight = 0;
		espi::matrixf modelMatrix = espi::matrixf(4, 4);
		virtual void doRender();
};

