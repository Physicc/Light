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
		inline static void drawIndexed(const std::shared_ptr<VertexArray>& vao) { rendererApi->drawIndexed(vao); }
		inline static void clear() { rendererApi->clear(); }
		inline static void setClearColor(glm::vec4 color) { rendererApi->setClearColor(color); }
		
	private:
		static RendererAPI* rendererApi;
	};
}

#endif // __RENDERCOMMAND_H__