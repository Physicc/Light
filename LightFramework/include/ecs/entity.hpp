#ifndef __ENTITY_HPP__
#define __ENTITY_HPP__

#include "entt.hpp"
#include "ecs/scene.hpp"
#include "core/assert.hpp"
#include "ecs/components.hpp"

namespace Light
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity entity, Scene* scene);
		Entity(const Entity& other) = default;

		Entity& operator=(const Entity& other) = default;

		template<typename T, typename... Args>
		inline T& addComponent(Args... args)
		{
			LIGHT_CORE_ASSERT(!hasComponent<T>(), "Entity already has component!");
			return m_scene->m_registry.emplace<T>(m_entity, std::forward<Args>(args)...);
		}

		template<typename T>
		inline bool hasComponent()
		{
			return m_scene->m_registry.any_of<T>(m_entity);
		}

		template<typename T>
		inline T& getComponent()
		{
			LIGHT_CORE_ASSERT(hasComponent<T>(), "Entity does not have component!");
			return m_scene->m_registry.get<T>(m_entity);
		}

		template<typename T>
		inline void removeComponent()
		{
			LIGHT_CORE_ASSERT(hasComponent<T>(), "Entity does not have component!");
			m_scene->m_registry.remove<T>(m_entity);
		}

		inline operator bool()
		{
			return m_scene != nullptr && m_scene->m_registry.valid(m_entity);
		}

		inline operator uint32_t()
		{
			return (uint32_t)m_entity;
		}

		inline bool operator==(const Entity& other)
		{
			return m_entity == other.m_entity && m_scene == other.m_scene;
		}

		inline std::string getUUID()
		{
			return getComponent<TagComponent>().uuid;
		}

	private:
		entt::entity m_entity{entt::null};
		Scene* m_scene = nullptr;
	};
}

#endif // __ENTITY_HPP__
