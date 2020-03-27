#pragma once
#define GLEW_STATIC
#include "GL/glew.h"
#include "../maths/Maths.h"
#include "../window/Window.h"

#define GL_DEBUG

#ifdef GL_DEBUG
#define glE(x)\
	GLUtil::clearGLErrors();\
	x;\
	if(GLUtil::getGLError()) __debugbreak();
#endif


namespace GLUtil
{
	static void clearGLErrors() { while (glGetError() != GL_NO_ERROR); }
	/*Returns true if an error is detected.*/
	static bool getGLError()
	{
		while (GLenum error = glGetError())
		{
			std::string errorName;
			switch (error)
			{
			case GL_INVALID_ENUM:
				errorName = "Invalid Enum"; break;
			case GL_INVALID_VALUE:
				errorName = "Invalid Value"; break;
			case GL_INVALID_OPERATION:
				errorName = "Invalid Operation"; break;
			case GL_STACK_OVERFLOW:
				errorName = "Stack Overflow"; break;
			case GL_STACK_UNDERFLOW:
				errorName = "Stack Underflow"; break;
			case GL_OUT_OF_MEMORY:
				errorName = "Out Of Memory"; break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				errorName = "Invalid Framebuffer Operation"; break;
			case GL_CONTEXT_LOST:
				errorName = "Context Lost"; break;
			default:
				errorName = "Unknown Error"; break;
			}
			std::cout << "[OpenGL] : [Error] : " << errorName << std::endl;
			return true;
		}
		return false;
	}
	void initOpenGL(Window* window);
	/**
		Important to call this function to display text properly.
	*/
	void setGUIViewPort(int x, int y, int width, int height);
	espi::matrixf getModelViewMatrix();
	espi::matrixf getProjectionMatrix();
	espi::matrixf getGUIViewportMatrix(int x, int y, int width, int height);

	struct GLObject { protected: mutable bool beingCopied = false; };
	struct Texture2D : public GLObject
	{
		public:
			unsigned int id = 0;
			uint16_t width = 0;
			uint16_t height = 0;
			GLenum format = GL_RGBA;
			GLenum type = GL_UNSIGNED_BYTE;
			Texture2D(uint16_t width, uint16_t height, GLenum format = GL_RGBA, GLenum type = GL_UNSIGNED_BYTE)
			{
				this->width = width;
				this->height = height;
				this->format = format;
				this->type = type;

				glGenTextures(1, &id);
				glBindTexture(GL_TEXTURE_2D, id);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			Texture2D() {}
			~Texture2D() { if (!beingCopied && id != 0) glDeleteTextures(1, &id); }
			void operator=(const Texture2D& copy)
			{
				copy.beingCopied = true;
				id = copy.id;
				width = copy.width; height = copy.height; format = copy.format; type = copy.type;
			}
			void texImage(void* src, GLenum pixelFormat = GL_RGBA)
			{
				beingCopied = false;
				glBindTexture(GL_TEXTURE_2D, id);
				glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, pixelFormat, type, src);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			void bind() { beingCopied = false; glBindTexture(GL_TEXTURE_2D, id); }
			void unbind() { beingCopied = false; glBindTexture(GL_TEXTURE_2D, 0); }
			void texParameteri(GLenum parameter, GLint value) { bind(); glTexParameteri(GL_TEXTURE_2D, parameter, value); unbind(); }
			void generateMipmaps()
			{
				glBindTexture(GL_TEXTURE_2D, id);
				glGenerateMipmap(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
	};
	struct Framebuffer : public GLObject
	{
		public:
			unsigned int id = 0;
			Texture2D texture;
			Texture2D depth;
			Framebuffer(uint16_t width, uint16_t height, bool useDepth, GLenum format = GL_RGBA, GLenum type = GL_UNSIGNED_BYTE, GLenum depthFormat = GL_DEPTH_COMPONENT)
			{
				if (useDepth)
				{
					texture = Texture2D(width, height, format, type);
					depth = Texture2D(width, height, depthFormat);
					depth.texParameteri(GL_TEXTURE_COMPARE_MODE, GL_NONE);

					glE(texture.texImage(nullptr);)
					glE(depth.texImage(nullptr, GL_DEPTH_COMPONENT);)

					glGenFramebuffers(1, &id);
					glBindFramebuffer(GL_FRAMEBUFFER, id);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.id, 0);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth.id, 0);
					glBindFramebuffer(GL_FRAMEBUFFER, 0);
				}
				else
				{
					texture = Texture2D(width, height, format, type);

					texture.texImage(nullptr);

					glGenFramebuffers(1, &id);
					glBindFramebuffer(GL_FRAMEBUFFER, id);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.id, 0);
					glBindFramebuffer(GL_FRAMEBUFFER, 0);
				}

				int i = glCheckFramebufferStatus(GL_FRAMEBUFFER);
				if (i != GL_FRAMEBUFFER_COMPLETE) throw std::invalid_argument("Framebuffer acted wierd.");
			}
			Framebuffer() {}
			~Framebuffer()
			{
				if (!beingCopied && id != 0) glDeleteFramebuffers(1, &id);
			}
			void operator=(const Framebuffer& copy)
			{
				copy.beingCopied = true;
				id = copy.id;
				texture = copy.texture;
				depth = copy.depth;
			}
			void bind() { beingCopied = false; glBindFramebuffer(GL_FRAMEBUFFER, id); }
			void unbind() { beingCopied = false; glBindFramebuffer(GL_FRAMEBUFFER, 0); }
			void render()
			{
				glEnable(GL_TEXTURE_2D);
				texture.bind();
				glBegin(GL_QUADS);
				{
					glTexCoord2f(0.0f, 1.0f);
					glVertex2f(0, 0);

					glTexCoord2f(1.0f, 1.0f);
					glVertex2f(texture.width, 0);

					glTexCoord2f(1.0f, 0.0f);
					glVertex2f(texture.width, texture.height);

					glTexCoord2f(0.0f, 0.0f);
					glVertex2f(0, texture.height);
				}
				glEnd();
			}
	};
}