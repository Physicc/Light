#include "platform/opengl/openglframebuffer.hpp"

#include "glad/glad.h"

namespace Light
{
	std::shared_ptr<Framebuffer> Framebuffer::create(const FramebufferSpec& spec) 
	{
		return std::make_shared<OpenGLFramebuffer>(spec);
	}

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpec& spec)
		: spec(spec)
	{
		invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer() 
	{
		glDeleteFramebuffers(1, &rendererId);
		glDeleteTextures(1, &colorAttachment);
		glDeleteRenderbuffers(1, &depthAttachment);
	}

	void OpenGLFramebuffer::resize(uint32_t width, uint32_t height) 
	{
		spec.width = width;
		spec.height = height;

		invalidate();
	}

	void OpenGLFramebuffer::invalidate() 
	{
		if(rendererId == 0)
		{
			glDeleteTextures(1, &colorAttachment);
			glDeleteRenderbuffers(1, &depthAttachment);
		}

		glGenFramebuffers(1, &rendererId);
		glBindFramebuffer(GL_FRAMEBUFFER, rendererId);

		glGenTextures(1, &colorAttachment);
		glBindTexture(GL_TEXTURE_2D, colorAttachment);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, spec.width, spec.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachment, 0);

		glGenRenderbuffers(1, &depthAttachment);
		glBindRenderbuffer(GL_RENDERBUFFER, depthAttachment);

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, spec.width, spec.height);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthAttachment);

		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			LIGHT_CORE_ERROR("Framebuffer is incomplete!");
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::bind() 
	{
		glBindFramebuffer(GL_FRAMEBUFFER, rendererId);
		glViewport(0, 0, spec.width, spec.height);
	}
	
	void OpenGLFramebuffer::unbind() 
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
}