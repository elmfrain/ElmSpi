#include "../render/GLUtil.h"
#include "GUISlider.h"
#include "../maths/Maths.h"

using namespace espi;

void GUISlider::setText(const char* text)
{
	int textLength = strlen(text);
	memset(this->text, 0, 256);
	memcpy(this->text, text, textLength > 255 ? 255 : textLength);
	this->text[255] = 0;
}
void GUISlider::setFont(Font& font)
{
	this->font = font;
}
GUISlider::GUISlider(const char* text, Font& font)
{
	this->font = font;
	setText(text);
}
GUISlider::GUISlider()
{

}
void GUISlider::update(const Window& window)
{
	track.disabled = track.disabled = disabled;
	track.visible = knob.visible = visible;
	prevAmount = amount;

	if (!disabled && visible)
	{
		windowWidth = window.getWidth();
		windowHeight = window.getHeight();

		vec4f cursor = vec4f(window.getMouse().posX(), window.getMouse().posY(), 0.0f, 1.0f);
		matrixf mat = inverse(modelMatrix * inverse(GLUtil::getGUIViewportMatrix(0, 0, window.getWidth(), window.getHeight())));
		cursor = cursor * mat;

		track.width = width;
		track.height = height - knobLedge * 2;
		track.x = x;
		track.y = y + knobLedge;

		knob.width = knobWidth;
		knob.height = height;
		knob.x = amount * (width - knobWidth) + x;
		knob.y = y;

		track.knobPressed = isKnobPressed();
		track.update(window);
		knob.update(window);

		if (knob.isPressed() && !disabled && visible)
		{
			if (cursor.x < x + knobWidth / 2)
			{
				amount = 0.0;
			}
			else if (cursor.x > x + width - knobWidth / 2)
			{
				amount = 1.0;
			}
			else
			{
				amount = (cursor.x - (x - 0.5 + knobWidth / 2.0)) / (width - knobWidth);
			}
		}
		if (track.justPressed() && !disabled && visible)
		{
			if (cursor.x < x + knobWidth / 2)
			{
				amount = 0.0;
			}
			else if (cursor.x > x + width - knobWidth / 2)
			{
				amount = 1.0;
			}
			else
			{
				amount = (cursor.x - (x - 0.5 + knobWidth / 2.0)) / (width - knobWidth);
				knob.click();
			}
		}
	}
}
void GUISlider::doRender()
{
	track.render();
	knob.render();

	if (text[0] != 0)
	{
		glPushMatrix();
		{
			if (!disabled) glColor3f(0.97f, 0.97f, 0.97f);
			else glColor3f(0.6f, 0.6f, 0.6f);
			glTranslatef(x + width / 2 - font.getStringWidth(text) / 2, y + height / 2 - font.fontHeight / 2, 0.0f);
			font.renderString(text);
		}
		glPopMatrix();
	}
}
void GUISlider::render()
{
	modelMatrix = GLUtil::getModelViewMatrix();
	if(visible) doRender();
}
bool GUISlider::isKnobMoving() const
{
	return prevAmount != amount;
}
bool GUISlider::isSliderHovered() const
{
	return track.isHovered() || knob.isHovered();
}
bool GUISlider::isKnobHovered() const
{
	return knob.isHovered();
}
bool GUISlider::isKnobPressed() const
{
	return knob.isPressed();
}
bool GUISlider::knobJustPressed() const
{
	return knob.justPressed();
}
bool GUISlider::isKnobReleased() const
{
	return knob.isReleased();
}
bool GUISlider::knobReleasedInside() const
{
	return knob.isReleasedInside();
}
bool GUISlider::knobReleasedOutside() const
{
	return knob.isReleasedOutside();
}