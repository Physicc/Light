#include "ecs/entity.hpp"
#include "entt.hpp"

namespace Light
{
	Entity::Entity(entt::entity entity, Scene* scene) : m_entity(entity), m_scene(scene) {}
}