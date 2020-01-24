#include "../render/GLUtil.h"
#include "GUIButton.h"
#include "../maths/Maths.h"

using namespace espi;

GUIButton::GUIButton(const char* text, Font& font)
{
	this->font = Font(font);
	setText(text);
}

void GUIButton::setText(const char* text)
{
	int textLength = strlen(text);
	memset(this->text, 0, 256);
	memcpy(this->text, text, textLength > 255 ? 255 : textLength);
	this->text[255] = 0;
}

void GUIButton::setFont(Font& font)
{
	this->font = font;
}

GUIButton::GUIButton()
{

}

void GUIButton::update(const Window& window)
{
	initiallyPressed = false;
	released = false;

	if (!disabled && visible)
	{
		vec4f cursor = vec4f(window.getMouse().posX(), window.getMouse().posY(), 0.0f, 1.0f);
		matrixf mat = inverse(modelMatrix * inverse(GLUtil::getGUIViewportMatrix(0, 0, window.getWidth(), window.getHeight())));
		cursor = cursor * mat;

		windowWidth = window.getWidth();
		windowHeight = window.getHeight();

		if (x < cursor.x && y < cursor.y && cursor.x < x + width && cursor.y < y + height) hovered = true;
		else hovered = false;

		if (hovered && window.getMouse().leftJustPressed()) { pressed = true; initiallyPressed = true; }
		if (pressed && window.getMouse().leftJustReleased()) { released = true; pressed = false; }
	}
	else
	{
		pressed = initiallyPressed = false;
	}
}
bool GUIButton::isHovered() const
{
	return hovered;
}
bool GUIButton::isPressed() const
{
	return pressed;
}
bool GUIButton::justPressed() const
{
	return initiallyPressed;
}
bool GUIButton::isReleased() const
{
	return released;
}
bool GUIButton::isReleasedOutside() const
{
	return released && !hovered;
}
bool GUIButton::isReleasedInside() const
{
	return released && hovered;
}
void GUIButton::click()
{
	if (!disabled) pressed = initiallyPressed = true;
}
void GUIButton::release()
{
	if (!disabled && pressed) { released = true; pressed = false; }
}
void GUIButton::doRender()
{
	glEnable(GL_ALPHA);
	glEnable(GL_BLEND);
	glBlendEquation(GL_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_TEXTURE_2D);

	if(disabled) glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
	else if (pressed) glColor4f(0.5f, 0.5f, 0.5f, 0.5f); 
	else if (hovered) glColor4f(0.2f, 0.2f, 0.2f, 0.5f);
	else glColor4f(0.0f, 0.0f, 0.0f, 0.5f);

	glBegin(GL_QUADS);
	{
		glVertex2f(x, y);
		glVertex2f(x + width, y);
		glVertex2f(x + width, y + height);
		glVertex2f(x, y + height);
	}
	glEnd();

	glLineWidth(6);
	glColor4f(0.0f, 0.0f, 0.0f, 0.35f);

	glBegin(GL_LINE_STRIP);
	{
		glVertex2f(x, y);
		glVertex2f(x + width, y);
		glVertex2f(x + width, y + height);
		glVertex2f(x, y + height);
		glVertex2f(x, y);
	}
	glEnd();

	if (text[0] != 0)
	{
		glPushMatrix();
		{
			if (disabled) glColor3f(0.6f, 0.6f, 0.6f);
			else if (pressed) glColor3f(0.98f, 1.0f, 0.78f);
			else glColor3f(0.97f, 0.97f, 0.97f);
			glTranslatef(x + width / 2 - font.getStringWidth(text) / 2, y + height/ 2 - font.fontHeight / 2, 0.0f);
			font.renderString(text);
		}
		glPopMatrix();
	}
}
void GUIButton::render()
{
	modelMatrix = GLUtil::getModelViewMatrix();

	if(visible) doRender();
}