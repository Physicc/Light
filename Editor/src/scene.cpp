#include "light.hpp"
#include "scene.hpp"
#include "entity.hpp"

Entity Scene::addEntity() {
    return {m_registry.create(), this};
}

