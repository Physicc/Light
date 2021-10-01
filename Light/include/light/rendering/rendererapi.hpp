#ifndef __RENDERERAPI_H__
#define __RENDERERAPI_H__

#include "core/base.hpp"

#include "light/rendering/vertexarray.hpp"

namespace Light
{
	class RendererAPI
	{
	public:
		virtual void init() = 0;
		virtual void depthMask(bool enable) = 0;
		virtual void setViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void setClearColor(glm::vec4& color) = 0;
		virtual void clear() = 0;

		virtual void drawIndexed(const std::shared_ptr<VertexArray>& vao) = 0;
	};
}

#endif // __RENDERERAPI_H__

