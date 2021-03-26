#ifndef __OPENGLVERTEXARRAY_H__
#define __OPENGLVERTEXARRAY_H__

#include "core/base.hpp"
#include "rendering/vertexarray.hpp"
#include "rendering/buffer.hpp"

namespace Light
{
	class OpenGLVertexArray : public VertexArray
	{
		public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		void bind() const override;
		void unbind() const override;

		void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vbo) override;
		void setIndexBuffer(const std::shared_ptr<IndexBuffer>& ibo) override;

		const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffers() const override { return vertexBuffers; }
		const std::shared_ptr<IndexBuffer>& getIndexBuffer() const override { return indexBuffer; }

	private:
		uint32_t rendererId;

		std::vector<std::shared_ptr<VertexBuffer>> vertexBuffers;
		std::shared_ptr<IndexBuffer> indexBuffer;
	};	

}

#endif // __OPENGLVERTEXARRAY_H__