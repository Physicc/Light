#include "platform/opengl/openglframebuffer.hpp"

#include "glad/glad.h"

namespace Light
{
	std::shared_ptr<Framebuffer> Framebuffer::create(const FramebufferSpec& spec) 
	{
		return std::make_shared<OpenGLFramebuffer>(spec);
	}

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpec& spec)
		: m_spec(spec)
	{
		invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer() 
	{
		glDeleteFramebuffers(1, &m_rendererId);
		glDeleteTextures(1, &m_colorAttachment);
		glDeleteRenderbuffers(1, &m_depthAttachment);
	}

	void OpenGLFramebuffer::resize(uint32_t width, uint32_t height) 
	{
        m_spec.width = width;
        m_spec.height = height;

		invalidate();
	}

	void OpenGLFramebuffer::invalidate() 
	{
		if(m_rendererId == 0)
		{
			glDeleteTextures(1, &m_colorAttachment);
			glDeleteRenderbuffers(1, &m_depthAttachment);
		}

		glGenFramebuffers(1, &m_rendererId);
		glBindFramebuffer(GL_FRAMEBUFFER, m_rendererId);

		glGenTextures(1, &m_colorAttachment);
		glBindTexture(GL_TEXTURE_2D, m_colorAttachment);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_spec.width, m_spec.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorAttachment, 0);

		glGenRenderbuffers(1, &m_depthAttachment);
		glBindRenderbuffer(GL_RENDERBUFFER, m_depthAttachment);

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_spec.width, m_spec.height);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthAttachment);

		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cerr << " Framebuffer is incomplete!" << std::endl;
			exit(1);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::bind() 
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_rendererId);
		glViewport(0, 0, m_spec.width, m_spec.height);
	}
	
	void OpenGLFramebuffer::unbind() 
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
}