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

	void Renderer::beginScene(Camera& camera, glm::vec3 lightPos)
	{
		sceneData->viewProjectionMatrix = camera.getViewProjectionMatrix();
		sceneData->lightPos = lightPos;

		glm::mat4 view = glm::mat4(glm::mat3(camera.getViewMatrix()));
		sceneData->viewProjectionSkyboxMatrix = camera.getProjectionMatrix() * view;
	}
	
	void Renderer::endScene() 
	{
		
	}
	
	void Renderer::submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vao, glm::mat4 transform) 
	{
		vao->bind();
		
		shader->bind();

		std::dynamic_pointer_cast<OpenGLShader>(shader)->setUniformMat4("u_viewProjectionMatrix", sceneData->viewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->setUniformVec3("u_lightPos", sceneData->lightPos);

		std::dynamic_pointer_cast<OpenGLShader>(shader)->setUniformMat4("u_transform", transform);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->setUniformMat3("u_normal", glm::mat3(glm::transpose(glm::inverse(transform))));

		RenderCommand::drawIndexed(vao);

		shader->unbind();

		vao->unbind();
	}

	void Renderer::submitSkybox(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vao) 
	{
		vao->bind();
		
		shader->bind();

		std::dynamic_pointer_cast<OpenGLShader>(shader)->setUniformMat4("u_viewProjectionMatrix", sceneData->viewProjectionSkyboxMatrix);

		RenderCommand::depthMask(false);

		RenderCommand::drawIndexed(vao);

		RenderCommand::depthMask(true);

		shader->unbind();

		vao->unbind();
	}
	
}