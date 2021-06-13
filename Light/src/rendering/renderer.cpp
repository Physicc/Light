#include "light/rendering/renderer.hpp"
#include "light/rendering/rendercommand.hpp"

namespace Light
{
	Renderer::SceneData* Renderer::s_sceneData = new Renderer::SceneData;

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
        s_sceneData->viewProjectionMatrix = camera.getViewProjectionMatrix();
        s_sceneData->lightPos = lightPos;

		glm::mat4 view = glm::mat4(glm::mat3(camera.getViewMatrix()));
        s_sceneData->viewProjectionSkyboxMatrix = camera.getProjectionMatrix() * view;
	}
	
	void Renderer::endScene() 
	{
		
	}
	
	void Renderer::submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vao, glm::mat4 transform) 
	{
		vao->bind();
		
		shader->bind();

		shader->setUniformMat4("u_viewProjectionMatrix", s_sceneData->viewProjectionMatrix);
		shader->setUniformVec3("u_lightPos", s_sceneData->lightPos);

		shader->setUniformMat4("u_transform", transform);
		shader->setUniformMat3("u_normal", glm::mat3(glm::transpose(glm::inverse(transform))));

		RenderCommand::drawIndexed(vao);

		shader->unbind();

		vao->unbind();
	}

	void Renderer::submitSkybox(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vao) 
	{
		vao->bind();
		
		shader->bind();

		shader->setUniformMat4("u_viewProjectionMatrix", s_sceneData->viewProjectionSkyboxMatrix);

		RenderCommand::depthMask(false);

		RenderCommand::drawIndexed(vao);

		RenderCommand::depthMask(true);

		shader->unbind();

		vao->unbind();
	}
	
}