#include "rendering/renderer.hpp"
#include "rendering/rendercommand.hpp"

#include "platform/opengl/openglshader.hpp"

namespace Light
{
	Renderer::SceneData* Renderer::sceneData = new Renderer::SceneData;

	void Renderer::init() 
	{
		RenderCommand::init();
	}

	void Renderer::onWindowResize(uint32_t width, uint32_t height) 
	{
		RenderCommand::setViewPort(0, 0, width, height);
	}

	void Renderer::beginScene(Camera& camera)
	{
		sceneData->viewProjectionMatrix = camera.getViewProjectionMatrix();
	}
	
	void Renderer::endScene() 
	{
		
	}
	
	void Renderer::submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vao, glm::mat4 transform) 
	{
		vao->bind();
		
		shader->bind();

		std::dynamic_pointer_cast<OpenGLShader>(shader)->setUniformMat4("u_viewProjectionMatrix", sceneData->viewProjectionMatrix);

		std::dynamic_pointer_cast<OpenGLShader>(shader)->setUniformMat4("u_transform", transform);

		RenderCommand::drawIndexed(vao);

		shader->unbind();

		vao->unbind();
	}
	
}