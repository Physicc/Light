#include "ecs/scene.hpp"
#include "ecs/entity.hpp"
#include "ecs/components.hpp"
#include "core/timestep.hpp"
#include "core/input.hpp"
#include "input/keycodes.hpp"
#include "light/rendering/renderer.hpp"
#include "light/rendering/buffer.hpp"
#include "light/rendering/lights.hpp"

namespace Light
{
	Scene::Scene() 
	{
		m_skybox.reset(Light::Cubemap::create("assets/cubemap"));
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
	

	void Scene::update(Timestep)
	{
		
	}

}