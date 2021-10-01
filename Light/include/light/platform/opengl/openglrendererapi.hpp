#ifndef __OPENGLRENDERERAPI_H__
#define __OPENGLRENDERERAPI_H__

#include "core/base.hpp"
#include "light/rendering/rendererapi.hpp"

namespace Light
{
	class OpenGLRendererAPI : public RendererAPI
	{
		void init() override;
		void depthMask(bool enable) override;
		void setViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		void setClearColor(glm::vec4& color) override;
		void clear() override;

		void drawIndexed(const std::shared_ptr<VertexArray>& vao) override;
	};

}

#endif // __OPENGLRENDERERAPI_H__

