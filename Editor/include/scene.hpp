#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include "entt.hpp"

class Entity;

class Scene {
    Scene() = default;
    Entity addEntity();
private:
    entt::registry m_registry;

    friend class Entity;
};

#endif // __SCENE_HPP__
