// #include "core/logging.hpp"
#include "rendering/scenerenderer.hpp"

#include "light/rendering/renderer.hpp"
#include "light/rendering/rendercommand.hpp"


namespace Light {
	#define TEXTURE_UNIT_START 4;

	SceneRenderer::SceneRenderer()
	{
		// Initialize the depth framebuffer
		Light::FramebufferSpec fbspecDepth;
		fbspecDepth.attachments = {
			{ Light::FramebufferTextureFormat::Depth, Light::TextureWrap::CLAMP_TO_BORDER }
		};
		fbspecDepth.width = 1280;
		fbspecDepth.height = 1280;
		for(int i=0;i<4;i++)
			m_depthFramebuffer[i] = Light::Framebuffer::create(fbspecDepth);

		Light::FramebufferSpec fbspecDepthCube;
		fbspecDepthCube.attachments = {
			{ Light::FramebufferTextureFormat::Depth, Light::TextureWrap::CLAMP_TO_BORDER }
		};
		fbspecDepthCube.width = 1280;
		fbspecDepthCube.height = 1280;
		fbspecDepthCube.type = Light::FramebufferTextureType::CUBEMAP;
		for (int i = 0; i < 4; i++)
		{
			m_pointDepthCubeFramebuffer[i] = Light::Framebuffer::create(fbspecDepthCube);
			m_spotDepthCubeFramebuffer[i] = Light::Framebuffer::create(fbspecDepthCube);
		}

		// Initialize the outline framebuffer
		Light::FramebufferSpec fbspecOutline;
		fbspecOutline.attachments = {
			{ Light::FramebufferTextureFormat::RED_INTEGER, Light::TextureWrap::CLAMP_TO_BORDER },
			{ Light::FramebufferTextureFormat::Depth, Light::TextureWrap::CLAMP_TO_BORDER }
		};
		fbspecOutline.width = 1280;
		fbspecOutline.height = 720;
		m_outlineFramebuffer = Light::Framebuffer::create(fbspecOutline);

		Light::FramebufferSpec fbspecTemp;
		fbspecTemp.attachments = {
			{ Light::FramebufferTextureFormat::RGBA8, Light::TextureWrap::CLAMP_TO_BORDER },
			{ Light::FramebufferTextureFormat::Depth, Light::TextureWrap::CLAMP_TO_BORDER }
		};
		fbspecTemp.width = 1280;
		fbspecTemp.height = 720;
		m_tempFramebuffer = Light::Framebuffer::create(fbspecTemp);

		m_depth_shader = Light::Shader::create("assets/shaders/depth.glsl");
		m_depth_cube_shader = Light::Shader::create("assets/shaders/depthCube.glsl");

		// Skybox Mesh (Cube)
		m_skybox_mesh.reset(VertexArray::create());

		float vertices[] = {
				//Front
				-1.0, -1.0, 1.0,
				1.0, -1.0, 1.0,
				1.0, 1.0, 1.0,
				-1.0, 1.0, 1.0,
				//Left
				-1.0, -1.0, 1.0,
				-1.0, 1.0, 1.0,
				-1.0, 1.0, -1.0,
				-1.0, -1.0, -1.0,
				//Right
				1.0, -1.0, 1.0,
				1.0, -1.0, -1.0,
				1.0, 1.0, -1.0,
				1.0, 1.0, 1.0,
				//Top
				-1.0, 1.0, 1.0,
				1.0, 1.0, 1.0,
				1.0, 1.0, -1.0,
				-1.0, 1.0, -1.0,
				//Bottom
				-1.0, -1.0, 1.0,
				-1.0, -1.0, -1.0,
				1.0, -1.0, -1.0,
				1.0, -1.0, 1.0,
				//Back
				-1.0, -1.0, -1.0,
				-1.0, 1.0, -1.0,
				1.0, 1.0, -1.0,
				1.0, -1.0, -1.0,
		};

		std::shared_ptr<VertexBuffer> vbo(VertexBuffer::create(vertices, sizeof(vertices)));

		vbo->setLayout(BufferLayout({
			{ ShaderDataType::Float3, "a_Position" }
		}));

		unsigned int indices[] = {
			0, 2, 1, 3, 2, 0,
			4, 6, 5, 7, 6, 4,
			8, 10, 9, 11, 10, 8,
			12, 14, 13, 15, 14, 12,
			16, 18, 17, 19, 18, 16,
			20, 22, 21, 23, 22, 20
		};

		std::shared_ptr<IndexBuffer> ibo(IndexBuffer::create(indices, sizeof(indices) / sizeof(unsigned int)));

		m_skybox_mesh->addVertexBuffer(vbo);
		m_skybox_mesh->setIndexBuffer(ibo);

		// Skybox Shader init
		m_skybox_shader = Light::Shader::create("assets/shaders/skybox.glsl");
		m_skybox_shader->bind();
		m_skybox_shader->setUniformInt("u_cubemap", 0);

		// Outline Mesh (Screen space quad)
		m_outline_mesh.reset(Light::VertexArray::create());

		float screen_vertices[] = {
			-1.0, -1.0,
			-1.0, 1.0,
			1.0, 1.0,
			1.0, -1.0
		};

		vbo.reset(VertexBuffer::create(screen_vertices, sizeof(screen_vertices)));

		vbo->setLayout({
			{ ShaderDataType::Float2, "a_Position" }
		});

		unsigned int screen_indices[] = {
			0, 2, 1, 3, 2, 0
		};

		ibo.reset(IndexBuffer::create(screen_indices, sizeof(screen_indices) / sizeof(unsigned int)));

		m_outline_mesh->addVertexBuffer(vbo);
		m_outline_mesh->setIndexBuffer(ibo);

		// Outline Shader init
		m_outline_shader = Light::Shader::create("assets/shaders/outline.glsl");
		m_outline_shader->bind();

		m_outline_temp_shader = Light::Shader::create("assets/shaders/outline-temp.glsl");
	}

	void SceneRenderer::onViewportResize(int width, int height)
	{
		m_outlineFramebuffer->resize(width, height);
		m_tempFramebuffer->resize(width, height);
	}

	void SceneRenderer::renderShadows(std::shared_ptr<Scene> scene, DirectionalLight light, int index)
	{
		m_depthFramebuffer[index]->bind();
		Light::RenderCommand::clearDepthBit();
		

		// Render depth of entities
		{
			auto view = scene->m_registry.view<MeshRendererComponent, MeshComponent, TransformComponent>();
			for (auto& entity : view)
			{
				auto [shader, mesh, transform] = view.get(entity);
				Renderer::submitForDirectionalShadow(m_depth_shader, mesh.mesh->getVao(), light.getSpaceMatrix(), transform.getTransform());
			}
		}
		Light::Renderer::endScene();
		m_depthFramebuffer[index]->unbind();
	}

	void SceneRenderer::renderShadows(std::shared_ptr<Scene> scene, PointLight light, int index)
	{
		m_pointDepthCubeFramebuffer[index]->bind();
		Light::RenderCommand::clearDepthBit();
		

		// Render depth of entities
		{
			auto view = scene->m_registry.view<MeshRendererComponent, MeshComponent, TransformComponent>();
			for (auto& entity : view)
			{
				auto [shader, mesh, transform] = view.get(entity);
				Renderer::submitForCubeShadow(m_depth_cube_shader, mesh.mesh->getVao(), light, transform.getTransform());
			}
		}
		Light::Renderer::endScene();
		m_pointDepthCubeFramebuffer[index]->unbind();
	}

	void SceneRenderer::renderShadows(std::shared_ptr<Scene> scene, SpotLight light, int index)
	{
		m_spotDepthCubeFramebuffer[index]->bind();
		Light::RenderCommand::clearDepthBit();


		// Render depth of entities
		{
			auto view = scene->m_registry.view<MeshRendererComponent, MeshComponent, TransformComponent>();
			for (auto& entity : view)
			{
				auto [shader, mesh, transform] = view.get(entity);
				Renderer::submitForCubeShadow(m_depth_cube_shader, mesh.mesh->getVao(), light, transform.getTransform());
			}
		}
		Light::Renderer::endScene();
		m_spotDepthCubeFramebuffer[index]->unbind();
	}

	void SceneRenderer::renderEditor(std::shared_ptr<Scene> scene, EditorCamera &camera)
	{
		std::vector<PointLight> pointLights;
		std::vector<SpotLight> spotLights;
		std::vector<DirectionalLight> directionalLights;
		{
			auto view = scene->m_registry.view<LightComponent, TransformComponent>();

			for (auto& entity : view)
			{

				auto [light, transform] = view.get(entity);
				switch (light.m_lightType)
				{
				case LightType::Directional:
					directionalLights.push_back({ transform.position,
													glm::normalize(transform.getTransform() * glm::vec4(0.0, 0.0, 1.0, 0.0)),
													light.m_lightColor});
					break;
				case LightType::Point:
					pointLights.push_back({ transform.position, light.m_lightColor, light.m_range });
					break;
				case LightType::Spot:
					spotLights.push_back({ transform.position,
											light.m_lightColor,
											glm::normalize(transform.getTransform() * glm::vec4(0.0, 0.0, 1.0, 0.0)),
											(float)glm::cos(glm::radians(light.m_inner)),
											(float)glm::cos(glm::radians(light.m_outer)),
											light.m_range });
					break;
				default:
					break;
				}
			}
		}
		LIGHT_CORE_ASSERT(directionalLights.size() <= 4, "Only 4 lights of each type supported now");
		LIGHT_CORE_ASSERT(pointLights.size() <= 4, "Only 4 lights of each type supported now");
		LIGHT_CORE_ASSERT(spotLights.size() <= 4, "Only 4 lights of each type supported now");

		for (unsigned int i = 0; i < directionalLights.size(); i++)
			renderShadows(scene, directionalLights[i], i);
		for (unsigned int i = 0; i < pointLights.size(); i++)
			renderShadows(scene, pointLights[i], i);
		for (unsigned int i = 0; i < spotLights.size(); i++)
			renderShadows(scene, spotLights[i], i);

		// Render scene
		m_framebuffer->bind();

		Light::RenderCommand::setClearColor({0.5f, 0.1f, 0.1f, 1.0f});
		Light::RenderCommand::clear();

		m_framebuffer->clearAttachment(1, 0);

		Light::Renderer::beginScene(camera, camera.getViewMatrix());


		Renderer::submitLight(directionalLights);
		Renderer::submitLight(pointLights);
		Renderer::submitLight(spotLights);

		// Render Skybox
		scene->m_skybox->bind();
		Renderer::submitSkybox(m_skybox_shader, m_skybox_mesh);

		// binding to texture units

		for (int i = 0; i < 4; i++)
		{
			m_depthFramebuffer[i]->bindDepthAttachmentTexture(i + 4);
			m_pointDepthCubeFramebuffer[i]->bindDepthAttachmentTexture(i + 8);
			m_spotDepthCubeFramebuffer[i]->bindDepthAttachmentTexture(i + 12);
		}
		// Render entities
		{
			auto view = scene->m_registry.view<MeshRendererComponent, MeshComponent, TransformComponent>();
			for (auto& entity : view)
			{
				auto [shader, mesh, transform] = view.get(entity);
				Renderer::submitID(shader.shader, mesh.mesh->getVao(), transform.getTransform(), (uint32_t)entity);
			}
		}

		Light::Renderer::endScene();
	}

	void SceneRenderer::setTargetFramebuffer(std::shared_ptr<Framebuffer> framebuffer)
	{
		m_framebuffer = framebuffer;
	}


	void SceneRenderer::renderOutline(std::shared_ptr<Scene> scene, Entity entity)
	{
		m_outlineFramebuffer->bind();
		m_outlineFramebuffer->clearAttachment(0, 0);
		m_outlineFramebuffer->clearDepthAttachment();
		if(entity && entity.hasComponent<TransformComponent>() && entity.hasComponent<MeshComponent>())
		{
			auto [transform, mesh]= scene->m_registry.get<TransformComponent, MeshComponent>((entt::entity)(uint32_t)entity);
			Renderer::submit(m_outline_temp_shader, mesh.mesh->getVao(), transform.getTransform());
		}
		m_outlineFramebuffer->unbind();

		RenderCommand::framebufferBlit(m_framebuffer, m_tempFramebuffer, true);

		m_framebuffer->bind();
		m_outlineFramebuffer->bindAttachmentTexture(0,0);
		m_outlineFramebuffer->bindDepthAttachmentTexture(2);
		m_tempFramebuffer->bindDepthAttachmentTexture(1);
		m_outline_shader->bind();
		m_outline_shader->setUniformInt("IDTexture", 0);
		m_outline_shader->setUniformInt("DepthTexture", 1);
		m_outline_shader->setUniformInt("SelectedDepth", 2);
		RenderCommand::setBlendFuncSeperate(BlendFactor::SRC_ALPHA, BlendFactor::ONE_MINUS_SRC_ALPHA, BlendFactor::ZERO, BlendFactor::ONE);
		Renderer::submit(m_outline_shader, m_outline_mesh);
		RenderCommand::setBlendFunc(BlendFactor::SRC_ALPHA, BlendFactor::ONE_MINUS_SRC_ALPHA);
		m_framebuffer->unbind();
	}
}
