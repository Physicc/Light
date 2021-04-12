#include "platform/opengl/openglrendererapi.hpp"

#include "rendering/rendercommand.hpp"

#include "glad/glad.h"

namespace Light
{
	RendererAPI* RenderCommand::rendererApi = new OpenGLRendererAPI;

	void OpenGLRendererAPI::init() 
	{
		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::setViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) 
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::setClearColor(glm::vec4& color) 
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}
	
	void OpenGLRendererAPI::clear() 
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	
	void OpenGLRendererAPI::drawIndexed(const std::shared_ptr<VertexArray>& vao) 
	{
		glDrawElements(GL_TRIANGLES, vao->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
	}
}