#ifndef __OPENGLBUFFER_H__
#define __OPENGLBUFFER_H__

#include "core/base.hpp"
#include "rendering/buffer.hpp"

namespace Light
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void bind() const override;
		virtual void unbind() const override;
	private:
		uint32_t m_rendererId;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void bind() const override;
		virtual void unbind() const override;

		uint32_t getCount() const override;
	private:
		uint32_t m_rendererId;
		uint32_t m_count;
	};
	
}

#endif // __OPENGLBUFFER_H__