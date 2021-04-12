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
		inline static void init() { rendererApi->init(); }
		inline static void setViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) 
		{ 
			rendererApi->setViewPort(x, y, width, height); 
		}

		inline static void depthMask(bool enable) { rendererApi->depthMask(enable); }

		inline static void drawIndexed(const std::shared_ptr<VertexArray>& vao) { rendererApi->drawIndexed(vao); }
		inline static void clear() { rendererApi->clear(); }
		inline static void setClearColor(glm::vec4 color) { rendererApi->setClearColor(color); }
		
	private:
		static RendererAPI* rendererApi;
	};
}

#endif // __RENDERCOMMAND_H__