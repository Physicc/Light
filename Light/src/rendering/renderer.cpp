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
		s_sceneData->cameraPosition = -glm::vec3(camera_view[3] * view);
	}

	void Renderer::beginScene(glm::mat4 viewProjectionMatrix, glm::vec3 position)
	{
		s_sceneData->viewProjectionMatrix = viewProjectionMatrix;

		// glm::mat4 view = glm::mat4(glm::mat3(camera_view));
		// s_sceneData->viewProjectionSkyboxMatrix = camera.getProjectionMatrix() * view;
		s_sceneData->cameraPosition = position;
		
	}

	void Renderer::endScene()
	{
	}

	void Renderer::submitLight(const std::vector<PointLight> &lights)
	{
		s_sceneData->pointLights = lights;
	}

	void Renderer::submitLight(const std::vector<SpotLight> &lights)
	{
		s_sceneData->spotLights = lights;
	}

	void Renderer::submitLight(const std::vector<DirectionalLight> &lights)
	{
		s_sceneData->directionalLights = lights;
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
		shader->setUniformMat4("u_transform", transform);

		shader->setUniformInt("u_id", id);
		shader->setUniformInt("u_n_dLights", (int)s_sceneData->directionalLights.size());
		shader->setUniformInt("u_n_pLights", (int)s_sceneData->pointLights.size());
		shader->setUniformInt("u_n_sLights", (int)s_sceneData->spotLights.size());

		shader->setUniformVec3("cameraPos", s_sceneData->cameraPosition);

		

		for (unsigned int i = 0; i < 4; i++)
		{
			if (i < s_sceneData->directionalLights.size())
			{
				shader->setUniformInt("u_dirLights[" + std::to_string(i) + "].depthMap", i + 4);
				shader->setUniformVec4("u_dirLights[" + std::to_string(i) + "].color", glm::vec4(s_sceneData->directionalLights[i].color, 1.0));
				shader->setUniformVec3("u_dirLights[" + std::to_string(i) + "].direction", s_sceneData->directionalLights[i].direction);
				shader->setUniformMat4("u_dirLights[" + std::to_string(i) + "].lightSpaceMatrix", s_sceneData->directionalLights[i].getSpaceMatrix());
			}
			else
			{
				shader->setUniformInt("u_dirLights[" + std::to_string(i) + "].depthMap", i + 4);
				//Note: Skybox texture(Cubemap) is currently bound to tex unit 0 so Sampler2D depthMap cannot have value 0
				//Possible fix: unbind texture after use
			}
		}
		for (unsigned int i = 0; i < s_sceneData->pointLights.size(); i++)
		{
			shader->setUniformInt("u_pointLights[" + std::to_string(i) + "].depthCubemap", i + 8);
			shader->setUniformVec4("u_pointLights[" + std::to_string(i) + "].color", glm::vec4(s_sceneData->pointLights[i].color, 1.0));
			shader->setUniformVec3("u_pointLights[" + std::to_string(i) + "].position", s_sceneData->pointLights[i].position);
			shader->setUniformFloat("u_pointLights[" + std::to_string(i) + "].far_plane", s_sceneData->pointLights[i].range);
		}
		for (unsigned int i = 0; i < s_sceneData->spotLights.size(); i++)
		{
			shader->setUniformInt("u_spotLights[" + std::to_string(i) + "].depthCubemap", i + 12);
			shader->setUniformVec4("u_spotLights[" + std::to_string(i) + "].color", glm::vec4(s_sceneData->spotLights[i].color, 1.0));
			shader->setUniformVec3("u_spotLights[" + std::to_string(i) + "].position", s_sceneData->spotLights[i].position);
			shader->setUniformVec3("u_spotLights[" + std::to_string(i) + "].direction", s_sceneData->spotLights[i].direction);
			shader->setUniformFloat("u_spotLights[" + std::to_string(i) + "].far_plane", s_sceneData->spotLights[i].range);
			shader->setUniformFloat("u_spotLights[" + std::to_string(i) + "].innerCutoff", s_sceneData->spotLights[i].innerCutoff);
			shader->setUniformFloat("u_spotLights[" + std::to_string(i) + "].outerCutoff", s_sceneData->spotLights[i].outerCutoff);
		}

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

	void Renderer::submitForDirectionalShadow(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vao, glm::mat4 lightSpaceMatrix, glm::mat4 transform)
	{
		vao->bind();

		shader->bind();
		
		// (void)lightSpaceMatrix;
		shader->setUniformMat4("u_lightSpaceMatrix", lightSpaceMatrix);

		shader->setUniformMat4("u_transform", transform);

		RenderCommand::drawIndexed(vao);

		shader->unbind();

		vao->unbind();
	}
	void Renderer::submitForCubeShadow(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vao, PointLight light, glm::mat4 transform)
	{
		vao->bind();

		shader->bind();
		
		for (unsigned int i = 0; i < 6; ++i)
            shader->setUniformMat4("shadowMatrices[" + std::to_string(i) + "]", light.getSpaceMatrices()[i]);

		shader->setUniformMat4("u_transform", transform);
		shader->setUniformFloat("far_plane", light.range);
		shader->setUniformVec3("lightPos", light.position);

		RenderCommand::drawIndexed(vao);

		shader->unbind();

		vao->unbind();
	}
	void Renderer::submitForCubeShadow(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vao, SpotLight light, glm::mat4 transform)
	{
		vao->bind();

		shader->bind();

		for (unsigned int i = 0; i < 6; ++i)
			shader->setUniformMat4("shadowMatrices[" + std::to_string(i) + "]", light.getSpaceMatrices()[i]);

		shader->setUniformMat4("u_transform", transform);
		shader->setUniformFloat("far_plane", light.range);
		shader->setUniformVec3("lightPos", light.position);

		RenderCommand::drawIndexed(vao);

		shader->unbind();

		vao->unbind();
	}
}
