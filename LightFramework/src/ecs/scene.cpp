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
	}

	Entity Scene::addEntity()
	{
		return {m_registry.create(), this};
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
				Renderer::submitLight(transform.position);
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
				Renderer::submit(shader.shader, mesh.mesh, transform.getTransform());
			}
		}
	}
}