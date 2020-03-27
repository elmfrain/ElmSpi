#pragma once
#include "GUIButton.h"

class GUISlider
{
	public:
		int x = 0;
		int y = 0;
		int width = 0;
		int height = 0;
		int knobWidth = 30;
		int knobLedge = 14;
		double amount = 0.0;
		bool visible = true;
		bool disabled = false;

		GUISlider(const char* text, Font& font);
		GUISlider();
		void render();
		void update(const Window& window);
		void setText(const char* text);
		void setFont(Font& font);

		bool isSliderHovered() const;
		bool isKnobHovered() const;
		bool isKnobPressed() const;
		bool isKnobMoving() const;
		bool knobJustPressed() const;
		bool isKnobReleased() const;
		bool knobReleasedOutside() const;
		bool knobReleasedInside() const;
	protected:
		typedef class GUISliderTrack : public GUIButton
		{
			public:
				bool knobPressed = false;
			void doRender()
			{
				glEnable(GL_ALPHA);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glDisable(GL_TEXTURE_2D);
				glDisable(GL_DEPTH_TEST);

				if((this->isHovered() || knobPressed) && !disabled) glColor4f(0.05f, 0.05f, 0.05f, 0.5f);
				else glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
				glBegin(GL_QUADS);
				{
					glVertex2f(x, y);
					glVertex2f(x + width, y);
					glVertex2f(x + width, y + height);
					glVertex2f(x, y + height);
				}
				glEnd();
			}
		};
		GUISliderTrack track;
		GUIButton knob;
		char text[256];
		Font* font = nullptr;
		int windowWidth = 0;
		int windowHeight = 0;
		espi::matrixf modelMatrix = espi::matrixf(4, 4);
		double prevAmount = 0.0;

		virtual void doRender();
};