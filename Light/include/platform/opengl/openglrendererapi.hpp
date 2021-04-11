#ifndef __OPENGLRENDERERAPI_H__
#define __OPENGLRENDERERAPI_H__

#include "core/base.hpp"
#include "rendering/rendererapi.hpp"

namespace Light
{
	class OpenGLRendererAPI : public RendererAPI
	{
		void setClearColor(glm::vec4& color) override;
		void clear() override;

		void drawIndexed(const std::shared_ptr<VertexArray>& vao) override;
	};
	
}

#endif // __OPENGLRENDERERAPI_H__