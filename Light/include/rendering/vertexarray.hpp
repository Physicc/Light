#ifndef __VERTEXARRAY_H__
#define __VERTEXARRAY_H__

#include "core/base.hpp"
#include "rendering/buffer.hpp"

namespace Light
{
	class VertexArray
	{
		public:
		VertexArray() {}
		virtual ~VertexArray() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vbo) = 0;
		virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer>& ibo) = 0;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffers() const = 0;
		virtual const std::shared_ptr<IndexBuffer>& getIndexBuffer() const = 0;

		static VertexArray* create();
	};
	
}

#endif // __VERTEXARRAY_H__