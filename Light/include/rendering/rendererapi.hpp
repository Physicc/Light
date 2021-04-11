#ifndef __RENDERERAPI_H__
#define __RENDERERAPI_H__

#include "core/base.hpp"
#include "rendering/vertexarray.hpp"

namespace Light
{
	class RendererAPI
	{
	public:
		virtual void init() = 0;
		virtual void setClearColor(glm::vec4& color) = 0;
		virtual void clear() = 0;

		virtual void drawIndexed(const std::shared_ptr<VertexArray>& vao) = 0;
	};
}

#endif // __RENDERERAPI_H__