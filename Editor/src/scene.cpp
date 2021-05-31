#include "light.hpp"
#include "scene.hpp"
#include "entity.hpp"
#include "components.hpp"

Entity Scene::addEntity() {
    return {m_registry.create(), this};
}

void Scene::update(Light::Timestep dt) {
    auto view = m_registry.view<TransformComponent, Interactive>();
    for (auto entity : view) {
        auto& t = view.get<TransformComponent>(entity);
        if(Light::Input::isKeyPressed(LIGHT_KEY_A))
        {
            t.position.x -= 1.0 * dt.getSeconds();
        }
        else if(Light::Input::isKeyPressed(LIGHT_KEY_D))
        {
            t.position.x += 1.0 * dt.getSeconds();
        }
        else if(Light::Input::isKeyPressed(LIGHT_KEY_W))
        {
            t.position.z += 1.0 * dt.getSeconds();
        }
        else if(Light::Input::isKeyPressed(LIGHT_KEY_S))
        {
            t.position.z -= 1.0 * dt.getSeconds();
        }
        else if(Light::Input::isKeyPressed(LIGHT_KEY_1))
        {
            t.rotation.x += 2.0 * dt.getSeconds();
        }
        else if(Light::Input::isKeyPressed(LIGHT_KEY_2))
        {
            t.rotation.x -= 2.0 * dt.getSeconds();
        }
        else if(Light::Input::isKeyPressed(LIGHT_KEY_3))
        {
            t.scale += 0.2 * dt.getSeconds();
        }
        else if(Light::Input::isKeyPressed(LIGHT_KEY_4))
        {
            t.scale -= 0.2 * dt.getSeconds();
        }
    }
}

void Scene::render() {
    auto view1 = m_registry.view<ShaderComponent, VertexArrayComponent, TransformComponent>();
    for (auto& entity : view1) {
        auto [s, v, t] = view1.get(entity);
        Light::Renderer::submit(s.shader, v.vertexArray, t.getTransform());
    }
    auto view2 = m_registry.view<ShaderComponent, VertexArrayComponent, CubemapComponent>();
    for (auto& entity : view2) {
        auto [s, v, c] = view2.get(entity);
        c.bind();
        Light::Renderer::submitSkybox(s.shader, v.vertexArray);
    }
}