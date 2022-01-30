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

	void Renderer::beginScene(Camera& camera, glm::mat4 camera_view)
	{
		s_sceneData->viewProjectionMatrix = camera.getProjectionMatrix() * camera_view;

		glm::mat4 view = glm::mat4(glm::mat3(camera_view));
		s_sceneData->viewProjectionSkyboxMatrix = camera.getProjectionMatrix() * view;
	}
	
	void Renderer::endScene() 
	{
		
	}

	void Renderer::submitLight(const std::vector<PointLight> &lights) 
	{
		s_sceneData->pointLights = lights;
	}
	
	void Renderer::submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vao, glm::mat4 transform) 
	{
		submitID(shader, vao, transform);
	}

	void Renderer::submitID(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vao, glm::mat4 transform, int id) 
	{
		vao->bind();
		
		shader->bind();

		shader->setUniformMat4("u_viewProjectionMatrix", s_sceneData->viewProjectionMatrix);

		for (int i = 0; i < 4; i++)
		{
			if (i < (int)s_sceneData->pointLights.size())
			{
				shader->setUniformVec4("u_pointLights[" + std::to_string(i) + "].position", glm::vec4(s_sceneData->pointLights[i].position, 1.0));
				shader->setUniformVec4("u_pointLights[" + std::to_string(i) + "].color", glm::vec4(s_sceneData->pointLights[i].color, 1.0));
			}
			else
			{
				shader->setUniformVec4("u_pointLights[" + std::to_string(i) + "].position", glm::vec4(0.0, 0.0, 0.0, 1.0));
				shader->setUniformVec4("u_pointLights[" + std::to_string(i) + "].color", glm::vec4(0.0, 0.0, 0.0, 1.0));
			}
		}

		shader->setUniformInt("u_numPointLights", (int)s_sceneData->pointLights.size());
		shader->setUniformInt("u_id", id);

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
		shader->setUniformInt("u_cubemap", 0);

		RenderCommand::depthMask(false);

		RenderCommand::drawIndexed(vao);

		RenderCommand::depthMask(true);

		shader->unbind();

		vao->unbind();
	}
	
}