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
		void removeEntity(Entity entity);
		void update(Light::Timestep dt);

	private:
		entt::registry m_registry;

		std::shared_ptr<Light::Cubemap> m_skybox;

		friend class Entity;
		friend class ScenePanel;
		friend class SceneRenderer;
	};
}

#endif // __SCENE_HPP__
