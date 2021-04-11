#include "rendering/renderer.hpp"
#include "rendering/rendercommand.hpp"

#include "platform/opengl/openglshader.hpp"

namespace Light
{
	Renderer::SceneData* Renderer::sceneData = new Renderer::SceneData;

	void Renderer::beginScene(Camera& camera)
	{
		sceneData->viewProjectionMatrix = camera.getViewProjectionMatrix();
	}
	
	void Renderer::endScene() 
	{
		
	}
	
	void Renderer::submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vao) 
	{
		vao->bind();
		
		shader->bind();

		std::dynamic_pointer_cast<OpenGLShader>(shader)->setUniformMat4("u_viewProjectionMatrix", sceneData->viewProjectionMatrix);

		RenderCommand::drawIndexed(vao);

		shader->unbind();

		vao->unbind();
	}
	
}