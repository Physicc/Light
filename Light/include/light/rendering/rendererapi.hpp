#ifndef __RENDERERAPI_H__
#define __RENDERERAPI_H__

#include "core/base.hpp"

#include "light/rendering/vertexarray.hpp"
#include "light/rendering/framebuffer.hpp"

namespace Light
{

	enum class BlendFactor
	{
		ZERO,
		ONE,
		SRC_COLOR,
		ONE_MINUS_SRC_COLOR,
		DST_COLOR,
		ONE_MINUS_DST_COLOR,
		SRC_ALPHA,
		ONE_MINUS_SRC_ALPHA,
		DST_ALPHA,
		ONE_MINUS_DST_ALPHA,
		CONSTANT_COLOR,
		ONE_MINUS_CONSTANT_COLOR,
		CONSTANT_ALPHA,
		ONE_MINUS_CONSTANT_ALPHA,
	};

	class RendererAPI
	{
	public:
		virtual void init() = 0;
		virtual void depthMask(bool enable) = 0;
		virtual void setViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void setClearColor(glm::vec4& color) = 0;
		virtual void clear() = 0;
		virtual void setBlendFunc(BlendFactor src, BlendFactor dst) = 0;
		virtual void setBlendFuncSeperate(BlendFactor srcRGB, BlendFactor dstRGB, BlendFactor srcAlpha, BlendFactor dstAlpha) = 0;

		virtual void drawIndexed(const std::shared_ptr<VertexArray>& vao) = 0;

		virtual void framebufferBlit(const std::shared_ptr<Framebuffer>& src, const std::shared_ptr<Framebuffer>& dst, bool depth) = 0;
	};
}

#endif // __RENDERERAPI_H__
