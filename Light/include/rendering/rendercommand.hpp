#ifndef __RENDERCOMMAND_H__
#define __RENDERCOMMAND_H__

#include "core/base.hpp"
#include "rendering/rendererapi.hpp"
#include "rendering/vertexarray.hpp"

namespace Light
{
	class RenderCommand
	{
	public:
		inline static void init() { s_rendererApi->init(); }
		inline static void setViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) 
		{ 
			s_rendererApi->setViewPort(x, y, width, height);
		}

		inline static void depthMask(bool enable) { s_rendererApi->depthMask(enable); }

		inline static void drawIndexed(const std::shared_ptr<VertexArray>& vao) { s_rendererApi->drawIndexed(vao); }
		inline static void clear() { s_rendererApi->clear(); }
		inline static void setClearColor(glm::vec4 color) { s_rendererApi->setClearColor(color); }
		
	private:
		static RendererAPI* s_rendererApi;
	};
}

#endif // __RENDERCOMMAND_H__