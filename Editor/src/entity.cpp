#include "entity.hpp"
#include "../libs/entt.hpp"

Entity::Entity(entt::entity entity, Scene* scene) : m_entity(entity), m_scene(scene) {}

