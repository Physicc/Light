#include "light/platform/opengl/openglbuffer.hpp"

#include "glad/gl.h"

namespace Light
{
	VertexBuffer* VertexBuffer::create(float* vertices, uint32_t size)
	{
		return new OpenGLVertexBuffer(vertices, size);
	}

	IndexBuffer* IndexBuffer::create(uint32_t* indices, uint32_t count)
	{
		return new OpenGLIndexBuffer(indices, count);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
	{
		glGenBuffers(1, &m_rendererId);
		bind();

		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count) : m_count(count)
	{
		glGenBuffers(1, &m_rendererId);
		bind();

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_rendererId);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_rendererId);
	}

	void OpenGLVertexBuffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
	}

	void OpenGLVertexBuffer::unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLIndexBuffer::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId);
	}

	void OpenGLIndexBuffer::unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	uint32_t OpenGLIndexBuffer::getCount() const
	{
		return m_count;
	}
}
