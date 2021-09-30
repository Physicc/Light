#include "ecs/scene.hpp"
#include "ecs/entity.hpp"
#include "ecs/components.hpp"
#include "core/timestep.hpp"
#include "core/input.hpp"
#include "input/keycodes.hpp"
#include "light/rendering/renderer.hpp"
#include "light/rendering/buffer.hpp"

namespace Light
{
	Scene::Scene() 
	{
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

		m_skybox.reset(Light::Cubemap::create("../Editor/assets/cubemap"));
		m_skybox_shader = Light::Shader::create("../Editor/assets/shaders/skybox.glsl");
		m_skybox_shader->bind();
		m_skybox_shader->setUniformInt("u_cubemap", 0);

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

		m_outline_shader = Light::Shader::create("../Editor/assets/shaders/outline.glsl");
		m_outline_shader->bind();
		m_outline_shader->setUniformInt("ColorTexture", 0);
		m_outline_shader->setUniformInt("IDTexture", 1);

		m_outline_temp_shader = Light::Shader::create("../Editor/assets/shaders/outline-temp.glsl");
	}

	Entity Scene::addEntity(const std::string& name)
	{
		Entity entity = {m_registry.create(), this};
		auto& tag = entity.addComponent<TagComponent>();
		tag.tag = name.empty() ? "New Entity" : name;
		entity.addComponent<TransformComponent>();
		return entity;
	}

	void Scene::removeEntity(Entity entity) 
	{
		m_registry.destroy((entt::entity)(uint32_t)entity);
	}
	

	void Scene::update(Timestep dt)
	{
		
	}

	void Scene::render()
	{
		{
			auto view = m_registry.view<LightComponent, TransformComponent>();
			for(auto& entity: view)
			{
				auto[light, transform] = view.get(entity);
				Renderer::submitLight(transform.position, light.m_lightColor);
			}
		}

		// Render Skybox
		m_skybox->bind();
		Renderer::submitSkybox(m_skybox_shader, m_skybox_mesh);
		
		// Render entities
		{
			auto view = m_registry.view<MeshRendererComponent, MeshComponent, TransformComponent>();
			for (auto& entity : view)
			{
				auto [shader, mesh, transform] = view.get(entity);
				Renderer::submitID(shader.shader, mesh.mesh->getVao(), transform.getTransform(), (uint32_t)entity);

			}
		}
	}

	void Scene::renderSelection(Entity entity)
	{
		if(entity && entity.hasComponent<TransformComponent>() && entity.hasComponent<MeshComponent>())
		{
			auto [transform, mesh]= m_registry.get<TransformComponent, MeshComponent>((entt::entity)(uint32_t)entity);
			Renderer::submit(m_outline_temp_shader, mesh.mesh->getVao(), transform.getTransform());
		}
	}

	void Scene::renderOutline(Entity entity)
	{
		m_outline_shader->bind();
		m_outline_shader->setUniformInt("ColorTexture", 0);
		if(entity)
			m_outline_shader->setUniformInt("selectionID", uint32_t(entity));
		else
			m_outline_shader->setUniformInt("selectionID", -1);
		Renderer::submit(m_outline_shader, m_outline_mesh);
	}
}