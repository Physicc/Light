#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include "entt.hpp"
#include "core/timestep.hpp"
#include "light/rendering/texture.hpp"
#include "light/rendering/shader.hpp"
#include "light/rendering/vertexarray.hpp"

namespace Light
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene() = default;
		Entity addEntity(const std::string& name="");
		void update(Light::Timestep dt);
		void render();
		void renderSelection(Entity entity);
		void renderOutline(Entity entity);
	private:
		entt::registry m_registry;

		std::shared_ptr<Light::Cubemap> m_skybox;
		std::shared_ptr<Light::Shader> m_skybox_shader;
		std::shared_ptr<Light::Shader> m_outline_shader;
		std::shared_ptr<Light::Shader> m_outline_temp_shader;
		std::shared_ptr<Light::VertexArray> m_skybox_mesh;
		std::shared_ptr<Light::VertexArray> m_outline_mesh;

		friend class Entity;
		friend class ScenePanel;
	};
	
}

#endif // __SCENE_HPP__
